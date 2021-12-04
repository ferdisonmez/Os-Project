CC     = gcc
CFLAGS = -O2 -Wall -pedantic
LIBS   = -lm -lnsl 

Main:Main.o
	$(CC) $(CFLAGS) $(LIBS) -o Main Main.o

Main.o:Main.c
	$(CC) $(CFLAGS) -c Main.c

clean:
	rm -f Main *.o
