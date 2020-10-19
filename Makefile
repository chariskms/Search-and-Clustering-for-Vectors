CXX = g++
all: lsh cube
lsh:
	$(CXX) -o lsh lsh.cpp dataset.cpp hash.cpp manhattan.cpp algorithms.cpp
cube:
	$(CXX) -o cube cube.cpp dataset.cpp
run_lsh:
	./lsh -d train-images.idx3-ubyte -R 1.0 -q t10k-images.idx3-ubyte -k 4 -L 5 -o fileo -N 1
run_cube:
	./cube -d train-images.idx3-ubyte -q t10k-images.idx3-ubyte -k 3 -M 10 -probes 2 -ο output -Ν 1 -R 1.0
val_lsh:
	valgrind --leak-check=yes --track-origins=yes ./lsh -d train-images.idx3-ubyte -R 1.0 -q t10k-images.idx3-ubyte -k 4 -L 5 -o fileo -N 1
val_cube:
	valgrind --leak-check=yes --track-origins=yes ./cube -d train-images.idx3-ubyte -q t10k-images.idx3-ubyte -k 3 -M 10 -probes 2 -ο output -Ν 1 -R 1.0
clean:
	rm -f lsh cube *.o
