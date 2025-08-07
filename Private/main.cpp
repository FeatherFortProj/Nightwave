#include <dlfcn.h>
#include <pthread.h>
#include <android/log.h>
#include "Public/Dobby/dobby.h"
#include "Public/log.h"
#include "redirect.cpp"
#include "Public/redirect.h"

#ifndef RS_SUCCESS
#define RS_SUCCESS 0
#endif

void* HookThread(void*) {
    LOGI("Loading libUE4.so...");
    void* handle = dlopen("libUE4.so", RTLD_NOW);
    if (!handle) {
        LOGE("Failed to load libUE4.so");
        return nullptr;
    }

    void* CurlEasySetOpt = dlsym(handle, "curl_easy_setopt");
    if (!CurlEasySetOpt) {
        LOGE("Failed to find curl_easy_setopt symbol");
        dlclose(handle);
        return nullptr;
    }

    LOGI("Found curl_easy_setopt at %p", CurlEasySetOpt);

    if (DobbyHook(CurlEasySetOpt, (void*)hookedCurlEasySetOpt, (void**)&OGCurlEasySetOpt) == RS_SUCCESS) {
        LOGI("Successfully hooked curl_easy_setopt");
    } else {
        LOGE("Failed to hook curl_easy_setopt");
    }

    return nullptr;
}

__attribute__((constructor)) void Main() {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, HookThread, nullptr);
}
