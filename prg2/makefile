prg2mn:prg2mn.c libprg2lb.so
	gcc -L /home/she/SheRepo/prg2/ -Wall prg2mn.c -o prg2mn -l prg2lb

libprg2lb.so:prg2lb.o
	gcc -shared -o libprg2lb.so  prg2lb.o

prg2lb.o:
	gcc -c -Wall -Werror  -fPIC  prg2lb.c



