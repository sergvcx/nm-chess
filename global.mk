
ifndef NMPP
$(error *** ERROR: 'NMPP' environment variable is not defined!  )
endif 

ifndef HAL
$(error *** ERROR: 'HAL' environment variable is not defined!  )
endif 

TEMPLATE_DIR = $(ROOT)/deps/templates
GNUWIN32  ?=c:/gnuwin32/bin
#NMPP       =$(ROOT)
DEVKIT    ?=$(ROOT)/deps/dev-kit-master
ARMTOOLCH ?=$(DEVKIT)/arm-rcm-linux-gnueabihf
NEURO     ?=$(DEVKIT)/nmsdk
VSHELL32  ?=$(DEVKIT)/vshell32
EASYNMC   ?=$(DEVKIT)/nmc-utils-0.1.1/libeasynmc-nmc
MC5103    ?=$(DEVKIT)/mc5103sdk
MC7601    ?=$(DEVKIT)/mc7601sdk
MB7707    ?=$(DEVKIT)/mb7707sdk
MC12101   ?=$(DEVKIT)/mc12101sdk

ifeq ($(OS),Windows_NT)
SHELL       = cmd
export PATH:=$(GNUWIN32);\
$(realpath $(NEURO)/bin);\
$(realpath $(MC12101)/bin);\
$(realpath $(MC5103)/bin);\
$(realpath $(MB7707)/bin);\
$(realpath $(MC7601)/bin);\
$(realpath $(VSHELL32)/bin);\
$(realpath $(ARMTOOLCH)/bin);\
$(PATH);
  
   CROSS_COMPILE=arm-rcm-linux-gnueabihf-
   ARM_CC       =arm-rcm-linux-gnueabihf-gcc.exe
   ARM_AR       =arm-rcm-linux-gnueabihf-ar.exe
   ARM_LD       =arm-rcm-linux-gnueabihf-ld.exe

#  OS_FIND  = $(call BACKSLASH,$(GNUWIN32)/find)
#  OS_DIFF  = $(call BACKSLASH,$(GNUWIN32)/diff)
#  OS_SCP   = $(call BACKSLASH,$(GNUWIN32)/pscp)
#  OS_CAT   = $(call BACKSLASH,$(GNUWIN32)/cat)
   OS_FIND  = gfind
   OS_DIFF  = diff
   OS_SCP   = pscp
   OS_CAT   = cat
    define OSX
	$(subst /,\,$(1))
  endef  
  
#  OS_MV    = rename
   OS_RM    = del /Q
   RM       = rm -f -r
   OS_RD    = rd /Q /S 
#  OS_CP    = $(call BACKSLASH,$(GNUWIN32)/cp)
   OS_CP    = copy
   CP       = cp
   OS_WHICH =$(windir)/system32/where
 

  MB7707MAC ?= 1A-2B-3C-4D-5E-6F
else
  ARM_CC   = arm-module-linux-gnueabi-gcc 
  define OS_PATH
	$(1)
  endef 

  OS_DIFF  = diff   
  OS_SCP   = scp
  OS_RM    = rm -f -r
  RM       = rm -f -r
  OS_RD    = rm -d -r
  OS_FIND  = find
  OS_CAT   = cat
  OS_CP    = cp  
  CP       = cp

  
  define OSX
	$(subst \,/,$(1))
  endef  
  
  OS_WHICH = which
  OS_WGET  = wget  
  OS_UNZIP = unzip
  OS_TAR   = tar xvzf 
  OS_TODIR = -C
  PATH    := $(NEURO)/bin:$(MC5103)/bin:$(MB7707)/bin:$(PATH)
  LD_LIBRARY_PATH = $(MC5103)/bin:$(MB7707)/bin
  MB7707MAC = eth0
endif

#PLATFORMS = vs8 mc5103 emu6405  mb7707 mc12101 gcc mc12101_nmpu1_gcc mc12101_nmpu1 
NMC_PLATFORMS = mc5103 mc7601 mb7707 emu6405 
X86_PLATFORMS = x86
HOST_TARGET_PLATFORMS = host_mc5103 host_mb7707 host_mc7601 host_virtualboard
PLATFORMS = $(NMC_PLATFORMS) $(X86_PLATFORMS) $(HOST_TARGET_PLATFORMS)
TARGET1=mc5103
TARGET2=x86
#mc12101_nmc1


define newline


endef	

define BACKSLASH
	$(subst /,\,$(1))
endef