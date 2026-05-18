#ifndef MICROWAVE_H
#define MICROWAVE_H

#include <stdint.h>

// Estados de la MEF
typedef enum {
    ESTADO_STANDBY,
    ESTADO_CONFIGURANDO,
    ESTADO_COCINANDO,
    ESTADO_PAUSADO,
    ESTADO_FINALIZADO
} EstadoMicroondas;

// Funciones para manejar cada estado
EstadoMicroondas Handle_ESTADO_STANDBY(uint8_t presiono, uint8_t tecla);
EstadoMicroondas Handle_ESTADO_CONFIGURANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual);
EstadoMicroondas Handle_ESTADO_COCINANDO(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual);
EstadoMicroondas Handle_ESTADO_PAUSADO(uint8_t presiono, uint8_t tecla);
EstadoMicroondas Handle_ESTADO_FINALIZADO(uint8_t presiono, uint8_t tecla);

// Función auxiliar para actualizar dígitos desde segundos
void Actualizar_Digitos_Desde_Segundos(void);
// MEF pura: toma entrada y estado, devuelve nuevo estado
EstadoMicroondas MEF_update(uint8_t presiono, uint8_t tecla, EstadoMicroondas estado_actual);
// Envoltorio público: lee teclado y actualiza la MEF interna
void MW_update(void);

#endif
