#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------

# use VERBOSE=1 to show full output of commands
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  QUIET := @
endif

#-------------------------------------------------------------------------------
# Tools
#-------------------------------------------------------------------------------

# C compiler
CC := cc
# C compiler flags
CFLAGS := -O0 -g -Wall -fPIC \
	$(shell pkg-config --cflags glib-2.0 pango cairo atk gdk-pixbuf-2.0) \
	-Iinclude -Istlwrt/include -Istlwrt/include/x11 \
	-DSTLWRT_COMPILATION -DGETTEXT_PACKAGE='"stlwrt"'
# Linker
LD := cc
# Linker flags
LDFLAGS := -shared $(shell pkg-config --libs glib-2.0 pango cairo atk gdk-pixbuf-2.0)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

# Main target to build
TARGET := libstlwrt.so
# Files containing source code
SOURCE_FILES := $(wildcard src/*.c)
# Generated header files
GENERATED_HEADERS := include/config.h include/gdkconfig.h
# Compiled object files
O_FILES := $(addsuffix .o,$(basename $(SOURCE_FILES)))
# Dependency files
DEP_FILES := $(O_FILES:.o=.dep)

#-------------------------------------------------------------------------------
# Recipes
#-------------------------------------------------------------------------------

# Remove built-in nonsense rules
MAKEFLAGS += -r
.SUFFIXES:

$(TARGET): $(O_FILES)

# Remove all build artifacts
clean:
	$(RM) $(TARGET) $(O_FILES) $(GENERATED_HEADERS) $(DEP_FILES)

# Compile C code
%.o: %.c $(GENERATED_HEADERS)
	@echo Compiling $<
	$(QUIET) $(CC) -MMD -MF $(@:.o=.dep) -MT $@ $(CFLAGS) -o $@ -c $<

# Link shared library
%.so:
	@echo Linking shared library $@
	$(QUIET) $(LD) $(LDFLAGS) $(filter %.o,$^) -o $@

# Generate headers
include/config.h: ; touch $@
include/gdkconfig.h: ; touch $@

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
