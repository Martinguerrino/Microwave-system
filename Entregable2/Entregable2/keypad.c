#include <stdint.h>
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include "keypad.h"

const uint8_t teclado[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
void KEYPAD_Init(){
	DDRB |= (1<<PB4) | (1<<PB3) | (1<<PB0);
	DDRD |= (1<<PD7);
	DDRD &= ~((1<<PD3)|(1<<PD5)|(1<<PD4)|(1<<PD2));
	PORTD |= (1<<PD3)|(1<<PD5)|(1<<PD4)|(1<<PD2);
}

uint8_t KeypadUpdate(void)
{
	for(uint8_t f = 0; f < 4; f++) {
		PORTB |= (1<<PB4) | (1<<PB3) | (1<<PB0);
		PORTD |= (1<<PD7);

		switch(f)
		{
			case 0: PORTB &= ~(1<<PB4); break; 
			case 1: PORTB &= ~(1<<PB3); break; 
			case 2: PORTB &= ~(1<<PB0); break; 
			case 3: PORTD &= ~(1<<PD7); break; 
		}
		
		_delay_us(10);
		uint8_t col = 0xFF;

		if(!(PIND & (1<<PD3))) col = 0;
		else if(!(PIND & (1<<PD5))) col = 1;
		else if(!(PIND & (1<<PD4))) col = 2;
		else if(!(PIND & (1<<PD2))) col = 3;

		if(col != 0xFF) return teclado[f][col];
	}
	return 0xFF;
}

uint8_t KEYPAD_Scan(uint8_t *pkey)
{
	static uint8_t Old_key = 0xFF;
	static uint8_t Last_valid_key = 0xFF;

	uint8_t Key = KeypadUpdate();

	if(Key == 0xFF){
		Old_key = 0xFF;
		Last_valid_key = 0xFF;
		return 0;
	}

	if(Key == Old_key){
		if(Key != Last_valid_key){
			*pkey = Key;
			Last_valid_key = Key;
			return 1;
		}
	}

	Old_key = Key;
	return 0;
}