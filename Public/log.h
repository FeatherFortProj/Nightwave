#pragma once

#include <android/log.h>
#include "Public/opts.h"

#define LOG_TAG "nightwave"

#define LOGI(...) do { __android_log_print(ANDROID_LOG_ERROR, "nightwave", __VA_ARGS__); } while(0)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "nightwave", __VA_ARGS__)

