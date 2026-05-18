/*
 * timer.c
 * ---
 * Implementa la temporización base del microondas con Timer1.
 *
 * Responsabilidades:
 * - Generar un tick de 1 segundo para el conteo descendente.
 * - Exponer un flag mínimo para desacoplar la ISR de la MEF.
 *
 * Dependencias importantes:
 * - timer.h: interfaz pública.
 * - avr/interrupt: vector de interrupción.
 *
 * Restricción de diseño:
 * - La ISR evita lógica pesada para reducir latencia y jitter.
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

volatile uint8_t flag_un_segundo = 0;

/*
 * Inicializa Timer1 en CTC para generar una base de tiempo de 1 segundo.
 */
void TIMER1_Init(void) {
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = 15624; 
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

/*
 * ISR de Timer1.
 * Se ejecuta cada 1 segundo y solo levanta un flag para mantenerla mínima.
 */
ISR(TIMER1_COMPA_vect) {
    flag_un_segundo = 1; 
}
