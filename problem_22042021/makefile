threaded: sum_thread.o calc_sum.o
	gcc -o sum_thread sum_thread.o calc_sum.o -lpthread

libs: calc_sum.c calc_sum.h
	gcc -c calc_sum.c

threads: sum_thread.c calc_sum.h
	gcc -c sum_thread.c

clean:
	rm sum_thread calc_sum.o sum_thread.o

.PHONY: all clean
