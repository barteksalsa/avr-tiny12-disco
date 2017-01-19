rm -f leds.o leds.hex leds.elf

echo "C compiler" &&\
avr-gcc -ffunction-sections -fdata-sections -g -O4 -mmcu=at90s2313 -c leds.c &&\
echo "C linker" &&\
avr-gcc -Wl,--gc-sections -mmcu=attiny12 -o leds.elf leds.o &&\
echo "C dumps" &&\
avr-objcopy -j .text -O ihex leds.elf leds.hex &&\
avr-objdump -h -d leds.elf > leds.objdump &&\
echo "C done"

#sudo avrdude -v -p t12 -c usbasp -Uflash:w:leds.hex -Ereset,vcc
