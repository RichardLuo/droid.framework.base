LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := RLooperTest.cpp
# LOCAL_SRC_FILES :=  ALooperTest.cpp UDPPusher.cpp

LOCAL_C_INCLUDES:= \
    frameworks/base/include/media/stagefright/foundation

LOCAL_SHARED_LIBRARIES := \
		libstagefright_foundation \
        libbinder         \
        libutils          \
        libcutils


LOCAL_CFLAGS += -Wno-multichar

# LOCAL_MODULE:= ALooperTest
LOCAL_MODULE:= RLooperTest

LOCAL_PRELINK_MODULE:= false

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)
