CC=gcc
CFLAGS=-std=gnu99 -I.
DEPS = fuctions.h dio.h
OBJ = main.o fuctions.o dio.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

werhauz: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o werhauz
