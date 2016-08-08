#============================================================================
#   ����Ŀ����
#============================================================================
#include ../dir.rule

TB_APPDIR         = `pwd`/..
TARGET		= cw_ui
#INSTALL_DIR	= $(TB_APPDIR)/bin/apps
#============================================================================
#   ���ñ�����
#============================================================================
#BUILD_TOOL_DIR      := /usr/local/arm-2011.09
#PREFIX   := $(BUILD_TOOL_DIR)/bin/arm-none-linux-gnueabi-
PREFIX		= arm-linux-
CC		= $(PREFIX)gcc
CPP		= $(PREFIX)g++
CXX		= $(PREFIX)g++
C++		= $(PREFIX)g++
STRIP	= $(PREFIX)strip

#============================================================================
#   ������ʱ�ļ�
#============================================================================
CPP_SRC		= $(shell find ./src -name "*.cpp")
CPP_OBJS	= $(CPP_SRC:%.cpp=%.o)

C_SRC		= $(shell find ./src -name "*.c")
C_OBJS		= $(C_SRC:%.c=%.o)

#============================================================================
#   ���ҵ�ǰ·���µ�ͷ�ļ�·��
#============================================================================
INC_DIRS	= $(shell find ./inc -name "*.h")
INC_DIRS	:= $(foreach incdir, $(INC_DIRS), $(dir $(incdir)))
INC_DIRS	:= $(sort $(INC_DIRS))
INC_DIRS	:= $(foreach incdir, $(INC_DIRS), -I$(incdir))

#============================================================================
#   �ⲿͷ�ļ���·��
#============================================================================
EXTRA_INC_DIRS	+= depend/build/include/directfb
EXTRA_INC_DIRS	+= depend/build/include
EXTRA_INC_DIRS	+=  $(TB_APPDIR)/libs/include
EXTRA_INC_DIRS	+=  $(TB_APPDIR)/libs/msg_drivers/inc
EXTRA_INC_DIRS	+=  $(TB_APPDIR)/libs/utils/inc
EXTRA_INC_DIRS	+=  $(TB_APPDIR)/libs/msg_util/inc
EXTRA_INC_DIRS	+=  $(TB_APPDIR)/libs/avi/inc
EXTRA_INC_DIRS	+=  $(TB_APPDIR)/server/lib/include/timechip
EXTRA_INC_DIRS	+= $(LINUX_SRC_DIR)/include
EXTRA_INC_DIRS	:= $(foreach incdir, $(EXTRA_INC_DIRS), -I$(incdir))

#============================================================================
#   �ⲿ���ӿ�·��������
#============================================================================
EXTRA_LIB_DIRS	+= depend/build/lib
EXTRA_LIB_DIRS	+= /usr/local/arm_linux_4.2/arm-linux/lib

EXTRA_LIB_DIRS	:= $(foreach incdir, $(EXTRA_LIB_DIRS), -L$(incdir))

EXTRA_LIBS	+= -ldirectfb
EXTRA_LIBS	+= -ldirect
EXTRA_LIBS	+= -lfusion
EXTRA_LIBS	+= -llite
EXTRA_LIBS	+= -lleck
EXTRA_LIBS	+= -ljpeg
EXTRA_LIBS	+= $(TB_APPDIR)/libs/msg_drivers/libmsg_drivers.a
EXTRA_LIBS	+= $(TB_APPDIR)/libs/msg_util/libmsg_util.a
EXTRA_LIBS	+= $(TB_APPDIR)/libs/utils/libutils.a
EXTRA_LIBS	+= -liconv
EXTRA_LIBS	+= $(TB_APPDIR)/libs/avi/libavi.a
#EXTRA_LIBS	+= $(TB_APPDIR)/server/lib/src/timechip/cw_timechip.o

#============================================================================
#   ���ɱ�������  -Wno-write-strings // ��ʱȥ��const string�ľ���
#============================================================================
CFLAGS		+= -muclibc -g -Wall -O2 -Wno-write-strings -fpermissive #-D__UI_TEST__
CFLAGS		+= $(DEFS)
CFLAGS		+= $(INC_DIRS)
CFLAGS		+= $(EXTRA_INC_DIRS)

#============================================================================
#   ������������
#============================================================================
LDFLAGS		+= -lpthread
LDFLAGS		+= $(EXTRA_LIB_DIRS)
LDFLAGS		+= $(EXTRA_LIBS)

all: $(TARGET)
  
$(TARGET):$(CPP_OBJS) $(C_OBJS) 
	$(C++) -muclibc -o $(TARGET) $^ $(LDFLAGS) -ldl 

.cpp.o:
	$(C++) -c $(CFLAGS) $< -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -rf $(TARGET) $(C_OBJS)  $(CPP_OBJS) 

del:
	rm $(C_OBJS) $(CPP_OBJS)

install: $(TARGET)
	install $(TARGET) $(INSTALL_DIR)
