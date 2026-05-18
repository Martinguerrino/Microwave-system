/*
 * timer.c
 * ---
 * Implementa la temporización base del microondas con Timer1.
 *
 * Responsabilidades:
 * - Generar un tick de 1 segundo para el conteo descendente.
 * - Generar un tick de 100 ms para el ciclo principal.
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
volatile uint8_t flag_cien_milisegundos = 0;

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
 * Inicializa Timer0 en CTC para generar una base de tiempo de 100 ms.
 */
void TIMER0_Init(void) {
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
    sei();
}

/*
 * ISR de Timer1.
 * Se ejecuta cada 1 segundo y solo levanta un flag para mantenerla mínima.
 */
ISR(TIMER1_COMPA_vect) {
    flag_un_segundo = 1; 
}

/*
 * ISR de Timer0.
 * Se ejecuta cada 1 ms y levanta el flag de 100 ms por software.
 */
ISR(TIMER0_COMPA_vect) {
    static uint8_t contador_100ms = 0;

    contador_100ms++;
    if (contador_100ms >= 100) {
        contador_100ms = 0;
        flag_cien_milisegundos = 1;
    }
}
