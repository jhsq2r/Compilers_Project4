C = gcc -g3
CFLAGS = -g3

all: Project4

Project4: main.o scanner.o parser.o statSem.o
	$(CC) $(CFLAGS) -o Project4 main.o scanner.o parser.o statSem.o

main.o: main.c token.h scanner.h parser.h statSem.h
	$(CC) $(CFLAGS) -c main.c

scanner.o: scanner.c token.h scanner.h
	$(CC) $(CFLAGS) -c scanner.c

parser.o: parser.c token.h parser.h
	$(CC) $(CFLAGS) -c parser.c

statSem.o: statSem.c token.h statSem.h parser.h
	$(CC) $(CFLAGS) -c statSem.c

clean:
	rm -f Project4 *.o
	rm -f filteredFile.txt
	rm -f tempfile.txt
	rm -f Project4.exe.stackdump
