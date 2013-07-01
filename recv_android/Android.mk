#
LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= main.c \
mm.c \
utils.c\

LOCAL_MODULE:= arecv

LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/include

include $(BUILD_EXECUTABLE)

#	dgcc main.c mm.c util.c -o receiver -Wall -Iinclude -g


#clean:
#	rm -f receiver
