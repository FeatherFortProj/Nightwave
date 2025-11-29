#pragma once
#include "ue.h"
#include "log.h"
#include <unistd.h>
#include <atomic>
#include <cinttypes>
#include <cstring>

static std::atomic<uintptr_t> g_moduleBase {0};
inline uintptr_t GetModuleBase() {
    return g_moduleBase.load(std::memory_order_acquire);
}

inline void SetModuleBase(uintptr_t base) {
    g_moduleBase.store(base, std::memory_order_release);
}

using SetUrlFn = void*(*)(void*, const void*);
extern SetUrlFn SetUrlPtr;

class FCurlHttpRequest {
public:
    FString& GetURL() { return *(FString*)((uint64_t)this + 0x178); }

    void SetURL(const FString& url) {
        if (!SetUrlPtr) {
            LOGE("SetUrlPtr is null in SetURL!");
            return;
        }
        if (url.empty() || url.c_str() == nullptr) {
            LOGE("URL is empty or null");
            return;
        }
        SetUrlPtr((void*)this, (const void*)&url);
    }
};

using ProcessRequestFn = bool(*)(void*);
extern ProcessRequestFn OGProcessRequest;

bool ProcessRequest(FCurlHttpRequest* Request);