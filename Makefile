CC=msp430-elf-gcc
CFLAGS=-O0 -Wall -g -mmcu=msp430g2452 -I$(MSP430_TI)/include -L$(MSP430_TI)/include

OBJS=iris.o usi_i2c.o tsl2561.o digenc.o oled.o


all: $(OBJS)
	$(CC) $(CFLAGS) -o iris.elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr iris.elf $(OBJS) *~
