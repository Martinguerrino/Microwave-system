/*
 * main.c - Controlador de Horno a Microondas
 * Materia: Circuitos Digitales y Microcontroladores (E305) - UNLP
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
    KEYPAD_Init();
    LCDinit();
    TIMER1_Init();
    initActuators();
    LCDclr();
    Mostrar_Tiempo_LCD();
}




int main(void)
{   
    /* Inicializaci�n de Perif�ricos */
    initialice();

    /* Loop Principal */
    while (1)
    {
        MW_update();
    }
}

