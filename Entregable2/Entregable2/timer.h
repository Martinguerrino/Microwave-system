/*
 * timer.h
 * ---
 * Declara la base temporal del sistema de control del microondas.
 *
 * Responsabilidades:
 * - Inicializar Timer1 en modo CTC.
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
 * Flag temporal levantado por la ISR y consumido por la aplicación principal.
 */
extern volatile uint8_t flag_un_segundo;

#endif