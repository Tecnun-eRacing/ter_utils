#include "main.h"
#include <stddef.h>
#include "ter_tasks.h"
#include "ter_cs.h"

typedef struct {
    void (*task_fn)(void*);
    void* data;
} Task;

volatile Task TER_TASK_BUFFER[TER_TASK_BUFFER_LEN];
volatile size_t TER_TASKS_REMAINING = 0;
volatile size_t TER_TASKS_START_IDX = 0;

void loop_tasks() {
    while (TER_TASKS_REMAINING > 0) {
        CSState cs_state = enter_cs();
        Task task_to_run = TER_TASK_BUFFER[TER_TASKS_START_IDX];

        TER_TASKS_START_IDX = (TER_TASKS_START_IDX + 1) % TER_TASK_BUFFER_LEN;
        TER_TASKS_REMAINING--;
        exit_cs(cs_state);

        task_to_run.task_fn(task_to_run.data);
    }
    __WFI();
}

TaskCreationResult spawn_task(void (*task_fn)(void*), void* data) {
    CSState cs_state = enter_cs();
    if (TER_TASKS_REMAINING >= TER_TASK_BUFFER_LEN) {
        exit_cs(cs_state);
        return TASK_CREATION_RESULT_OVERRUN;
    }

    size_t insert_idx = (TER_TASKS_START_IDX + TER_TASKS_REMAINING) % TER_TASK_BUFFER_LEN;

    TER_TASK_BUFFER[insert_idx].task_fn = task_fn;
    TER_TASK_BUFFER[insert_idx].data = data;
    TER_TASKS_REMAINING++;

    exit_cs(cs_state);
    return TASK_CREATION_RESULT_OK;
}
