/*
 * main.c
 * ---
 * Punto de arranque del sistema embebido del microondas.
 *
 * Responsabilidades:
 * - Inicializar periféricos y módulos de aplicación.
 * - Encapsular el ciclo principal de ejecución.
 * - Coordinar la MEF principal sin agregar lógica de negocio.
 *
 * Dependencias importantes:
 * - keypad: adquisición de entrada del usuario.
 * - timer: temporización base de 1 segundo y 100 ms.
 * - display y actuators: presentación y control de hardware.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "keypad.h"
#include "microwave.h"
#include "timer.h"
#include "display.h"
#include "actuators.h"
#include <util/delay.h>


static void initialice(void)
{
    /* Inicialización secuencial para dejar el sistema en un estado conocido. */
    LCDinit();
    KEYPAD_Init();
    TIMER1_Init();
    TIMER0_Init();
    initActuators();
    LCDclr();
    Mostrar_Tiempo_LCD();
}




int main(void)
{   
    /* Inicializacion de Perifericos */
    initialice();

    /* Loop Principal */
    while (1)
    {
        if (flag_cien_milisegundos) {
            flag_cien_milisegundos = 0;
            MW_update();
        }
    }
}

