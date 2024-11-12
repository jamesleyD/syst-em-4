# Options de compilation
WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wstrict-prototypes -Wno-array-bounds

CFLAGS := -Os -DF_CPU=16000000UL -mmcu=atmega328p $(WARNINGS) -I./ring_buffer -I./include
LIBRARY := -L./ring_buffer -lring_buffer

upload: program.hex
	avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:$^

program.hex: program.elf
	avr-objcopy -O ihex $^ $@

program.elf: src/part4/main.c src/uart.c
	avr-gcc $(CFLAGS) -o $@ $^ $(LIBRARY)

clean:
	rm -f *.elf *.hex
