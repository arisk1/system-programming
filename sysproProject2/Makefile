OBJS1 = console.o
OBJS2 = coord.o functions.o
SOURCE = console.c coord.c functions.c
HEADER = functions.h
OUT1 = console
OUT2 = coord
CC = gcc
FLAGS = -std=gnu99 -g -c

all: $(OUT1) $(OUT2)

$(OUT1): $(OBJS1)
	$(CC) -g $(OBJS1) -o $@

$(OUT2): $(OBJS2)
	$(CC) -g $(OBJS2) -o $@

console.o: console.c
	$(CC) $(FLAGS) console.c

coord.o: coord.c
	$(CC) $(FLAGS) coord.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

clean : 
	rm -f $(OBJS1) $(OBJS2) $(OUT1) $(OUT2)
 
