#include <avr/io.h>
#include "actuators.h"

void initActuators(void)
{
	DDRB |= (1 << PB5);
	DDRC |= (1 << PC4) | (1 << PC5);
	PORTB &= ~(1 << PB5);
	PORTC &= ~((1 << PC4) | (1 << PC5));
}
