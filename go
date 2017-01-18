rm -f leds.o leds.hex leds.elf

avr-gcc -ffunction-sections -fdata-sections -g -O4 -mmcu=at90s2313 -c leds.c &&\
avr-gcc -Wl,--gc-sections -mmcu=attiny12 -o leds.elf leds.o &&\
avr-objcopy -j .text -O ihex leds.elf leds.hex &&\
avr-objdump -h -d leds.elf > leds.objdump

#sudo avrdude -v -p t12 -c usbasp -Uflash:w:leds.hex -Ereset,vcc
