prg: prg.o
	gcc -lpthread -o prgc prgc.o -lpthread
	gcc -lpthread -o prgs prgs.o -lpthread

prg.o: prgc.c prgs.c
	gcc -c prgc.c
	gcc -c prgs.c

clean:
	rm prgc.o prgs.o prgc prgs



