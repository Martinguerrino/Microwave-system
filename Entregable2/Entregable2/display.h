/*
 * display.h
 * ---
 * Declara la interfaz de presentación del tiempo en el LCD.
 *
 * Responsabilidades:
 * - Ofrecer una API mínima para mostrar el tiempo del microondas.
 * - Ocultar el detalle de formateo desde la lógica de aplicación.
 *
 * Dependencias importantes:
 * - lcd: controlador de pantalla importado.
 * - microwave: fuente del arreglo de dígitos visibles.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "lcd.h"

extern uint8_t digitos[4];

/*
 * Muestra el tiempo actual en formato MM:SS sobre la primera línea del LCD.
 */
void Mostrar_Tiempo_LCD(void);

#endif