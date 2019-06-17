CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

SRCDIR=src
BLDDIR=bin
OBJDIR=$(BLDDIR)/obj

EXEC=virtex
SOURCES=$(wildcard $(SRCDIR)/*.c)
# OBJECTS=$(SOURCES:$(SRCDIR)/%.c,$(OBJDIR)/%.o)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

build: $(EXEC)

clean:
	rm -rf $(BLDDIR)

.PHONY: build clean

$(EXEC): $(OBJECTS)
	@echo "Linking objects"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful"

$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -c $<

