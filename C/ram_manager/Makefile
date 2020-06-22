#Compiler and Linker
CC          := gcc

#The Target Binary Program
#Warning: Main src file required by 'target' should be named 'target.src' for proper compilation
TARGET      := ep3

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
BUILDDIR    := obj
TARGETDIR   := ./
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -c -std=gnu99 -Wall -Wextra -O3 -ggdb
LIB         := 
INC         := -I/usr/local/include

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
MAINS       := $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(TARGET)))

#Defauilt Make
all: directories $(TARGET)

#Remake
remake: cleaner $(shell reset) all

#Make the Directories
directories:
		@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
		@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
		@$(RM) -rf

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): %: $(addprefix $(BUILDDIR)/, %.o) $(filter-out $(MAINS), $(OBJECTS)) 
		$(CC) -o $(TARGETDIR)/$@ $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<
		@$(CC) $(CFLAGS) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
		@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
		@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
		@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
		@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources