ifneq ($(TARGET_SIMULATOR),true)

# Build the unit tests.
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# Build the unit tests.
test_src_files := \
	ObbFile_test.cpp \
	Looper_test.cpp \
	String8_test.cpp

shared_libraries := \
	libz \
	liblog \
	libcutils \
	libutils \
	libstlport

static_libraries := \
	libgtest \
	libgtest_main

c_includes := \
    external/zlib \
    external/icu4c/common \
    bionic \
    bionic/libstdc++/include \
    external/gtest/include \
    external/stlport/stlport

module_tags := eng tests

$(foreach file,$(test_src_files), \
    $(eval include $(CLEAR_VARS)) \
    $(eval LOCAL_SHARED_LIBRARIES := $(shared_libraries)) \
    $(eval LOCAL_STATIC_LIBRARIES := $(static_libraries)) \
    $(eval LOCAL_C_INCLUDES := $(c_includes)) \
    $(eval LOCAL_SRC_FILES := $(file)) \
    $(eval LOCAL_MODULE := $(notdir $(file:%.cpp=%))) \
    $(eval LOCAL_MODULE_TAGS := $(module_tags)) \
    $(eval include $(BUILD_EXECUTABLE)) \
)

ifneq ($(TARGET_SIMULATOR),true)
LOCAL_C_INCLUDES += bionic		# very important!
LOCAL_C_INCLUDES += external/stlport/stlport 
LOCAL_SHARED_LIBRARIES += libstlport libdl
endif

LOCAL_CFLAGS += -nostdinc

endif