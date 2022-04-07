obj-m += driverdrlr.o

all:
	make -C /lib/modules/5.16.0-kali6-amd64/build/ M=$(shell pwd) modules

clean:
	make -C /lib/modules/5.16.0-kali6-amd64/build/ M=$(shell pwd) clean