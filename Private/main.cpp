#include <dlfcn.h>
#include <pthread.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include "Public/redirect.h"
#include "Public/log.h"
#include "Public/Dobby/dobby.h"

#ifndef RS_SUCCESS
#define RS_SUCCESS 0
#endif

uintptr_t FindModuleBase(const char* moduleName) {
    FILE* fp = fopen("/proc/self/maps", "r");
    if (!fp) return 0;

    uintptr_t base = 0;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, moduleName)) {
            sscanf(line, "%lx-%*lx %*s %*s %*s %*d", &base);
            break;
        }
    }
    fclose(fp);
    return base;
}

void HookProcessRequest() {
    uintptr_t base = GetModuleBase();
    if (!base) {
        LOGE("Failed to find module base");
        return;
    }

    SetUrlPtr = (SetUrlFn)(base + 0xB98A238);
    void *ProcessRequestAddr = (void *)(base + 0xB98C690);
    if (ProcessRequestAddr) {
        DobbyHook(ProcessRequestAddr, (void *)ProcessRequest, (void **)&OGProcessRequest);
    }    
}

void* HookThread(void*) {
    LOGI("Waiting for libUnreal.so to load...");
    uintptr_t base = 0;
    while (!base) {
        usleep(10000);
        base = FindModuleBase("libUnreal.so");
    }

    SetModuleBase(base);
    HookProcessRequest();
    return nullptr;
}

__attribute__((constructor))
static void Main() {
    pthread_t thread;
    if (pthread_create(&thread, nullptr, HookThread, nullptr) == 0) {
        LOGI("Nightwwave Hook thread initialized");
        pthread_detach(thread);
    } else {
        LOGE("Failed to create hook thread");
    }
}