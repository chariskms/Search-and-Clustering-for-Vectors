CXX = g++
OBJ = lsh
all: $(OBJ)
$(OBJ): %: %.cpp
	$(CXX) -o $@ dataset.cpp $<
clean:
	rm -f lsh *.o
