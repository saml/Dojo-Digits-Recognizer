all:
ifeq ($(threads),)
	@echo "Compiling for default, 6 threads."
	@echo -e "To build for N threads, run:\n\t$$ make threads=N"
	gcc knn.c -pthread -O3 -std=c99 -Wall -pedantic -o knn
else
	@echo "Compiling for $(threads) thread(s)."
	gcc knn.c -DNUM_THREADS=$(threads) -pthread -O3 -std=c99 -Wall -pedantic -o knn
endif
