prg1: prg1.o
	cc -o prg1 prg1.o
prg1.o: prg1.c
	cc -c prg1.c
clean:
	rm prg1 prg1.o
