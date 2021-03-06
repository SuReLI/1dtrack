CCC=g++
CCFLAGS=-std=c++11 -Wall -Wextra -I./src -O2 -g
LDFLAGS=-lm -lconfig++
EXEC=exe
NBSIM=1

all : clean compile run

clean :
	rm -f ${EXEC}

compile : main.cpp
	${CCC} ${CCFLAGS} main.cpp -o ${EXEC} ${LDFLAGS}

run :
	./${EXEC} ${NBSIM}

hist :
	python3 plot/histogram.py

graph :
	python3 plot/graph.py

