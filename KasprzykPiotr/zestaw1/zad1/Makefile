CC = gcc
CFLAGS = -Wall

# tworzenie programu do testowania find_test_static wykorzystujacego biblioteke statyczna find
static: find_test.o find.o
	$(CC) $(CFLAGS) -o find_test_static find_test.o find.o

# tworzenie object file programu find_test do testowania biblioteki statycznej
find_test.o: find_test.c find.h
	$(CC) $(CFLAGS) -c find_test.c

# tworzenie object file biblioteki find
find.o: find.c find.h
	$(CC) $(CFLAGS) -c find.c

# tworzenie programu do testowania find_test_shared wykorzystujacego biblioteke dynamiczna find
shared: find.c find.h
	$(CC) -c -fPIC find.c
	$(CC) -shared -fPIC -o libfind.so find.o # tworzenie dynamicznej bibilioteki libfind.so na podstawie plikow object find.o
	$(CC) -c find_test.c # utworzenie pliku find_test.o typu object
	$(CC) -o find_test_shared find_test.o -L. -lfind -Wl,-rpath,.

# tworzenie programu do testowania find_test_dll wykorzystujacego biblioteke ladowana dynamicznie
dll: find.c find_test.c find.h
	$(CC) -c -fPIC find.c -o find.o
	$(CC) -shared -o libfind.so find.o
	$(CC) find_test_with_dll.c -o find_test_dll -ldl

# usuwanie plikow object oraz programow do testowania biblioteki statycznej i dynamicznej find
clean:
	rm *.o *.so find_test_dll find_test_static find_test_shared 
