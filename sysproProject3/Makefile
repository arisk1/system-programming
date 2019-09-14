OBJS1 = MirrorInitiator.o
OBJS2 = msFunctions.o MirrorServer.o
OBJS3 = csFunctions.o ContentServer.o
SOURCE = MirrorInitiator.c msFunctions.c MirrorServer.c  csFunctions.c ContentServer.c 
HEADER = msFunctions.h csFunctions.h
OUT1 = mirrorInitiator
OUT2 = mirrorServer
OUT3 = contentServer
CC = gcc
FLAGS = -g -c


all: $(OUT1) $(OUT2) $(OUT3)

$(OUT1): $(OBJS1)
	$(CC) -g $(OBJS1) -o $@

$(OUT2): $(OBJS2)
	$(CC) -g $(OBJS2) -o $@ -lpthread

$(OUT3): $(OBJS3)
	$(CC) -g $(OBJS3) -o $@

MirrorInitiator.o: MirrorInitiator.c
	$(CC) $(FLAGS) MirrorInitiator.c

msFunctions.o: msFunctions.c
	$(CC) $(FLAGS) msFunctions.c 
	
MirrorServer.o: MirrorServer.c
	$(CC) $(FLAGS) MirrorServer.c -lpthread

csFunctions.o: csFunctions.c	
	$(CC) $(FLAGS) csFunctions.c
	
ContentServer.o: ContentServer.c	
	$(CC) $(FLAGS) ContentServer.c

clean : 
	rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(OUT1) $(OUT2) $(OUT3)
 
