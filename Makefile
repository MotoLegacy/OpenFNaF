NAME            := OpenFNaF

#------------------------------------------------

BUILD          := build
SOURCE         := src/
PC_WRAPPER_SRC := src/engine/pc/

#------------------------------------------------

SOURCES        := $(shell find $(SOURCE) -maxdepth 2 -name '*.c')

# PLATFORM: PC
ifeq ($(DESKTOP),1)
SOURCES        += $(shell find $(PC_WRAPPER_SRC) -name '*.c')
CFLAGS         := -DDESKTOP
endif

OBJECTS        := $(addprefix $(BUILD)/,$(SOURCES:%.c=%.o))

#-------------------------------------------------

CFLAGS         += -Wall -g -fPIC 
LDFLAGS        := -lm -lc $(subst sfml,csfml,$(shell pkg-config sfml-all --libs --silence-errors)) $(shell pkg-config sfml-all --libs --silence-errors)

#-------------------------------------------------

OUTPUT_BOLD    := `tput bold`
OUTPUT_GREEN   := `tput setaf 2`
OUTPUT_BLUE    := `tput setaf 6`
OUTPUT_NORMAL  := `tput sgr0`

#-------------------------------------------------
all: $(NAME)
#-------------------------------------------------

#-------------------------------------------------
$(NAME): $(OBJECTS)
#-------------------------------------------------
	@echo Linking...
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LDFLAGS)
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