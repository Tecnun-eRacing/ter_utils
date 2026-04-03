#ifndef __TER_RTT_H
#define __TER_RTT_H

// Añade 2 funciones. Hay que llamar ter_rtt_init() al principio del programa
// Y esto hace que llamar ter_rtt_printf funcinone cono print
// definir NOOP_PRINTS desactiva toda la logica

void ter_rtt_init();
int ter_rtt_printf(const char * sFormat, ...);

#endif
