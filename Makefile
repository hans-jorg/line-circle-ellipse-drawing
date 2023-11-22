

CFLAGS= -g
LDLIBS=

drawing-test: bresenham.o  main.o  mark.o  midpoint.o  screen.o
	$(CC) $(CFLAGS) -o $@  $^ $(LDLIBS)

clean:
	rm -f drawing-test *.o *.pgm

run: drawing-test
	./drawing-test
