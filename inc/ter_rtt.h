#ifndef __TER_RTT_H
#define __TER_RTT_H

#include "SEGGER_RTT.h"

// Añade 2 funciones. Hay que llamar ter_rtt_init() al principio del programa
// Y esto hace que llamar ter_rtt_printf funcinone cono print
// definir NOOP_PRINTS desactiva toda la logica

#ifndef NOOP_PRINTS
void ter_rtt_init() {
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
}

int ter_rtt_printf(const char * sFormat, ...) {
    int r;
    va_list ParamList;

    va_start(ParamList, sFormat);
    r = SEGGER_RTT_vprintf(0, sFormat, &ParamList);
    va_end(ParamList);

    return r;
}
#else
void ter_rtt_init() {}
int ter_rtt_printf(const char * sFormat, ...) {
    (void)sFormat;
    return 0;
}
#endif

#endif
