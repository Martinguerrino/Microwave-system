/*
 * display.c
 * ---
 * Implementa la presentación del tiempo de cocción en el LCD.
 *
 * Responsabilidades:
 * - Traducir el estado visible a formato MM:SS.
 * - Mantener desacoplada la lógica de formato respecto de la MEF.
 *
 * Dependencias importantes:
 * - lcd: acceso al dispositivo de visualización.
 * - display.h: contrato público del módulo.
 */

#include <stdint.h>
#include "display.h"

/*
 * Presenta el tiempo con formato fijo para evitar artefactos visuales.
 */
void Mostrar_Tiempo_LCD(void) {
    LCDGotoXY(0, 0);
    LCDsendChar(digitos[0] + '0'); 
    LCDsendChar(digitos[1] + '0'); 
    LCDsendChar(':'); 
    LCDsendChar(digitos[2] + '0'); 
    LCDsendChar(digitos[3] + '0'); 
}