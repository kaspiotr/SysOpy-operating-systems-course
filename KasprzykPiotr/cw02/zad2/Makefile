GCC_FLAGS = -Wall

main: program2.c
	gcc $(GCC_FLAGS) program2.c -o program2

clean:
	rm program2

test1:
	./program2 /etc/ '>' 'mar 19 2019' search_directory
test2:
	./program2 /tmp/ '>' 'mar 19 2019' search_directory
test3:
	./program2 /etc/ '>' 'mar 19 2019' nftw
test4:
	./program2 /tmp/ '>' 'mar 19 2019' nftw

