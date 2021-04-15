#.DEFAULT_GOAL := execfile
all: main.o calc_sum.o execfile

main.o: main.c calc_sum.h
	sleep 10
	gcc -c main.c

calc_sum.o: calc_sum.c calc_sum.h
	sleep 10
	gcc -c calc_sum.c

execfile: main.o calc_sum.o
	sleep 10
	gcc -o execfile main.o calc_sum.o

clean:
	echo "cleaning"
	rm execfile main.o calc_sum.o

.PHONY: all
