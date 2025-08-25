LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)


LOCAL_SRC_FILES := Public/Dobby/arm64-v8a/libdobby.a


LOCAL_MODULE := dobby
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := nightwave

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -g --std=c++2a

LOCAL_STATIC_LIBRARIES := dobby

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)

LOCAL_SRC_FILES := Private/main.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
