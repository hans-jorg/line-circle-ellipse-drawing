

CFLAGS= -g
LDLIBS=

drawing-algorithms: drawing-algorithms.o

clean:
	rm drawing-algorithms *.o

run: drawing-algorithms
	./drawing-algorithms saida.pbm
