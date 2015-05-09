LOCAL_PATH := $(call my-dir)

TARGET_FORMAT_STRING_CFLAGS := -Wformat -Wno-error

###################### prebuilt glib-2.0 ###############################
include $(CLEAR_VARS)

LOCAL_MODULE := glib-2.0
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libglib-2.0.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(PREBUILT_STATIC_LIBRARY)

###################### prebuilt libssl ###############################
include $(CLEAR_VARS)

LOCAL_MODULE := ssl
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libssl.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(PREBUILT_SHARED_LIBRARY)

###################### prebuilt libcrypto ###############################
include $(CLEAR_VARS)

LOCAL_MODULE := crypto
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libcrypto.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(PREBUILT_SHARED_LIBRARY)

###################### prebuilt curl ###############################
include $(CLEAR_VARS)

LOCAL_MODULE := curl
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libcurl.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(PREBUILT_STATIC_LIBRARY)

###################### getipd ###############################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := getip.c sendip.c getipd.c

LOCAL_STATIC_LIBRARIES := curl glib-2.0

LOCAL_SHARED_LIBRARIES := ssl crypto

LOCAL_LDLIBS := -fuse-ld=bfd -lz 

LOCAL_MODULE := getipd

LOCAL_CFLAGS+=-O3

include $(BUILD_EXECUTABLE)