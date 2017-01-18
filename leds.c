#include <avr/io.h>
 
register unsigned char i asm("r3");
register unsigned char y asm("r4");

void delayLoop(void)
{
    for (y = 2; y > 0; --y)
    {
        asm volatile ("nop"::);
    }
}

int main(void)
{
    asm volatile("eor	r1, r1"::);
    DDRB = _BV(PB3) | _BV(PB4);                       // initialize port C
    for (;;)
    {
        PORTB = _BV(3) | _BV(4);            // PC0 = High = Vcc
        delayLoop();
        PORTB = _BV(3) | _BV(4);            // PC0 = Low = 0v
        delayLoop();
    }
}

