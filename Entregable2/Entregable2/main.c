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
 * - timer: temporización base de 1 segundo.
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
    KEYPAD_Init();
    LCDinit();
    TIMER1_Init();
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
        MW_update();
    }
}

