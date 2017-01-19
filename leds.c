#include <avr/io.h>
#include <avr/interrupt.h>

register unsigned char pwmFastCount asm("r6");
register unsigned char savedR24 asm("r7");
register unsigned char savedR25 asm("r8");
register unsigned char pwmFill asm("r9");
register unsigned char counter asm("r10");
register unsigned char counter2 asm("r11");


/*
 *  PINS in PORTB setup
 */
#define PWMOUTPIN  3
#define DEBUGPIN   4

/*
 *  Using ACSR to return flags from Timer Interrupt.
 *  Register variables are non-volatile by default, so hacking is needed
 */

#ifdef ACSR
#undef ACSR
#endif
#define ACSR    _SFR_IO8(0x08)

void clrFlagTimerTick(void)
{
    ACSR &= ~_BV(0);
}

void setFlagTimerTick(void)
{
    ACSR |= _BV(0);
}

uint8_t getFlagTimerTick(void)
{
    return (ACSR & _BV(0));
}




/* timer interrupt handler */
ISR(_VECTOR(3), ISR_NAKED)
{
    /* save r24, r25 */
    asm("mov r7, r24"::);
    asm("mov r8, r25"::);

    /* rewind timer - must be first */
    _SFR_IO8(0x32) = 256 - 117; /* TCNT0: need to count 25 ticks */

    /* rest */
    ++pwmFastCount;
    if ((pwmFastCount & 32) == 32)
    {
        pwmFastCount = 0;
        PORTB |= _BV(PWMOUTPIN);  /* enable PWM pin here */
        setFlagTimerTick(); /* 300 Hz flag */
    }

    if (pwmFastCount >= pwmFill)
    {
        PORTB &= ~_BV(PWMOUTPIN); /* disable PWM pin when counter */
    }

    //PORTB ^= _BV(DEBUGPIN);
    /* restore r24, r25 and return */
    asm("mov r24, r7"::);
    asm("mov r25, r8"::);
    reti();
}



void setupTimer(void)
{
    _SFR_IO8(0x33) = 1; /* TCCR0 set timer to CLK/1 */
    TIMSK = _BV(TOIE0);
}


void calibrateOscillator(void)
{
    _SFR_IO8(0x31) = 0x27; /* OSCCAL 0x27 read from avrdude -Ucalibrate */
}

void setupPwm(void)
{
    pwmFill = 2;
}

void setPwm(uint8_t newPwmFill)
{
    pwmFill = newPwmFill;
}

uint8_t getPwm(void)
{
    return pwmFill;
}

void incPwm(void)
{
    if (pwmFill < 32)
    {
        pwmFill = pwmFill + 1;
    }
    else
    {
        pwmFill = 0;
    }
}


int main(void)
{
    /* hack to provide minimum C runtime */
    asm volatile("eor	r1, r1"::);
    DDRB = _BV(PB3) | _BV(PB4);
    calibrateOscillator();
    setupTimer();
    setupPwm();
    MCUCR |= _BV(SE);
    sei();

    counter = 0;
    for (;;)
    {
        if (getFlagTimerTick())
        {
            cli();
            clrFlagTimerTick();
            sei();
            ++counter;
        }

        if (counter > 20)
        {
            incPwm();
            counter = 0;
        }
        asm("sleep"::);
    }
}

