all: init getty sh

init: init.c
	gcc init.c -o init

getty: getty.c
	gcc getty.c -o getty

sh: sh.c
	gcc sh.c -o sh

clean:
	rm -f init getty sh
