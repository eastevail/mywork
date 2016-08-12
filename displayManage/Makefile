#============================================================================
#   ����Ŀ����
#============================================================================
#include ../dir.rule

TB_APPDIR         = `pwd`/..
TARGET		= display
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
C_SRC		= $(shell find ./jpeg_codec -name "*.c")
C_SRC		+= $(shell find ../common/ -name "*.c")
C_OBJS		= $(C_SRC:%.c=%.o)

CPP_SRC		= $(shell find ./ -name "*.cpp")
CPP_SRC		+= $(shell find ../common/ -name "*.cpp")

CPP_OBJS	= $(CPP_SRC:%.cpp=%.o)

#============================================================================
#   ���ҵ�ǰ·���µ�ͷ�ļ�·��
#============================================================================
INC_DIRS	= $(TB_APPDIR)/common/thirdlib/build/include
INC_DIRS	:= $(foreach incdir, $(INC_DIRS), -I$(incdir))
#============================================================================
#   �ⲿͷ�ļ���·��
#============================================================================


EXTRA_INC_DIRS	:= 
EXTRA_INC_DIRS	:= $(foreach incdir, $(EXTRA_INC_DIRS), -I$(incdir))
#============================================================================
#   �ⲿ���ӿ�·��������
#============================================================================

EXTRA_LIB_DIRS	+= /usr/local/arm_linux_4.2/arm-linux/lib
EXTRA_LIB_DIRS	+= $(TB_APPDIR)/common/thirdlib/build/lib
EXTRA_LIB_DIRS	:= $(foreach incdir, $(EXTRA_LIB_DIRS), -L$(incdir))

EXTRA_LIBS	+= -lpthread -lavilib  -levent

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

LDFLAGS		+= $(EXTRA_LIB_DIRS)
LDFLAGS		+= $(EXTRA_LIBS)

all: $(TARGET)
  
$(TARGET):$(CPP_OBJS) $(C_OBJS) 
	$(C++) -muclibc -o $(TARGET) $^ $(LDFLAGS) -ldl 

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(C++) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(TARGET) $(C_OBJS)  $(CPP_OBJS) 

del:
	rm $(C_OBJS) $(CPP_OBJS)

install: $(TARGET)
	install $(TARGET) $(INSTALL_DIR)
