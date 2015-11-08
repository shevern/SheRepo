prg:prg.o prgm.o
	gcc  -o prg prg.o -lm -lpthread
	gcc  -o prgm prgm.o

prg.o: prg.c prgm.c
	gcc -c prg.c
	gcc -c prgm.c

clean:
	rm prg.o prg prgm.o prgm




