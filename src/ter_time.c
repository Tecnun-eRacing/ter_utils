#include "ter_time.h"
#include "ter_bin_heap.h"
#include "ter_cs.h"
#include "ter_tasks.h"

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

TIM_HandleTypeDef* HTIM;
uint32_t HTIM_KHZ;

uint32_t CURRENT_TARGET_TICK = -1;

void ter_time_update_current_target_tick(uint32_t new_target_tick) {
    CSState cs_state = enter_cs();
    CURRENT_TARGET_TICK = new_target_tick;
    HTIM->Instance->CNT = (new_target_tick - HAL_GetTick()) * HTIM_KHZ;
    exit_cs(cs_state);
}

typedef struct {
    void (*task)(void*);
    void* data;
    uint32_t next_when;
    uint32_t period; // -1 means never
    bool must_spawn;
} TerTimTask;

int min_tim_task_cmp(TerTimTask* a, TerTimTask* b) {
    return a->next_when < b->next_when;
}

CREATE_HEAP(TerTimTask, TER_TIM_TASK_MAX_AMOUNT, ter_time_heap, min_tim_task_cmp)

void ter_time_append_task(TerTimTask tim_task) {
    CSState cs_state = enter_cs();
    if (tim_task.next_when < CURRENT_TARGET_TICK) ter_time_update_current_target_tick(tim_task.next_when);
    ter_time_heap_heap_push(tim_task);
    exit_cs(cs_state);
}

void ter_time_period_callback(TIM_HandleTypeDef *htim) {
    if (htim != HTIM) return;
    CSState cs_state = enter_cs();

    while (ter_time_heap_heap_peek() && ter_time_heap_heap_peek()->next_when <= HAL_GetTick()) {
        TerTimTask task = ter_time_heap_heap_pop();

        if (task.must_spawn) spawn_task(task.task, task.data);
        else task.task(task.data);

        if (task.period != TER_TIM_TASK_PERIOD_NEVER) {
            task.next_when += task.period;
            ter_time_append_task(task);
        }
    }

    TerTimTask* next_task = ter_time_heap_heap_peek();
    if (next_task) {
        ter_time_update_current_target_tick(next_task->next_when);
    }
    exit_cs(cs_state);
}

void ter_time_init(TIM_HandleTypeDef* htim, uint32_t htim_khz) {
    HTIM = htim;
    HTIM_KHZ = htim_khz;

    HAL_TIM_RegisterCallback(HTIM, HAL_TIM_PERIOD_ELAPSED_CB_ID, ter_time_period_callback);
    HAL_TIM_Base_Start_IT(HTIM);
}

void ter_time_run_after(void (*task)(void*), void* data, uint32_t after) {
    TerTimTask tim_task = {
        .task = task,
        .data = data,
        .next_when = HAL_GetTick() + after,
        .period = TER_TIM_TASK_PERIOD_NEVER,
        .must_spawn = false,
    };
    ter_time_append_task(tim_task);
}

void ter_time_spawn_after(void (*task)(void*), void* data, uint32_t after) {
    TerTimTask tim_task = {
        .task = task,
        .data = data,
        .next_when = HAL_GetTick() + after,
        .period = TER_TIM_TASK_PERIOD_NEVER,
        .must_spawn = true,
    };
    ter_time_append_task(tim_task);
}

void ter_time_run_periodic(void (*task)(void*), void* data, uint32_t period) {
    TerTimTask tim_task = {
        .task = task,
        .data = data,
        .next_when = HAL_GetTick() + period,
        .period = period,
        .must_spawn = false,
    };
    ter_time_append_task(tim_task);
}

void ter_time_spawn_periodic(void (*task)(void*), void* data, uint32_t period) {
    TerTimTask tim_task = {
        .task = task,
        .data = data,
        .next_when = HAL_GetTick() + period,
        .period = period,
        .must_spawn = true,
    };
    ter_time_append_task(tim_task);
}
