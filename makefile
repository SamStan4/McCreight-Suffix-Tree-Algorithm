CXX       := g++
CXXFLAGS  := -Wall -Wextra -std=c++17 -Iinclude

SRCDIR    := src
OBJDIR    := obj
BINDIR    := bin

SOURCES   := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS   := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

TARGET        := $(BINDIR)/app
DEBUG_TARGET  := $(BINDIR)/app_debug

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*

run_s1: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/English_alphabet.txt inputs/genes/banana.txt; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_s2: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/English_alphabet.txt inputs/genes/mississippi.txt; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_opsin_human: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Opsin1_colorblindness_gene.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_brca2_human: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Human-BRCA2-cds.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

# Tomato chloroplast genome
run_slyco: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Slyco.fas; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

# yeast chromosome 12
run_chr12: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/chr12.fas; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

# Covid genomes
run_covid_china: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Covid_Wuhan.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_covid_usa: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Covid_USA-CA4.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_covid_aus: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Covid_Australia.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_covid_india: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Covid_India.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

run_covid_braz: $(TARGET)
	@if [ -f $(TARGET) ]; then \
		./$(TARGET) inputs/alphabets/gene_alphabet.txt inputs/genes/Covid_Brazil.fasta; \
	else \
		echo "Error: Executable $(TARGET) not found. Please build it first."; \
		exit 1; \
	fi

debug: CXXFLAGS += -g -O0
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $(DEBUG_TARGET)
