all: init getty sh

init: init.c
	gcc r_init.c -o init

getty: getty.c
	gcc r_getty.c -o getty

sh: sh.c
	gcc r_sh.c -o sh

clean:
	rm -f init getty sh