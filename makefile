CC        := gcc
CFLAGS    := -Wall -Wextra -std=c11 -Iinclude

SRCDIR    := src
OBJDIR    := obj
BINDIR    := bin

SOURCES   := $(shell find $(SRCDIR) -name '*.c')
OBJECTS   := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

TARGET        := $(BINDIR)/app
DEBUG_TARGET  := $(BINDIR)/app_debug

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*

run: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Opsin1_colorblindness_gene.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_banana: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/English_alphabet.txt inputs/genes/banana.txt; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_mississippi: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/English_alphabet.txt inputs/genes/mississippi.txt; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

debug: CFLAGS += -g -O0
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $(DEBUG_TARGET)