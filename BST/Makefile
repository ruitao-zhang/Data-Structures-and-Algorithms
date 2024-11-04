CXX = g++
CXXFLAGS = -Wall -g
BST_SRC = main.cpp
BST_OBJ = $(BST_SRC:.cpp=.o)
REPORT = report.tex
PDF = report.pdf

all: $(BST_OBJ) $(PDF)

$(BST_OBJ): $(BST_SRC) BinarySearchTree.h
	$(CXX) $(CXXFLAGS) -c $(BST_SRC)

main: $(BST_OBJ)
	$(CXX) $(CXXFLAGS) -o main $(BST_OBJ)

$(PDF): $(REPORT)
	xelatex $(REPORT)
	xelatex $(REPORT) # 可能需要运行两次以确保引用正确

clean:
	rm -f $(BST_OBJ) main $(PDF) *.log *.aux

