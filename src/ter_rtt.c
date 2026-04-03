#include "ter_rtt.h"

#ifndef NOOP_PRINTS

#include "SEGGER_RTT.h"

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
