
CURDIR := $(shell pwd)

INSTALL_DIR = 
LIB_DIR = 
INCLUDES +=-I$(CURDIR)/../include 

CC           =  gcc
CXX			 = g++
AR 			 = ar
LIB	    	 = -L$(CURDIR)/../lib -lram_pool
INC_PATH     := $(INCLUDES)

ADD_LIB      = 

DEBUG_DEFINE =
CFLAGS	    := $(DEBUG_DEFINE) -g -Wall -O2 -o 

TARGET      := test_rampool
SRCC	    := $(wildcard *.c)
COBJ	    := $(SRCC:.c=.o)
SRCXX	    := $(wildcard *.cpp)
CXXOBJ      := $(SRCXX:.cpp=.o)
OBJ	        := $(COBJ) $(CXXOBJ)

$(TARGET):$(OBJ)
	$(CXX) $(INC_PATH)  $(CFLAGS) $@ $(OBJ) $(LIB) 

$(COBJ):%.o:%.c 
	$(CXX) $(INC_PATH) $(CFLAGS) $@ -c $<
	
$(CXXOBJ):%.o:%.cpp 
	$(CXX) $(INC_PATH) $(CFLAGS) $@ -c $<

.PHONY:clean
clean:
	rm -f $(TARGET) *~ *.o

install: $(TARGET)
	install $(RT_LIB) $(TARGET_FS)/lib
	install $(TARGET) $(INSTALL_DIR)
