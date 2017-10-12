CCC=g++
CCFLAGS=-std=c++11 -Wall -Wextra -I./src -O2 -g
EXEC=exe

all : clean compile run

clean :
	rm -f ${EXEC}

compile : main.cpp
	${CCC} ${CCFLAGS} main.cpp -o ${EXEC}

run :
	./${EXEC}

plot_hist :
	python3 plot/histogram.py

