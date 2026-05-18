/*
 * microwave.c - Manejadores de estados para Horno a Microondas
 * Materia: Circuitos Digitales y Microcontroladores (E305) - UNLP
 */

#include <avr/io.h>
#include "microwave.h"
#include "lcd.h"
#include "display.h"

#define MAX_SEGUNDOS 5999

// Variables globales (externs declaradas en main.c)
extern uint8_t digitos[4];
extern uint16_t total_segundos;
extern volatile uint8_t flag_un_segundo;
extern uint8_t contador_finalizado;
extern uint8_t visible_finalizado;
extern uint8_t cantidad_digitos;

// Función auxiliar
void Actualizar_Digitos_Desde_Segundos(void) {
    uint8_t min_act = total_segundos / 60;
    uint8_t seg_act = total_segundos % 60;
    digitos[0] = min_act / 10;
    digitos[1] = min_act % 10;
    digitos[2] = seg_act / 10;
    digitos[3] = seg_act % 10;
}

// Manejador para ESTADO_STANDBY
EstadoMicroondas Handle_ESTADO_STANDBY(uint8_t presiono, uint8_t tecla) {
    // Apagar todos los actuadores
    PORTB &= ~(1<<PB5);
    PORTC &= ~((1<<PC4) | (1<<PC5));
    
    if (presiono) {
        if (tecla >= '0' && tecla <= '9') {
            // Cambiar a ESTADO_CONFIGURANDO
            digitos[0] = 0; digitos[1] = 0; digitos[2] = 0;
            digitos[3] = tecla - '0';
            cantidad_digitos = 1; 
            Mostrar_Tiempo_LCD();
            return ESTADO_CONFIGURANDO;
        }
        else if (tecla == 'C') {
            // Cambiar a ESTADO_COCINANDO directamente con 30 segundos
            total_segundos = 30;
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Cocinando...    ", 16);
            PORTB |= (1<<PB5); 
            PORTC |= (1<<PC4); 
            return ESTADO_COCINANDO;
        }
    }
    return ESTADO_STANDBY;
}

// Manejador para ESTADO_CONFIGURANDO
EstadoMicroondas Handle_ESTADO_CONFIGURANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual) {
    if (presiono) {
        // Tope de entrada de 4 teclas
        if (tecla >= '0' && tecla <= '9') {
            if (cantidad_digitos < 4) {
                digitos[0] = digitos[1];
                digitos[1] = digitos[2];
                digitos[2] = digitos[3];
                digitos[3] = tecla - '0';
                cantidad_digitos++;
                Mostrar_Tiempo_LCD();
            }
        }
        else if (tecla == 'B') {
            // Volver a ESTADO_STANDBY
            digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
            cantidad_digitos = 0;
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"                ", 16);
            return ESTADO_STANDBY;
        }
        else if (tecla == 'A') {
            // Iniciar cocción
            uint8_t min = (digitos[0] * 10) + digitos[1];
            uint8_t seg = (digitos[2] * 10) + digitos[3];
            total_segundos = (min * 60) + seg;
            
            // APLICAMOS EL TECHO MATEMÁTICO AL INICIAR
            if (total_segundos > MAX_SEGUNDOS) {
                total_segundos = MAX_SEGUNDOS; 
            }
            
            if (total_segundos > 0) {
                cantidad_digitos = 0; 
                Actualizar_Digitos_Desde_Segundos(); 
                Mostrar_Tiempo_LCD();
                
                LCDGotoXY(0, 1);
                LCDstring((uint8_t*)"Cocinando...    ", 16);
                PORTB |= (1<<PB5); 
                PORTC |= (1<<PC4); 
                return ESTADO_COCINANDO;
            }
        }
        else if (tecla == 'C') {
            // Sumar 30 segundos sin dejar configurando
            uint8_t min = (digitos[0] * 10) + digitos[1];
            uint8_t seg = (digitos[2] * 10) + digitos[3];
            total_segundos = (min * 60) + seg + 30;
            
            // APLICAMOS EL TECHO MATEMÁTICO EN EL +30
            if (total_segundos > MAX_SEGUNDOS) {
                total_segundos = MAX_SEGUNDOS;
            }
            
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
        }
    }
    return ESTADO_CONFIGURANDO;
}

// Manejador para ESTADO_COCINANDO
EstadoMicroondas Handle_ESTADO_COCINANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual) {
    // Manejo del decremento de tiempo
    if (flag_un_segundo) {
        flag_un_segundo = 0;
        if (total_segundos > 0) {
            total_segundos--;
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
        }
        
        if (total_segundos == 0) {
            PORTB &= ~(1<<PB5); 
            PORTC &= ~(1<<PC4); 
            PORTC |= (1<<PC5);  
            contador_finalizado = 0;
            visible_finalizado = 1;
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Fin Coccion!    ", 16);
            return ESTADO_FINALIZADO;
        }
    }
    
    // Manejo de teclas durante cocción
    if (presiono) {
        if (tecla == 'B') {
            // Pausar
            PORTB &= ~(1<<PB5); 
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Pausado...      ", 16);
            return ESTADO_PAUSADO;
        }
        else if (tecla == 'C') {
            // Sumar 30 segundos
            total_segundos += 30;
            
            // APLICAMOS EL TECHO MATEMÁTICO AL SUMAR TIEMPO EN CALIENTE
            if (total_segundos > MAX_SEGUNDOS) {
                total_segundos = MAX_SEGUNDOS;
            }
            
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
        }
        else if (tecla == 'D') {
            // Abrir puerta
            PORTB &= ~(1<<PB5); 
            PORTC &= ~(1<<PC4); 
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Puerta Abierta! ", 16);
            return ESTADO_PAUSADO;
        }
    }
    return ESTADO_COCINANDO;
}

// Manejador para ESTADO_PAUSADO
EstadoMicroondas Handle_ESTADO_PAUSADO(uint8_t presiono, uint8_t tecla) {
    if (presiono) {
        if (tecla == 'A') {
            // Reanudar cocción
            PORTB |= (1<<PB5); 
            PORTC |= (1<<PC4); 
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Cocinando...    ", 16);
            return ESTADO_COCINANDO;
        } 
        else if (tecla == 'B') {
            // Volver a ESTADO_STANDBY
            digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"                ", 16);
            return ESTADO_STANDBY;
        }
    }
    return ESTADO_PAUSADO;
}

// Manejador para ESTADO_FINALIZADO
EstadoMicroondas Handle_ESTADO_FINALIZADO(uint8_t presiono, uint8_t tecla) {
    // Parpadeo de finalización
    if (flag_un_segundo) {
        flag_un_segundo = 0;
        contador_finalizado++;
        visible_finalizado = !visible_finalizado; 
        
        if (visible_finalizado) {
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Fin Coccion!    ", 16);
            PORTC |= (1<<PC5);  
        } else {
            LCDclr();           
            PORTC &= ~(1<<PC5); 
        }
        
        if (contador_finalizado >= 5) {
            digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
            LCDclr();
            Mostrar_Tiempo_LCD();
            return ESTADO_STANDBY;
        }
    }

    // Cualquier tecla presionada vuelve a STANDBY
    if (presiono) {
        PORTC &= ~(1<<PC5);
        digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
        LCDclr();
        Mostrar_Tiempo_LCD();
        return ESTADO_STANDBY;
    }
    
    return ESTADO_FINALIZADO;
}
