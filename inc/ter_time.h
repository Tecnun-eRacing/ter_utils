#ifndef __TER_TIME_H
#define __TER_TIME_H

#include "main.h"
#include <stdint.h>

// Implementa un sheduler de cosas basicas con un timer. Este tiene que ir configurado de la siguiente manera:
// Importante el timer a una frecuancia relativamente baja, idealmente entre 1khz y 1mhz
// 1: Ponerle un source al timer a utilizar
// 2: Ponerlo en modo que cuente hacia abajo
// 3: Activarle el auto reload preload
// 4: En la pestaña NVIC, activarle el interrupt global de update
// 5: ACTIVAR SI NO ESTA ACTIVADO: cubemx -> project manager -> advanced settings -> register callback -> TIM -> enable
// truco: la frecuencia es la frecuencia del reloj / (prescaler + 1), asi se puede bajar, por ejemplo si el reloj esta a 16mhz y queremos 1mhz, prescaler a 15

// Cada tick es 1 ms
// Primero llamar ter_time_init al principio del programa
// Y luego se pueden llamar las funciones
// Todas tienen 2 variantes, run y spawn
// La run solo puede hacer cosas que no bloquean (No calculos largos, No hal delay...)
// La spawn requiere de tener ter_tasks configurado
// Luego las after son para realizar algo tras una cantidad de tiempo y las periodic para que se ejecute cada cierta cantidad de tiempo

#define TER_TIM_TASK_MAX_AMOUNT 16
#define TER_TIM_TASK_PERIOD_NEVER -1

void ter_time_init(TIM_HandleTypeDef* htim, uint32_t htim_khz);

void ter_time_run_after(void (*task)(void*), void* data, uint32_t after);
void ter_time_spawn_after(void (*task)(void*), void* data, uint32_t after);

void ter_time_run_periodic(void (*task)(void*), void* data, uint32_t period);
void ter_time_spawn_periodic(void (*task)(void*), void* data, uint32_t period);

#endif
