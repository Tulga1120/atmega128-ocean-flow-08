# Hey Emacs, this is a -*- makefile -*-
# By	 : Gary Lee
# E-mail : dreamer2006@163.com
# Addr	 : Ocean University of China
#----------------------------------------------------------------------------
# On command line:
# make all 			: Make software.
# make clean 		: Clean out built project files.
# make coff 		: Convert ELF to AVR COFF.
# make extcoff 		: Convert ELF to AVR Extended COFF.
# make program 		: Download the hex file to the device, using avrdude.
#                	  Please customize the avrdude settings below first!
# make debug 		: Start either simulavr or avarice as specified for debugging, 
#              		  with avr-gdb or avr-insight as the front end for debugging.
# make filename.s 	: Just compile filename.c into the assembler code only.
# make filename.i 	: Create a preprocessed source file for use in submitting
#                     bug reports to the GCC project.
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

# MCU name
MCU = ATMega128

# Processor frequency.
F_CPU = 8000000

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Target file name (without extension).
TARGET = ocean_flow

TOPDIR 		= $(shwll pwd)
INCPATH  	= $(TOPDIR)
OUTPUTDIR 	= objs

CC  = iccavr
LIB = ilibw

export TOPDIR OUTPUTDIR MCU

CPU_C  = uart.c    	\
         menu.c		\
		 common.c	\
		 cs5463.c	\
		 key.c		\
		 lcd.c		\
		 main.c		\
		 measure.c	\
		 memory.c	\
		 pcf8563.c
		 		 
APPS   =

# List C source files here. (C dependencies are automatically generated.)
SRC = $(CPU_C)      \
      $(APPS)       \

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT =

# Debugging format.
DEBUG =

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = $(INCPATH)

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD =

# Place -D or -U options here
CDEFS = -D__ICC_VERSION=722 -D$(MCU)

# Place -I options here
CINCS  = -Ic:\iccv7avr\include
CINCS1 = -Ic:\iccv7avr\lib

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing 
CFLAGS  = $(CINCS) -e $(CDEFS) -l -g -Wa$(CINCS1) -MLongJump -MHasMul -MEnhanced -Wf-use_elpm
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

export CFLAGS
#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = $(CFLAGS) 
export ASFLAGS
#---------------- Library Options ----------------
# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB = 


# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB = 

MATH_LIB = -l

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
	  
LDFLAGS  = -Lc:\iccv7avr\lib -g -e:0x20000 -ucrtatmega.o
LDFLAGS += -bfunc_lit:0x8c.0x20000 -dram_end:0x10ff \
		   -bdata:0x100.0x10ff -dhwstk_size:70 -beeprom:0.4096 -fihx_coff -S2
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

#============================================================================

# Define programs and commands.
SHELL 		= shell
CC 			= iccavr
OBJCOPY 	= 
OBJDUMP 	= 
SIZE 		= 
NM 			= 
AVRDUDE 	= 
REMOVE 		= rm -f
REMOVEDIR	= rm -fr
COPY 		= cp
WINSHELL 	= cmd

export CC

# Define Messages
# English
MSG_ERRORS_NONE 	= Errors: none
MSG_BEGIN 			= -------- 开始编译 --------
MSG_END   			= -------- 编译完成 --------
MSG_AUTHOR 			= "By: Gary Lee <richard.lion.heart.gl@gmail.com>"
MSG_SIZE_BEFORE 	= Size before: 
MSG_SIZE_AFTER 		= Size after:
MSG_COFF 			= Converting to AVR COFF:
MSG_EXTENDED_COFF 	= Converting to AVR Extended COFF:
MSG_FLASH 			= Creating load file for Flash:
MSG_EEPROM 			= Creating load file for EEPROM:
MSG_EXTENDED_LISTING= Creating Extended Listing:
MSG_SYMBOL_TABLE 	= Creating Symbol Table:
MSG_LINKING 		= Linking:
MSG_COMPILING 		= Compiling:
MSG_ASSEMBLING 		= Assembling:
MSG_CLEANING 		= Cleaning project:

export MSG_COMPILING MSG_ASSEMBLING
# Define all object files.
OBJ = $(SRC:%.c=$(OUTPUTDIR)/%.o)

# Define all listing files.
LST = $(SRC:%.c=$(OUTPUTDIR)/%.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS  = -I. $(CFLAGS)
ALL_ASFLAGS = -I. $(ASFLAGS)

export ALL_CFLAGS ALL_ASFLAGS
# Default target.
all: begin gccversion build end author

build: hex

hex: $(TARGET)

# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN)
	
author:
	@echo ================================================
	@echo =$(MSG_AUTHOR)=
	@echo ================================================
	
end:
	@echo $(MSG_END)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(LDFLAGS) $^ -lcatm128
	
# Display compiler version information.
gccversion : 
	@$(CC) -v

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(SRC:%.c=$(OUTPUTDIR)/%.o)
	$(REMOVE) $(SRC:%.c=$(OUTPUTDIR)/%.lst)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRC:.c=.i)
	$(REMOVE) *.lst *.mp *.dbg *.cof *.hex
	$(REMOVEDIR) .dep
	$(REMOVEDIR) ./objs

# Create object files directory
$(shell mkdir $(OUTPUTDIR) 		2>/dev/null)

# Include the dependency files.

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff clean clean_list

include rules.mk

debug:
	@echo "topdir=$(TOPDIR)"
	@echo "objs=$(OBJ)"