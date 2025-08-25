#include "Public/opts.h"
#include "Public/log.h"
#include "Public/redirect.h"
#include "Public/curl.h"

#include <string>
#include <cstring>
#include <string.h>

extern "C" {
    CurlEasySetOptFn OGCurlEasySetOpt = nullptr;
}

static inline char toLowerAscii(char c) {
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;
}

void* hookedCurlEasySetOpt(void* curl, int option, void* arg) {
    static thread_local std::string redirectedUrl;
    CURL* c = reinterpret_cast<CURL*>(curl);

    if (option == CURLOPT_URL && arg) {
        const char* urlCStr = reinterpret_cast<const char*>(arg);
        size_t len = strnlen(urlCStr, 2048);
        if (len == 0 || len >= 2048) return OGCurlEasySetOpt(c, option, arg);

        const char* scheme = strstr(urlCStr, "://");
        const char* hostStart = scheme ? scheme + 3 : urlCStr;
        const char* pathStart = strchr(hostStart, '/');
        std::string host(hostStart, pathStart ? pathStart - hostStart : strlen(hostStart));
        std::string pathAndQuery = pathStart ? pathStart : "/";

        for (char& ch : host) ch = toLowerAscii(ch);

        bool isEpic = false;
        for (const auto& domain : epicDomains) {
            size_t dlen = domain.size();
            size_t hlen = host.size();
            if (dlen <= hlen) {
                bool match = true;
                for (size_t i = 0; i < dlen; ++i) {
                    if (toLowerAscii(host[hlen - dlen + i]) != toLowerAscii(domain[i])) {
                        match = false;
                        break;
                    }
                }
                if (match) { isEpic = true; break; }
            }
        }

        if (isEpic) {
            redirectedUrl = std::string(BACKEND_URL) + pathAndQuery;
            LOGI("Redirected URL from %s to %s", urlCStr, redirectedUrl.c_str());
            return OGCurlEasySetOpt(c, option, (void*)redirectedUrl.c_str());
        }
    } else if (option == CURLOPT_SSL_VERIFYPEER) {
        return OGCurlEasySetOpt(c, option, 0L);
    }

    return OGCurlEasySetOpt(c, option, arg);
}
