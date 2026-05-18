/*
 * microwave.h
 * ---
 * Define la interfaz y el contrato de la máquina de estados del microondas.
 *
 * Responsabilidades:
 * - Exponer los estados válidos de la MEF.
 * - Declarar la API de actualización de estado.
 * - Publicar el helper de conversión entre tiempo y dígitos visibles.
 *
 * Dependencias importantes:
 * - keypad: provee los eventos de teclas.
 * - timer: aporta el flag temporal de 1 segundo.
 * - display y actuators: reflejan el estado en LCD y hardware.
 */

#ifndef MICROWAVE_H
#define MICROWAVE_H

#include <stdint.h>

/*
 * Estados de la MEF principal.
 */
typedef enum {
    ESTADO_STANDBY,      /* Espera de entrada; todos los actuadores desactivados. */
    ESTADO_CONFIGURANDO,  /* Ingreso y edición del tiempo de cocción. */
    ESTADO_COCINANDO,     /* Cocción activa con temporización descendente. */
    ESTADO_ABIERTO,       /* Puerta abierta; el sistema inhibe acciones. */
    ESTADO_PAUSADO,       /* Cocción suspendida con tiempo conservado. */
    ESTADO_FINALIZADO     /* Ciclo finalizado con aviso. */
} EstadoMicroondas;

/*
 * Procesa la lógica del estado de espera.
 */
EstadoMicroondas Handle_ESTADO_STANDBY(uint8_t presiono, uint8_t tecla);
/*
 * Procesa el ingreso y edición del tiempo en formato MM:SS.
 */
EstadoMicroondas Handle_ESTADO_CONFIGURANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual);
/*
 * Procesa la cocción activa y los eventos permitidos durante el conteo.
 */
EstadoMicroondas Handle_ESTADO_COCINANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual);
/*
 * Procesa la transición entre pausa, reanudación y retorno a reposo.
 */
EstadoMicroondas Handle_ESTADO_ABIERTO(uint8_t presiono, uint8_t tecla);
/*
 * Procesa el estado de cocción suspendida por interacción del usuario.
 */
EstadoMicroondas Handle_ESTADO_PAUSADO(uint8_t presiono, uint8_t tecla);
/*
 * Procesa la etapa de finalización con aviso al usuario.
 */
EstadoMicroondas Handle_ESTADO_FINALIZADO(uint8_t presiono, uint8_t tecla);

/*
 * Actualiza la representación visible MM:SS a partir del tiempo total.
 */
void Actualizar_Digitos_Desde_Segundos(void);
/*
 * Aplica la MEF pura sobre la entrada recibida y devuelve el siguiente estado.
 */
EstadoMicroondas MEF_update(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual);
/*
 * Lee el teclado, filtra pulsaciones válidas y actualiza la MEF interna.
 */
void MW_update(void);

#endif
