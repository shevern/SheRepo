
prg2lb: prog2lb.so
	gcc -shared -o prog2lb.so

prog2lb.so: prog2lb.cpp prog2.h
	gcc -c prog2lb.cpp



prg1: prg1.o
	cc -o prg1 prg1.o
prg1.o: prg1.c
	cc -c prg1.c
clean:
	rm prg1 prg1.o
