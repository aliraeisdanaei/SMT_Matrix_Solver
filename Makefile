SHELL = /bin/sh

OBJS =  -o solver.o
CFLAG = -w -g 
CC = gcc
LIBS = -fopenmp
FILES = naive_solver.c optimised_solver.c main.c MatrixMarket/read_mtx.c MatrixMarket/mmio.c verifier.c DynamicList/dynamic_list.c optimised_orderings.c

INPUT_FILE1 = Inputs/mtx_files/torso1_leftside.mtx 
INPUT_B_FILE1 = Inputs/mtx_files/b_for_torso1.mtx
INPUT_FILE2 = Inputs/mtx_files/TSOPF_RS_b678_c2_leftside.mtx
INPUT_B_FILE2 = Inputs/mtx_files/b_for_TSOPF_RS_b678_c2_b.mtx

solver.o: main.c ${FILES}
	${CC} ${LIBS} ${CFLAG} ${OBJS} ${FILES}

clean:
	rm -f *.o

run_naive_1:
	./solver.o naive ${INPUT_FILE1} ${INPUT_B_FILE1}

run_naive_2:
	./solver.o naive ${INPUT_FILE2} ${INPUT_B_FILE1}

run_opt_1:
	./solver.o opt ${INPUT_FILE1} ${INPUT_B_FILE1}

run_opt_2:
	./solver.o opt ${INPUT_FILE2} ${INPUT_B_FILE1}