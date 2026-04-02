#include "main.h"
#include "ter_cs.h"

CSState enter_cs() {
    CSState state;
    state.primask_bit = __get_PRIMASK();
    __disable_irq();
    return state;
}

void exit_cs(CSState cs_state) {
    __set_PRIMASK(cs_state.primask_bit);
}
