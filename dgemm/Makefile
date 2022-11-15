
all: mt-dgemm

mt-dgemm: mt-dgemm.c
	g++ -O3 -std=c++11 -ffast-math -mavx2 -fopenmp -DUSE_CBLAS -o mt-dgemm mt-dgemm.c -lopenblas

clean:
	rm mt-dgemm *.o

