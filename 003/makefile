prg3:prg3.o libprg2lb.so
	gcc -o prg3 prg3.o -ldl

prg3.o: prg3.c
	gcc -c prg3.c

libprg2lb.so: prg2lb.o
	gcc -shared -o libprg2lb.so  prg2lb.o

prg2lb.o:
	gcc -c -Wall -Werror  -fPIC  prg2lb.c





