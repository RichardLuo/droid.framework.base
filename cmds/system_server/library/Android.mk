LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	system_init.cpp

base = $(LOCAL_PATH)/../../..

LOCAL_C_INCLUDES := \
	$(base)/services/camera/libcameraservice \
	$(base)/services/audioflinger \
	$(base)/services/surfaceflinger \
	$(base)/media/libmediaplayerservice \
	$(base)/services/helloservice \
	$(JNI_H_INCLUDE)

LOCAL_SHARED_LIBRARIES := \
	libHelloService \
	libutils \
	libbinder \
	libcutils

LOCAL_MODULE:= libsystem_server

include $(BUILD_SHARED_LIBRARY)
