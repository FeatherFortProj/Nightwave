#pragma once

#include <android/log.h>
#include "Public/opts.h"

#define LOG_TAG "Nightwave"

#define LOGI(...) do { if constexpr (ENABLE_LOG) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); } while(0)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
