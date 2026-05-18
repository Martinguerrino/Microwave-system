/*
 * actuators.h
 * ---
 * Declara la capa de abstracción de salidas físicas del microondas.
 *
 * Responsabilidades:
 * - Encapsular el control de pines del magnetrón, luz y alarma.
 * - Evitar que la lógica de aplicación manipule registros directos.
 * - Mantener una interfaz simple y reutilizable para la MEF.
 *
 * Dependencias importantes:
 * - avr/io: acceso a DDR y PORT.
 */

#ifndef ACTUATORS_H
#define ACTUATORS_H

/*
 * Configura las salidas físicas y deja los actuadores en estado seguro.
 */
void initActuators(void);
/*
 * Habilita el pin asociado al magnetrón.
 */
void Magnetron_On(void);
/*
 * Deshabilita el pin asociado al magnetrón.
 */
void Magnetron_Off(void);
/*
 * Enciende la luz interior de la cavidad.
 */
void InteriorLight_On(void);
/*
 * Apaga la luz interior de la cavidad.
 */
void InteriorLight_Off(void);
/*
 * Activa la alarma de fin de ciclo.
 */
void Alarm_On(void);
/*
 * Desactiva la alarma de fin de ciclo.
 */
void Alarm_off(void);

#endif
