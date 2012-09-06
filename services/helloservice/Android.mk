LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	HelloService.cpp \
	IHelloServer.cpp \

LOCAL_CFLAGS := -DLOG_TAG=\"HELLO_SERVICE\"

# need "-lrt" on Linux simulator to pick up clock_gettime
ifeq ($(TARGET_SIMULATOR),true)
	ifeq ($(HOST_OS),linux)
		LOCAL_LDLIBS += -lrt -lpthread
	endif
endif

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder 

LOCAL_MODULE_TAGS := eng
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE:= libHelloService

include $(BUILD_SHARED_LIBRARY)


################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	MainHelloService.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := eng
LOCAL_SHARED_LIBRARIES += libHelloService

LOCAL_MODULE:= MainHelloService

include $(BUILD_EXECUTABLE)




################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	ClientHelloService.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := eng
LOCAL_SHARED_LIBRARIES += libHelloService

LOCAL_MODULE:= ClientHelloService

include $(BUILD_EXECUTABLE)


