/*
 * timer.h
 * ---
 * Declara la base temporal del sistema de control del microondas.
 *
 * Responsabilidades:
 * - Inicializar Timer1 en modo CTC.
 * - Inicializar Timer0 para un flag periódico de 100 ms.
 * - Publicar el flag periódico de 1 segundo consumido por la MEF.
 *
 * Dependencias importantes:
 * - avr/interrupt: manejo del vector de interrupción asociado.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * Configura Timer1 para generar una interrupción periódica de 1 segundo.
 */
void TIMER1_Init(void);

/*
 * Configura Timer0 para generar un flag periódico de 100 ms.
 */
void TIMER0_Init(void);

/*
 * Flag temporal levantado por la ISR y consumido por la aplicación principal.
 */
extern volatile uint8_t flag_un_segundo;

/*
 * Flag temporal levantado cada 100 ms y consumido por el bucle principal.
 */
extern volatile uint8_t flag_cien_milisegundos;

#endif