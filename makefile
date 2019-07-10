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

TEST_SRCDIR=test
TEST_OUTDIR=$(BLDDIR)/test
TEST_GENDIR=$(TEST_OUTDIR)/gen
TEST_OBJDIR=$(TEST_OUTDIR)/obj

TEST_EXEC=$(TEST_OUTDIR)/virtex_test
TEST_MAIN=$(TEST_GENDIR)/main.gen.c
TEST_MAINOBJ=$(TEST_OBJDIR)/main.gen.o
TEST_SOURCES=$(wildcard $(TEST_SRCDIR)/*)
TEST_OBJECTS=$(patsubst $(TEST_SRCDIR)/%.test.c,$(TEST_OBJDIR)/%.gen.o,$(TEST_SOURCES))
TEST_GENERATES=$(patsubst $(TEST_SRCDIR)/%.test.c,$(TEST_GENDIR)/%.gen.c,$(TEST_SOURCES))

build: $(EXEC)

test: $(TEST_EXEC)

clean:
	@echo "Removing $(BLDDIR) directory"
	@rm -rf $(BLDDIR)
	@echo "Clean successful"

.PHONY: build clean test


$(EXEC): $(OBJECTS)
	@echo "Linking objects"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful"

$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR) $(DEPDIR)
	@$(CC) $(CFLAGS) -o $@ -MMD -MF $(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$@) -c $<

$(TEST_EXEC): $(TEST_MAINOBJ) $(TEST_OBJECTS) $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	@echo "Linking objects"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Test build successful"

$(TEST_MAIN) $(TEST_GENERATES): $(TEST_SOURCES)
	@echo "Generating tests"
	@mkdir -p $(TEST_GENDIR)
	@scripts/generateTests $(TEST_SRCDIR) $(TEST_GENDIR) "../../../src" $(TEST_SOURCES)

$(TEST_MAINOBJ): $(TEST_MAIN)
	@echo "Compiling $<"
	@mkdir -p $(TEST_OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(TEST_OBJECTS): $(TEST_OBJDIR)/%.gen.o: $(TEST_GENDIR)/%.gen.c $(SRCDIR)/%.h
	@echo "Compiling $<"
	@mkdir -p $(TEST_OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

-include $(DEPENDS)
