
C_INCLUDES += -Ilibraries/SFUD/sfud/inc
C_INCLUDES += -Icomponents/sfud

C_SOURCES += $(wildcard libraries/SFUD/sfud/src/*.c)
C_SOURCES += $(wildcard components/sfud/*.c)

CFLAGS += -Wno-error=discarded-qualifiers

C_INCLUDES += -Ilibraries/littlefs 
C_INCLUDES += -Icomponents/littlefs

C_SOURCES += libraries/littlefs/lfs.c \
		 libraries/littlefs/lfs_util.c 
C_SOURCES += $(wildcard components/littlefs/*.c)	 

CFLAGS += -Wno-error=unused-function