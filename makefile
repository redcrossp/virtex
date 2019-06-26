CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

SRCDIR=src
BLDDIR=out
OBJDIR=$(BLDDIR)/obj
DEPDIR=$(BLDDIR)/dep

EXEC=$(BLDDIR)/virtex
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
DEPENDS=$(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SOURCES))
# OBJECTS=$(SOURCES:$(SRCDIR)/%.c,$(OBJDIR)/%.o)

build: $(EXEC)

clean:
	@echo "Removing $(BLDDIR) directory"
	@rm -rf $(BLDDIR)
	@echo "Clean successful"

.PHONY: build clean

$(EXEC): $(OBJECTS)
	@echo "Linking objects"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful"

$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR) $(DEPDIR)
	@$(CC) $(CFLAGS) -o $@ -MMD -MF $(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$@) -c $<

-include $(DEPENDS)
