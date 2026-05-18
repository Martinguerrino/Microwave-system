/*
 * keypad.h
 * ---
 * Declara la interfaz del teclado matricial del microondas.
 *
 * Responsabilidades:
 * - Inicializar el hardware del teclado.
 * - Escanear pulsaciones y filtrar rebote.
 * - Exponer una API simple orientada a eventos.
 *
 * Dependencias importantes:
 * - avr/io: configuración de pines.
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <avr/io.h>

/*
 * Configura filas y columnas del teclado matricial.
 */
void KEYPAD_Init(void);
/*
 * Realiza una lectura directa del teclado sin filtrado de pulsación única.
 */
uint8_t KeypadUpdate(void);
/*
 * Filtra pulsaciones sostenidas y entrega una tecla válida por evento.
 */
uint8_t KEYPAD_Scan(uint8_t *pkey);

#endif