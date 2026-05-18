/*
 * microwave.c
 * ---
 * Implementa la máquina de estados principal del microondas.
 *
 * Responsabilidades:
 * - Interpretar eventos de teclado.
 * - Mantener el tiempo de cocción y su representación visible.
 * - Coordinar actuadores, LCD y alarmas según el estado.
 * - Encapsular reglas de transición y restricciones de operación.
 *
 * Dependencias importantes:
 * - actuators: magnetrón, luz interior y buzzer.
 * - display: actualización de la visualización del tiempo.
 * - keypad: eventos de usuario.
 * - timer: flag de un segundo para el conteo descendente.
 */

#include <avr/io.h>
#include "microwave.h"
#include "actuators.h"
#include "lcd.h"
#include "display.h"
#include "keypad.h"
#include "timer.h"

#define MAX_SEGUNDOS 5999


uint8_t digitos[4] = {0, 0, 0, 0};
uint16_t total_segundos = 0;
uint8_t contador_finalizado = 0;
uint8_t visible_finalizado = 1;
uint8_t cantidad_digitos = 0;


static EstadoMicroondas estado_actual = ESTADO_STANDBY;


/*
 * Convierte el tiempo total almacenado a dígitos visibles MM:SS.
 */
void Actualizar_Digitos_Desde_Segundos(void) {
    uint8_t min_act = total_segundos / 60;
    uint8_t seg_act = total_segundos % 60;
    digitos[0] = min_act / 10;
    digitos[1] = min_act % 10;
    digitos[2] = seg_act / 10;
    digitos[3] = seg_act % 10;
}

/*
 * Lleva al sistema a un estado seguro al detectar apertura de puerta.
 */
static void Abrio(void)
{
    Magnetron_Off();
    InteriorLight_Off();
    LCDGotoXY(0, 1);
    LCDstring((uint8_t*)"Puerta Abierta! ", 16);
}

/*
 * Maneja el estado de espera: arma una nueva cocción o inicia el modo rápido.
 */
EstadoMicroondas Handle_ESTADO_STANDBY(uint8_t presiono, uint8_t tecla) {
    /* En reposo se fuerza hardware inactivo para mantener una salida segura. */
    Magnetron_Off();
    InteriorLight_Off();
    Alarm_off();
    
    if (presiono) {
        if (tecla >= '0' && tecla <= '9') {
            digitos[0] = 0; digitos[1] = 0; digitos[2] = 0;
            digitos[3] = tecla - '0';
            cantidad_digitos = 1; 
            Mostrar_Tiempo_LCD();
            return ESTADO_CONFIGURANDO;
        }
        else if (tecla == 'C') {
            /* El modo rápido evita la secuencia de configuración para uso frecuente. */
            total_segundos = 30;
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Cocinando...    ", 16);
            Magnetron_On();
            InteriorLight_On();
            return ESTADO_COCINANDO;
        }
    }
    return ESTADO_STANDBY;
}

/*
 * Maneja la edición del tiempo y la validación de límites antes de cocinar.
 */
EstadoMicroondas Handle_ESTADO_CONFIGURANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual) {
    if (presiono) {
        /* Se limita a cuatro dígitos para preservar el formato MM:SS. */
        if (tecla >= '0' && tecla <= '9') {
            if (cantidad_digitos < 4) {
                /* Desplazamiento a la izquierda para simular ingreso secuencial. */
                digitos[0] = digitos[1];
                digitos[1] = digitos[2];
                digitos[2] = digitos[3];
                digitos[3] = tecla - '0';
                cantidad_digitos++;
                Mostrar_Tiempo_LCD();
            }
        }
        else if (tecla == 'B') {
            /* B actúa como cancelación explícita de la configuración. */
            digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
            cantidad_digitos = 0;
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"                ", 16);
            return ESTADO_STANDBY;
        }
        else if (tecla == 'A') {
            /* Se traduce el tiempo visible a segundos para el conteo interno. */
            uint8_t min = (digitos[0] * 10) + digitos[1];
            uint8_t seg = (digitos[2] * 10) + digitos[3];
            total_segundos = (min * 60) + seg;
            
            /* El techo evita desbordes y mantiene el rango operativo previsto. */
            if (total_segundos > MAX_SEGUNDOS) {
                total_segundos = MAX_SEGUNDOS; 
            }
            
            if (total_segundos > 0) {
                cantidad_digitos = 0; 
                Actualizar_Digitos_Desde_Segundos(); 
                Mostrar_Tiempo_LCD();
                
                LCDGotoXY(0, 1);
                LCDstring((uint8_t*)"Cocinando...    ", 16);
                Magnetron_On();
                InteriorLight_On();
                return ESTADO_COCINANDO;
            }
        }
        else if (tecla == 'C') {
            /* Incremento rápido para ajustes pequeños sin abandonar la edición. */
            uint8_t min = (digitos[0] * 10) + digitos[1];
            uint8_t seg = (digitos[2] * 10) + digitos[3];
            total_segundos = (min * 60) + seg + 30;
            
            /* El mismo límite se aplica aquí para conservar coherencia funcional. */
            if (total_segundos > MAX_SEGUNDOS) {
                total_segundos = MAX_SEGUNDOS;
            }
            
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
        }
    }
    return ESTADO_CONFIGURANDO;
}

/*
 * Maneja la cocción activa y los eventos permitidos durante el conteo.
 */
EstadoMicroondas Handle_ESTADO_COCINANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual) {
    /* El flag de 1 segundo desacopla el tiempo del bucle principal. */
    if (flag_un_segundo) {
        flag_un_segundo = 0;
        if (total_segundos > 0) {
            total_segundos--;
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
        }
        
        if (total_segundos == 0) {
            /* Al terminar se apagan salidas y se entra en una fase de aviso. */
            Magnetron_Off();
            InteriorLight_Off();
            Alarm_On();
            contador_finalizado = 0;
            visible_finalizado = 1;
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Fin Coccion!    ", 16);
            return ESTADO_FINALIZADO;
        }

    }
    
    /* Solo se aceptan acciones que preservan la seguridad y la continuidad. */
    if (presiono) {
        if (tecla == 'B') {
            /* La pausa conserva el tiempo restante y corta la potencia. */
            Magnetron_Off();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Pausado...      ", 16);
            return ESTADO_PAUSADO;
        }
        else if (tecla == 'C') {
            total_segundos += 30;
            
            /* La misma cota evita que el modo rápido supere el rango del diseño. */
            if (total_segundos > MAX_SEGUNDOS) {
                total_segundos = MAX_SEGUNDOS;
            }
            
            Actualizar_Digitos_Desde_Segundos();
            Mostrar_Tiempo_LCD();
        }
        else if (tecla == 'D') {
            /* Abrir la puerta obliga a inhibir cocción y declarar la condición. */
            Abrio();
            return ESTADO_ABIERTO;
        }
    }
    return ESTADO_COCINANDO;
}

/*
 * Maneja la cocción suspendida sin perder el tiempo acumulado.
 */
EstadoMicroondas Handle_ESTADO_PAUSADO(uint8_t presiono, uint8_t tecla) {
    if (presiono) {
        if (tecla == 'A') {
            /* Reanudar vuelve a habilitar salidas y mantiene el tiempo restante. */
            Magnetron_On();
            InteriorLight_On();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Cocinando...    ", 16);
            return ESTADO_COCINANDO;
        } 
        else if (tecla == 'D') {
            /* Si la puerta se cierra, solo se limpia el mensaje sin asumir cocción. */
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"                ", 16);
            return ESTADO_PAUSADO;
        }
        else if (tecla == 'B') {
            /* Cancelación explícita: borra la sesión y vuelve a reposo. */
            digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"                ", 16);
            return ESTADO_STANDBY;
        }
    }
    return ESTADO_PAUSADO;
}

/*
 * Mantiene el sistema inhibido mientras la puerta permanece abierta.
 */
EstadoMicroondas Handle_ESTADO_ABIERTO(uint8_t presiono, uint8_t tecla) {
    /* Se conserva el tiempo visible para no perder contexto del usuario. */
    if (presiono) {
        if (tecla == 'D') {
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"                ", 16);
            Mostrar_Tiempo_LCD();
            return ESTADO_CONFIGURANDO;
        }
    }
    return ESTADO_ABIERTO;
}

/*
 * Ejecuta el patrón de finalización y espera confirmación del usuario.
 */
EstadoMicroondas Handle_ESTADO_FINALIZADO(uint8_t presiono, uint8_t tecla) {
    /* El parpadeo marca el fin del ciclo sin bloquear el lazo principal. */
    if (flag_un_segundo) {
        flag_un_segundo = 0;
        contador_finalizado++;
        visible_finalizado = !visible_finalizado; 
        
        if (visible_finalizado) {
            Mostrar_Tiempo_LCD();
            LCDGotoXY(0, 1);
            LCDstring((uint8_t*)"Fin Coccion!    ", 16);
            Alarm_On();
        } else {
            LCDclr();           
            Alarm_off();
        }
        
        if (contador_finalizado >= 5) {
            digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
            LCDclr();
            Mostrar_Tiempo_LCD();
            return ESTADO_STANDBY;
        }
    }

    /* Cualquier interacción descarta la alarma y limpia el estado final. */
    if (presiono) {
        Alarm_off();
        digitos[0] = digitos[1] = digitos[2] = digitos[3] = 0;
        LCDclr();
        Mostrar_Tiempo_LCD();
        return ESTADO_STANDBY;
    }
    
    return ESTADO_FINALIZADO;
}

/*
 * Despacha la lógica de estado sin mezclar la selección con la implementación.
 */
EstadoMicroondas MEF_update(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual) {
    
    if (presiono && tecla == 'D' && estado_actual != ESTADO_ABIERTO) {
        Abrio();
        return ESTADO_ABIERTO;
    }
    switch (estado_actual) {
        case ESTADO_STANDBY:
            return Handle_ESTADO_STANDBY(presiono, tecla);

        case ESTADO_CONFIGURANDO:
            return Handle_ESTADO_CONFIGURANDO(presiono, tecla, estado_actual);

        case ESTADO_COCINANDO:
            return Handle_ESTADO_COCINANDO(presiono, tecla, estado_actual);

        case ESTADO_PAUSADO:
            return Handle_ESTADO_PAUSADO(presiono, tecla);

        case ESTADO_ABIERTO:
            return Handle_ESTADO_ABIERTO(presiono, tecla);

        case ESTADO_FINALIZADO:
            return Handle_ESTADO_FINALIZADO(presiono, tecla);

        default:
            return ESTADO_STANDBY;
    }
}

/*
 * Lee el teclado y actualiza la MEF interna que gobierna el sistema.
 */
void MW_update(void) {
    uint8_t tecla;
    uint8_t presiono = KEYPAD_Scan(&tecla);
    estado_actual = MEF_update(presiono, tecla, estado_actual);
}
