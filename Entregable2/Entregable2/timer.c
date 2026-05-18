#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void TIMER1_Init(void) {
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = 15624; 
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect) {
    flag_un_segundo = 1; 
}
