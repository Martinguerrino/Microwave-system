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
#include <util/delay.h>

// El techo matem�tico m�ximo: 99 minutos y 59 segundos
#define MAX_SEGUNDOS 5999 


// Matriz de teclado en formato telef�nico

// --- Variables Globales ---
uint8_t digitos[4] = {0, 0, 0, 0};       
uint16_t total_segundos = 0;             
volatile uint8_t flag_un_segundo = 0;    
uint8_t contador_finalizado = 0;         
uint8_t visible_finalizado = 1;          
uint8_t cantidad_digitos = 0;            




int main(void)
{   
    /* Inicializaci�n de Perif�ricos */
    KEYPAD_Init();
    LCDinit();
    TIMER1_Init();
    
    // Actuadores
    DDRB |= (1<<PB5);            
    DDRC |= (1<<PC4) | (1<<PC5); 
    PORTB &= ~(1<<PB5);
    PORTC &= ~((1<<PC4) | (1<<PC5));

    EstadoMicroondas estado_actual = ESTADO_STANDBY;

    LCDclr();
    Mostrar_Tiempo_LCD(); 

    /* Loop Principal */
    while (1)
    {
	    uint8_t tecla;
	    uint8_t presiono = KEYPAD_Scan(&tecla);
	    
        estado_actual = MEF_update(presiono, tecla, estado_actual);
    }
}

