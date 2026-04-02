#ifndef __TER_CS
#define __TER_CS

#include <stdint.h>

// Implementacion de una seccion critica simple
// Valida para procesadores de un solo nucleo
// Hace que no haya conciciones de carrera
// Lo que hace es desactivar interrupciones y ya
// Es Equivalente a Adquirir un Mutex Global

typedef struct {
    uint32_t primask_bit;
} CSState;

CSState enter_cs();
void exit_cs(CSState cs_state);

#endif
