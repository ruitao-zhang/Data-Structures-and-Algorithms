LATEX = Xelatex
TEXFILE = report
PDFVIEWER = xdg-open  

CPP_COMPILER = g++
CPP_SRC = Programming Assignments in 1.8.2/Solver.cpp
CPP_OUTPUT = Solver

all: pdf

pdf: $(TEXFILE).tex
	$(LATEX) $(TEXFILE).tex
	$(LATEX) $(TEXFILE).tex  

view: pdf
	$(PDFVIEWER) $(TEXFILE).pdf

report: pdf

$(CPP_OUTPUT): $(CPP_SRC)
	$(CPP_COMPILER) $(CPP_SRC) -o $(CPP_OUTPUT) $(CPP_FLAGS)

run: $(CPP_OUTPUT)
	./$(CPP_OUTPUT)

all: $(CPP_OUTPUT) report

