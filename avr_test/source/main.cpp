#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRD = 0x20;
    while(true)
    {
        PORTD |= 0x20;
        _delay_ms(500);
        PORTD &= ~0x20;
        _delay_ms(500);
    }
}