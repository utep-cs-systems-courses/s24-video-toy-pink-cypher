# Makefile configuration
CPU = msp430g2553
CFLAGS = -mmcu=${CPU} -Os -I../h
LDFLAGS = -L../lib -L/opt/ti/msp430_gcc/include/

# Switch the compiler (for the internal make rules)
CC = msp430-elf-gcc

# Define the objects needed
OBJECTS = wakedemo.o buzzer.o wdt_handler.o


install: libLcd.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

# Define the target
all: wakedemo.elf

# Rule to create the ELF file
wakedemo.elf: ${OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

# Rule to load the ELF file
load: wakedemo.elf
	msp430loader.sh $^

# Rule to compile each source file
%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

# Rule to clean up generated files
clean:
	rm -f *.o *.elf
	rm -f README.md~ wakedemo.c~ Makefile~ buzzer.h~ buzzer.c~

wdt_handler.o: wdt_handler.s
switches.o: switches.s
switches_c.o: switches_c.c switches.h
buzzer.o: buzzer.c buzzer.h
