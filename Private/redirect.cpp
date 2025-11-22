#include "Public/redirect.h"
#include "Public/opts.h"
#include "Public/log.h"
#include "Public/ue.h"

SetUrlFn SetUrlPtr = nullptr;
ProcessRequestFn OGProcessRequest = nullptr;

bool ProcessRequest(FCurlHttpRequest* Request) {
    if (!Request) return false;

    FString& originalUrl = Request->GetURL();
    if (originalUrl.empty()) {
        return OGProcessRequest(Request);
    }

    FString backendUrl(BACKEND_URL);    
    int protoEnd = backendUrl.find(u"://");
    FString backendHost;
    if (protoEnd != -1) {
        int hostStart = protoEnd + 3;
        int hostEnd = backendUrl.find(u"/", hostStart);
        if (hostEnd == -1) hostEnd = backendUrl.len();
        backendHost = backendUrl.substr(hostStart, hostEnd - hostStart);
    } else {
        backendHost = backendUrl;
    }

    if (originalUrl.find(backendHost.c_str()) != -1) {
        return OGProcessRequest(Request);
    }

    bool shouldRedirect = false;
    for (const auto& domain : epicDomains) {
        std::string narrowDomain;
        for (wchar_t wc : domain) {
            if (wc <= 0x7F) narrowDomain += static_cast<char>(wc);
        }
        FString domainStr(narrowDomain.c_str());
        if (originalUrl.find(domainStr.c_str()) != -1) {
            shouldRedirect = true;
            break;
        }
    }
    if (!shouldRedirect) {
        return OGProcessRequest(Request);
    }

    FString pathQuery;
    int protoPos = originalUrl.find(u"://");
    if (protoPos != -1) {
        int hostEnd = originalUrl.find(u"/", protoPos + 3);
        if (hostEnd != -1) {
            pathQuery = originalUrl.substr(hostEnd);
        } else {
            pathQuery = FString(u"/");
        }
    } else {
        int slashPos = originalUrl.find(u"/");
        if (slashPos != -1) {
            pathQuery = originalUrl.substr(slashPos);
        } else {
            pathQuery = FString(u"/");
        }
    }

    if (backendUrl.len() > 0 && backendUrl[backendUrl.len() - 1] == u'/') {
        backendUrl = backendUrl.substr(0, backendUrl.len() - 1);
    }

    FString newUrl;
    
    int32_t backendLen = backendUrl.len() > 0 ? backendUrl.len() - 1 : 0;
    int32_t pathLen = pathQuery.len() > 0 ? pathQuery.len() - 1 : 0;
    int32_t totalLen = backendLen + pathLen;
        
    if (totalLen > 0) {
        newUrl = FString(totalLen);
        if (newUrl.get()) {
            if (backendLen > 0) {
                memcpy(newUrl.get(), backendUrl.c_str(), backendLen * sizeof(FString::CharType));
            }
            if (pathLen > 0) {
                memcpy(newUrl.get() + backendLen, pathQuery.c_str(), pathLen * sizeof(FString::CharType));
            }
            newUrl.get()[totalLen] = 0;
        }
    }

    if (newUrl.empty()) {
        return OGProcessRequest(Request);
    }

    Request->SetURL(newUrl);
    return OGProcessRequest(Request);
}