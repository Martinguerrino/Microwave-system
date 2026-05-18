#include <stdint.h>
#include "display.h"

// Imprime el arreglo de dígitos en formato MM:SS
void Mostrar_Tiempo_LCD(void) {
    LCDGotoXY(0, 0);
    LCDsendChar(digitos[0] + '0'); 
    LCDsendChar(digitos[1] + '0'); 
    LCDsendChar(':'); 
    LCDsendChar(digitos[2] + '0'); 
    LCDsendChar(digitos[3] + '0'); 
}