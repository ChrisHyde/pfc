#########################################################################################################
# File Name   : Makefile
# Copyright   : Christopher T. Hyde
# Version     : 1.0
# File type   : Makefile for GNU Make 3.81
# Compiler    : g++ 
# Linker      : GNU ld (GNU Binutils for Debian) 2.18.0.20080103
# Project     : Indoor Robot
# Description : to build a project
# Rules		  : compileIdl, cleanAll, all, clean
#					 On compilation specify operating system means: OS=LINUX or OS=XENOMAI
#               
# 
# History     :  Makefile to compile in Xenomai
# TODO        : 
##########################################################################################################


#
#Declarations
#
LENGUAJE	= GCC
SRC_DIR 	= src
SRC      = autopilot.c

# 
# Find out the compilation mode.
#
ifeq ($(RELEASE_MODE),1)
        BINARY_FOLDER	= release
        DEBUG_OPTIONS	=
        DEBUG_NAME		=
else
        BINARY_FOLDER	= debug
        DEBUG_OPTIONS	= -g -Wall
        DEBUG_NAME		= d
endif

#
# Find out the operating system wrapping.
#
ifeq ($(OS), XENOMAI)
			LIB_OS_OPTIONS=-DXENOMAI_RTOS
			LIB_OS_WRAPPER_NAME=xenomai
else
ifeq ($(OS), LINUX)
        LIB_OS_OPTIONS=-DLINUX_OS
        LIB_OS_WRAPPER_NAME=linux
else
        LIB_OS_OPTIONS=-DDUMMY
        LIB_OS_WRAPPER_NAME=dummy
endif
endif

#
# These source files contains the business logical  
#
OUTPUT_EXEC = $(BINARY_FOLDER)/autopilot
#NAME_LIB    = ixwriter$(DEBUG_NAME)
#OUTPUT_LIB  = lib/lib$(NAME_LIB).so

#
# Objects Files
#
OBJECTS 		=	$(addprefix $(BINARY_FOLDER)/, $(SRC:.c=.o))

#
# EXECUTABLE
#
EXEC   =  $(OUTPUT_EXEC)

#
# INCLUDES + LIBS
#
PFC_INCLUDE_DIR	= -I/usr/local/include/pfc
PFC_LIBS_DIR	= -L/usr/local/lib/pfc
INCLUDE_DIR	= 	  -Iinclude -I../modules/include  -I../modules/xplane_comm/include -I../modules/apc220_comm/include -I../modules/controller_comm/include -I../modules/panel_comm/include -I/usr/xenomai/include/ -I /usr/xenomai/  $(PFC_INCLUDE_DIR)	
LIBS_DIR	=	-L. $(PFC_LIBS_DIR)

#
# Dynamics Libs
# 
syslibs   = -L/usr/xenomai/lib -lpthread_rt -lxenomai -lpthread -lrt  -lnative  -lm

LIBS= xplane_comm$(DEBUG_NAME) -l apc220_comm$(DEBUG_NAME) -l controller_comm$(DEBUG_NAME) -l panel_comm$(DEBUG_NAME)  $(syslibs)   

COMPILER          =  gcc -Werror  -fmessage-length=0   -fomit-frame-pointer --pipe -fPIC $(DEBUG_OPTIONS)  $(LIB_OS_OPTIONS)

#-lnative -lxenomai -lrt -lpthread -lrtdk  
# Rules
#
all:  cleanbinary $(OBJECTS)  apps 

cleanbinary:
	-rm $(BINARY_FOLDER)/* 1>/dev/null 2>/dev/null
	-mkdir -p $(BINARY_FOLDER)



apps: $(EXEC:%=%.outaux)
%.outaux : %.o
	-mkdir -p bin
	$(COMPILER) -o $(@:%.outaux=%) $(@:%.outaux=%.o) $(LIBS_DIR) -L lib/ -l$(NAME_LIB) $(LIBS) 
	cp -f $(@:%.outaux=%) bin/$(notdir $(@:%.outaux=%))$(DEBUG_NAME)
	
	
$(BINARY_FOLDER)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(COMPILER) -o $@ $(INCLUDE_DIR) -c $<
				
clean: 
	-rm -r bin/ 
	-rm -r debug/ 
	-rm -r release/ 