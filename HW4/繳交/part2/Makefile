all:
	mpicxx -fPIC -O3 -O2 -mavx2 -mavx -mfma -pedantic main.cc functions.cc -o matmul -ffast-math -funsafe-math-optimizations -floop-parallelize-all -fprefetch-loop-arrays -fno-tree-vectorize -w
