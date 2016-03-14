obj-m := printer.o

PWD := $(shell pwd)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
