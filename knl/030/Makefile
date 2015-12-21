obj-m += prg.o
obj-m += prg2.o
obj-m += prgfs.o
obj-m += prgcd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
