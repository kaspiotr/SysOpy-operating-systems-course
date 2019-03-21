GCC_FLAGS = -Wall -g

all: program.c
	gcc $(GCC_FLAGS) program.c -o program

tests:
	./program generate dane 500 1
	./program copy dane dane_copy_lib 500 1 lib
	./program copy dane dane_copy_sys 500 1 sys
	./program sort dane_copy_lib 500 1 lib
	./program sort dane_copy_sys 500 1 sys
	./program generate dane 500 4
	./program copy dane dane_copy_lib 500 4 lib
	./program copy dane dane_copy_sys 500 4 sys
	./program sort dane_copy_lib 500 4 lib
	./program sort dane_copy_sys 500 4 sys
	./program generate dane 500 512
	./program copy dane dane_copy_lib 500 512 lib
	./program copy dane dane_copy_sys 500 512 sys
	./program sort dane_copy_lib 500 512 lib
	./program sort dane_copy_sys 500 512 sys
	./program generate dane 500 1024
	./program copy dane dane_copy_lib 500 1024 lib
	./program copy dane dane_copy_sys 500 1024 sys
	./program sort dane_copy_lib 500 1024 lib
	./program sort dane_copy_sys 500 1024 sys
	./program generate dane 500 4096
	./program copy dane dane_copy_lib 500 4096 lib
	./program copy dane dane_copy_sys 500 4096 sys
	./program sort dane_copy_lib 500 4096 lib
	./program sort dane_copy_sys 500 4096 sys
	./program generate dane 500 8192
	./program copy dane dane_copy_lib 500 8192 lib
	./program copy dane dane_copy_sys 500 8192 sys
	./program sort dane_copy_lib 500 8192 lib
	./program sort dane_copy_sys 500 8192 sys
	./program generate dane 1000 1
	./program copy dane dane_copy_lib 1000 1 lib
	./program copy dane dane_copy_sys 1000 1 sys
	./program sort dane_copy_lib 1000 1 lib
	./program sort dane_copy_sys 1000 1 sys
	./program generate dane 1000 4
	./program copy dane dane_copy_lib 1000 4 lib
	./program copy dane dane_copy_sys 1000 4 sys
	./program sort dane_copy_lib 1000 4 lib
	./program sort dane_copy_sys 1000 4 sys
	./program generate dane 1000 512
	./program copy dane dane_copy_lib 1000 512 lib
	./program copy dane dane_copy_sys 1000 512 sys
	./program sort dane_copy_lib 1000 512 lib
	./program sort dane_copy_sys 1000 512 sys
	./program generate dane 1000 1024
	./program copy dane dane_copy_lib 1000 1024 lib
	./program copy dane dane_copy_sys 1000 1024 sys
	./program sort dane_copy_lib 1000 1024 lib
	./program sort dane_copy_sys 1000 1024 sys
	./program generate dane 1000 4096
	./program copy dane dane_copy_lib 1000 4096 lib
	./program copy dane dane_copy_sys 1000 4096 sys
	./program sort dane_copy_lib 1000 4096 lib
	./program sort dane_copy_sys 1000 4096 sys
	./program generate dane 1000 8192
	./program copy dane dane_copy_lib 1000 8192 lib
	./program copy dane dane_copy_sys 1000 8192 sys
	./program sort dane_copy_lib 1000 8192 lib
	./program sort dane_copy_sys 1000 8192 sys

clean:
	rm program dane dane_copy_lib dane_copy_sys wyniki.txt


	
