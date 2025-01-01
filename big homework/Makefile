SRC_DIR = src
DOC_DIR = doc
FIG_DIR = figure

CXX = g++
CXXFLAGS = -Wall -g

EXEC = program

TEX_FILES = $(DOC_DIR)/report.tex $(DOC_DIR)/design.tex
PDF_FILES = $(TEX_FILES:$(DOC_DIR)/%.tex=$(DOC_DIR)/%.pdf)

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(SRC_DIR)/%.o)

all: $(EXEC) $(PDF_FILES)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@

$(DOC_DIR)/%.pdf: $(DOC_DIR)/%.tex
	cd $(DOC_DIR) && xelatex $<

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(SRC_DIR)/*.o $(EXEC) $(PDF_FILES)
	cd $(DOC_DIR) && rm -f *.aux *.log *.out



.PHONY: all clean run help
