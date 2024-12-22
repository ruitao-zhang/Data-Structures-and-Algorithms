CXX = g++
CXXFLAGS = -Wall -g
Calculator_SRC = main.cpp
Calculator_OBJ = $(Calculator_SRC:.cpp=.o)
REPORT = report.tex
PDF = report.pdf

all: $(Calculator_OBJ) $(PDF)

$(Calculator_OBJ): $(Calculator_SRC) expression_evaluator.h
	$(CXX) $(CXXFLAGS) -c $(Calculator_SRC)

main: $(Calculator_OBJ)
	$(CXX) $(CXXFLAGS) -o main $(Calculator_OBJ)

$(PDF): $(REPORT)
	xelatex $(REPORT)
	xelatex $(REPORT) 

clean:
	rm -f $(Calculator_OBJ) main $(PDF) *.log *.aux
