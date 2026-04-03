#ifndef __TER_TASKS_H
#define __TER_TASKS_H

#define TER_TASK_BUFFER_LEN 5

// Permite añadir tareas a una cola para que se ejecuten de forma bloqueante
// en el main tiene que aparecer loop_taks() dentro del user code begin while (en el bucle)
// Y nada mas, luego llamar spawn task inicia una tarea

typedef enum {
    TASK_CREATION_RESULT_OK,       // Se ejecutara
    TASK_CREATION_RESULT_OVERRUN,  // La cola de tareas se ha llenado, esto es malo, pasa si se piden demasiadas tareas y no hay tiempo para que se hagan todas
} TaskCreationResult;

void loop_tasks();

TaskCreationResult spawn_task(void (*task_fn)(void*), void* data);

#endif
