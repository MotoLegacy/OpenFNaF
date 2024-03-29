NAME            	:= OpenFNaF

#------------------------------------------------

BUILD          		:= build
SOURCE         		:= src/
PC_WRAPPER_SRC 		:= src/pc/
PSP_WRAPPER_SRC		:= src/psp/

#------------------------------------------------

SOURCES        		:= $(shell find $(SOURCE) -maxdepth 1 -name '*.c')

# PLATFORM: PC
ifeq ($(DESKTOP),1)
SOURCES        		+= $(shell find $(PC_WRAPPER_SRC) -name '*.c')
CFLAGS         		:= -DDESKTOP -fPIC
endif

# PLATFORM: PSP
ifeq ($(PSP),1)
SOURCES 			+= $(shell find $(PSP_WRAPPER_SRC) -name '*.c')
SOURCES 			+= libs/gLib2D/glib2d.c
CFLAGS 				:= -DPSP
endif

OBJECTS        		:= $(addprefix $(BUILD)/,$(SOURCES:%.c=%.o))

#-------------------------------------------------

CFLAGS         		+= -Wall -g
LDFLAGS        		:= -llua -lm -lc

# PLATFORM: PC
ifeq ($(DESKTOP),1)
LDFLAGS 			+= -lraylib
endif

# PLATFORM: PSP
ifeq ($(PSP),1)
LDFLAGS 			+= libs/libosl.a
LDFLAGS 			+= -lpspgum_vfpu -lpspvfpu -lpspgu -lpspvram -lpspaudiocodec
LDFLAGS 			+= -lpspaudiolib -lpspaudio -lpspmp3 -lpsprtc -lpsppower -lpsphprm -ljpeg -lpng -lz
endif

# PSP: Uses more automated building tech using its custom SDK build.mak
ifneq ($(PSP),1)
#-------------------------------------------------

OUTPUT_BOLD    		:= `tput bold`
OUTPUT_GREEN   		:= `tput setaf 2`
OUTPUT_BLUE    		:= `tput setaf 6`
OUTPUT_NORMAL  		:= `tput sgr0`

#-------------------------------------------------
all: $(NAME)
#-------------------------------------------------

#-------------------------------------------------
$(NAME): $(OBJECTS)
#-------------------------------------------------
	@echo Linking...
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LDFLAGS)
	@mv $(NAME) pc/
	@echo $(OUTPUT_BOLD)$(OUTPUT_GREEN)Build done.$(OUTPUT_NORMAL)

#-------------------------------------------------
$(BUILD)/%.o: %.c
#-------------------------------------------------
	@echo $(OUTPUT_BOLD)- $(subst $(SOURCE)/,,$<)$(OUTPUT_NORMAL)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(dir $<) -c $< -o $@

#-------------------------------------------------
clean:
#-------------------------------------------------
	@echo $(OUTPUT_BLUE)$(OUTPUT_BOLD)Cleaning...$(OUTPUT_NORMAL)
	@rm -rf $(BUILD) $(NAME)
endif

# Special PSP Building
ifeq ($(PSP),1)
TARGET 				= $(NAME)
INCDIR 				= $(PSPPATH)/include
INCDIR 				+= libs
OBJS				= $(SOURCES)
LIBS 				= $(LDFLAGS)
EXTRA_TARGETS 		= EBOOT.PBP
PSP_EBOOT_TITLE 	= OpenFNaF
PSPSDK				= $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

install: EBOOT.PBP
	mv EBOOT.PBP psp/
endif