CC = gcc
CFLAGS = -Wall -g
OPT = -Os # set here one of options: -O, -O1, -O2, -O3, -O0, -Os

# tworzenie programu do testowania find_test_static wykorzystujacego biblioteke statyczna find
static: find_test.o find.o
	$(CC) $(CFLAGS) $(OPT) -o find_test_static find_test.o find.o

# tworzenie object file programu find_test do testowania biblioteki statycznej
find_test.o: find_test.c find.h
	$(CC) $(CFLAGS) $(OPT) -c find_test.c

# tworzenie object file biblioteki find
find.o: find.c find.h
	$(CC) $(CFLAGS) $(OPT) -c find.c

# tworzenie programu do testowania find_test_shared wykorzystujacego biblioteke dynamiczna find
shared: find.c find.h
	$(CC) $(CFLAGS) $(OPT) -c -fPIC find.c
	$(CC) $(CFLAGS) $(OPT) -shared -fPIC -o libfind.so find.o # tworzenie dynamicznej bibilioteki libfind.so na podstawie plikow object find.o
	$(CC) $(CFLAGS) $(OPT) -c find_test.c # utworzenie pliku find_test.o typu object
	$(CC) $(CFLAGS) $(OPT) -o find_test_shared find_test.o -L. -lfind -Wl,-rpath,.

# tworzenie programu do testowania find_test_dll wykorzystujacego biblioteke ladowana dynamicznie
dll: find.c find_test.c find.h
	$(CC) $(CFLAGS) $(OPT) -c -fPIC find.c -o find.o
	$(CC) $(CFLAGS) $(OPT) -shared -o libfind.so find.o
	$(CC) $(CFLAGS) $(OPT) find_test_with_dll.c -o find_test_dll -ldl

# uruchamianie testow
test_static: static
	./find_test_static create_table 300 search_directory / "*" tempfile.txt save_temp_file_to_block tempfile.txt remove_block 0 repeat_save_to_block_and_remove tempfile.txt 1000000

test_shared: shared
	./find_test_shared create_table 300 search_directory / "*" tempfile.txt save_temp_file_to_block tempfile.txt remove_block 0 repeat_save_to_block_and_remove tempfile.txt 1000000

test_dll: dll
	./find_test_dll create_table 300 search_directory / "*" tempfile.txt save_temp_file_to_block tempfile.txt remove_block 0 repeat_save_to_block_and_remove tempfile.txt 1000000

# usuwanie plikow object oraz programow do testowania biblioteki statycznej, shared i dynamicznej find
clean:
	rm *.o *.so find_test_dll find_test_static find_test_shared raport2.txt raport3a.txt raport3b.txt
