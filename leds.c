#include <avr/io.h>
 
register unsigned char i asm("r3");
register unsigned char y asm("r4");
register unsigned char z asm("r5");

void delayLoop(void)
{
    for (z = 100; z > 0; --z)
    {
        for (y = 255; y > 0; --y)
        {
            asm volatile ("nop"::);
        }
    }
}

void setupTimer(void)
{

}

void calibrateOscillator(void)
{
    _SFR_IO8(0x31) = 0x27; /* 0x27 read from avrdude -Ucalibrate */
}

int main(void)
{
    /* hack to provide minimum C runtime */
    asm volatile("eor	r1, r1"::);

    calibrateOscillator();
    setupTimer();


    DDRB = _BV(PB3) | _BV(PB4);                       // initialize port C
    for (;;)
    {
        PORTB = _BV(3) | _BV(4);            // PC0 = High = Vcc
        delayLoop();
        PORTB = 0;            // PC0 = Low = 0v
        delayLoop();
    }
}

