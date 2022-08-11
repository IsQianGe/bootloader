
C_INCLUDES +=  -Ilibraries/SFUD/sfud/inc

C_SOURCES += $(wildcard libraries/sfud/src/*.c)

CFLAGS += -Wno-error=discarded-qualifiers

C_INCLUDES += -Ilibraries/littlefs

C_SOURCES += libraries/littlefs/lfs.c \
		 libraries/littlefs/lfs_util.c \

CFLAGS += -Wno-error=unused-function