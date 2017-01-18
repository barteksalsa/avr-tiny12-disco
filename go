rm -f leds.o leds.hex leds.elf

avr-gcc -g -O4 -mmcu=at90s2313 -c leds.c &&\
avr-gcc -mmcu=attiny12 -o leds.elf leds.o &&\
avr-objcopy -j .text -O ihex leds.elf leds.hex &&\
avr-objdump -h -d leds.elf

