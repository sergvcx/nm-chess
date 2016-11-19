ROOT=../.
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:

#NMPP?=
ifndef NMPP
$(error *** ERROR: 'NMPP' environment variable is not defined!  )
endif 

$(info $(NMPP))
INC_DIRS = -I"$(NMPP)/include"
LIB_DIRS = -L"$(NMPP)/lib"  -L$(NMPP)/app/deps/nm_io/lib
LIBS     = nm_io_dummy.lib nmpp-nmc3d.lib libint05.lib  
TARGET   = main.abs
CFG      = emu6405.cfg
SRC_DIRS  = .. ../src/nm ../src/common
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))


VPATH    = $(subst -L,,$(LIB_DIRS)) $(NEURO)/lib
$(info $(NMPP))

$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile $(NMPP)/lib/nmpp-nmc3d.lib
	nmcc -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM) -6405 -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) -heap=0 -heap1=0 -heap2=0 -heap3=0

	
emudbg:
	emudbg $(TARGET)
	
run: $(TARGET) 
	nm6405emurun ./$(TARGET) 

test: .exitcode 
	
ifeq ($(OS),Windows_NT)	
.exitcode: $(TARGET) .test.bat
	.test.bat 
	
.test.bat: 
	@echo nm6405emurun $(TARGET) >.test.bat
	@echo @echo %%errorlevel%% ^>.exitcode >>.test.bat
else 
.exitcode: $(TARGET)
	nm6405emurun ./$(TARGET) ; echo $$? > .exitcode
endif


clean:
	-rm -f *.abs *.elf .*


	

