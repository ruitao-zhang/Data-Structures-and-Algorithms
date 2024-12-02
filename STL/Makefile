CXX = g++
CXXFLAGS = -Wall -g
STL_SRC = test.cpp
STL_OBJ = $(STL_SRC:.cpp=.o)
REPORT = report.tex
PDF = report.pdf

all: $(STL_OBJ) $(PDF)

$(STL_OBJ): $(STL_SRC) HeapSort.h
	$(CXX) $(CXXFLAGS) -c $(STL_SRC)

main: $(STL_OBJ)
	$(CXX) $(CXXFLAGS) -o main $(STL_OBJ)

$(PDF): $(REPORT)
	xelatex $(REPORT)
	xelatex $(REPORT) 

clean:
	rm -f $(STL_OBJ) main $(PDF) *.log *.aux
