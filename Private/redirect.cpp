#include "Public/opts.h"
#include "Public/log.h"
#include "Public/curl.h"
#include "Public/redirect.h"
#include "Public/epicdomains.h"

#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>

extern "C" {

CurlEasySetOptFn OGCurlEasySetOpt = nullptr;

}

bool IsValidCString(const char* p) {
    if (!p) return false;
    size_t len = 0;
    while (len < 2048 && p[len] != '\0') ++len;
    return (len > 0 && len < 2048);
}

static std::wstring StringToLowerWString(const std::string& s) {
    std::wstring ws;
    ws.reserve(s.size());
    for (char c : s) {
        ws.push_back(towlower(static_cast<wchar_t>(c)));
    }
    return ws;
}

static bool EndsWithCI(const std::wstring& str, const std::wstring& suffix) {
    if (suffix.size() > str.size()) return false;

    auto strIt = str.rbegin();
    auto suffixIt = suffix.rbegin();

    while (suffixIt != suffix.rend()) {
        if (towlower(*strIt) != towlower(*suffixIt)) return false;
        ++strIt;
        ++suffixIt;
    }
    return true;
}

bool IsEpicHost(const std::string& host) {
    std::wstring wHost = StringToLowerWString(host);

    for (const auto& domain : epicDomains) {
        if (EndsWithCI(wHost, domain)) {
            return true;
        }
    }
    return false;
}

static void ParseUrl(const std::string& url, std::string& host, std::string& pathAndQuery) {
    size_t schemePos = url.find("://");
    size_t hostStart = (schemePos == std::string::npos) ? 0 : schemePos + 3;

    size_t pathStart = url.find('/', hostStart);
    if (pathStart == std::string::npos) {
        host = url.substr(hostStart);
        pathAndQuery = "/";
    } else {
        host = url.substr(hostStart, pathStart - hostStart);
        pathAndQuery = url.substr(pathStart);
    }
}

void* hookedCurlEasySetOpt(void* curl, int option, void* arg) {
    static thread_local std::string redirectedUrl;

    if (option == CURLOPT_URL && IsValidCString(reinterpret_cast<const char*>(arg))) {
        const char* urlCStr = reinterpret_cast<const char*>(arg);
        std::string url(urlCStr);
        std::string host, pathAndQuery;

        ParseUrl(url, host, pathAndQuery);

        if (IsEpicHost(host)) {
            redirectedUrl = std::string(BACKEND_URL) + pathAndQuery;
            LOGI("Redirected URL from %s to %s", urlCStr, redirectedUrl.c_str());
            return OGCurlEasySetOpt(curl, option, (void*)redirectedUrl.c_str());
        }
    }
    else if (option == CURLOPT_SSL_VERIFYPEER) {
        return OGCurlEasySetOpt(curl, option, (void*)0);
    }

    return OGCurlEasySetOpt(curl, option, arg);
}