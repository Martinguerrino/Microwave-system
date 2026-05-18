/*
 * actuators.c
 * ---
 * Implementa la capa de salida física del microondas.
 *
 * Responsabilidades:
 * - Configurar y conmutar los pines de actuadores.
 * - Mantener aislada la manipulación de hardware directo.
 * - Garantizar un estado inicial seguro al arrancar.
 *
 * Dependencias importantes:
 * - actuators.h: contrato público del módulo.
 * - avr/io: acceso a DDRx y PORTx.
 *
 * Mapeo de hardware:
 * - PB5 -> Magnetrón.
 * - PC4 -> Luz interior.
 * - PC5 -> Buzzer/alarma.
 */

#include <avr/io.h>
#include "actuators.h"

void initActuators(void)
{
	DDRB |= (1 << PB5);
	DDRC |= (1 << PC4) | (1 << PC5);
	Magnetron_Off();
	InteriorLight_Off();
	Alarm_off();
}

void Magnetron_On(void)
{
	PORTB |= (1 << PB5);
}

void Magnetron_Off(void)
{
	PORTB &= ~(1 << PB5);
}

void InteriorLight_On(void)
{
	PORTC |= (1 << PC4);
}

void InteriorLight_Off(void)
{
	PORTC &= ~(1 << PC4);
}

void Alarm_On(void)
{
	PORTC |= (1 << PC5);
}

void Alarm_off(void)
{
	PORTC &= ~(1 << PC5);
}
