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

ifeq ($(origin TEST_EXEC), undefined)
	TEST_EXEC=$(TEST_OUTDIR)/virtex_test
	TEST_MAIN=$(TEST_GENDIR)/main.gen.c
else
	TEST_MAIN=$(patsubst $(TEST_OUTDIR)/%,$(TEST_GENDIR)/%.gen.c,$(TEST_EXEC))
endif
TEST_MAINOBJ=$(patsubst $(TEST_GENDIR)/%.gen.c,$(TEST_OBJDIR)/%.gen.o,$(TEST_MAIN))
TEST_SOURCES?=$(wildcard $(TEST_SRCDIR)/*)
TEST_GENERATES=$(patsubst $(TEST_SRCDIR)/%.test.c,$(TEST_GENDIR)/%.gen.c,$(TEST_SOURCES))
TEST_OBJECTS=$(patsubst $(TEST_GENDIR)/%.gen.c,$(TEST_OBJDIR)/%.gen.o,$(TEST_GENERATES))

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

$(TEST_MAINOBJ): $(TEST_MAIN)
	@echo "Compiling $<"
	@mkdir -p $(TEST_OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(TEST_OBJECTS): $(TEST_OBJDIR)/%.gen.o: $(TEST_GENDIR)/%.gen.c $(SRCDIR)/%.h
	@echo "Compiling $<"
	@mkdir -p $(TEST_OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(TEST_MAIN): $(TEST_GENERATES)
	@echo "Generating suite"
	@mkdir -p $(TEST_GENDIR)
	@scripts/genSuiteFromTests $(TEST_MAIN) $(TEST_GENERATES)

$(TEST_GENERATES): $(TEST_GENDIR)/%.gen.c: $(TEST_SRCDIR)/%.test.c
	@echo "Generating $<"
	@mkdir -p $(TEST_GENDIR)
	@scripts/genTestsFromTemplate $(SRCDIR) $< $@

-include $(DEPENDS)

