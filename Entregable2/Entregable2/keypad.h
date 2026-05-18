#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <avr/io.h>

void KEYPAD_Init(void);
uint8_t KeypadUpdate(void);
uint8_t KEYPAD_Scan(uint8_t *pkey);

#endif