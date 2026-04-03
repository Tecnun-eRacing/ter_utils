#ifndef __TER_BIN_HEAP_H
#define __TER_BIN_HEAP_H

// Implementacion de Heap Binario Simple
// Hecho Sobre una Global, asi que cuidado al usarla de compartir estado global
// Ejemplo de uso:
// int min_int_cmp(int* a, int* b) {
//     return (*a) < (*b);
// }
//
// //         tipo,len, name, cmp_op
// CREATE_HEAP(int, 10, test, min_int_cmp)
//
// Y Luego para usarla:
// test_heap_push(val);
// int val = test_heap_pop();
// int* next_val = test_heap_peek();
// if  (next_val) { /* existe */ }

#include <stddef.h>

#define CREATE_HEAP(type, len, name, CMP_FUNC) \
type name##_HEAP[len]; \
size_t name##_HEAP_SIZE = 0; \
\
void name##_heap_push(type val) { \
    if (name##_HEAP_SIZE >= len) return; \
    name##_HEAP[name##_HEAP_SIZE] = val; \
    size_t curr = name##_HEAP_SIZE++; \
    while (curr > 0) { \
        size_t p = (curr - 1) / 2; \
        if (CMP_FUNC(&name##_HEAP[curr], &name##_HEAP[p])) { \
            type tmp = name##_HEAP[curr]; \
            name##_HEAP[curr] = name##_HEAP[p]; \
            name##_HEAP[p] = tmp; \
            curr = p; \
        } else break; \
    } \
} \
\
type name##_heap_pop() { \
    type root = name##_HEAP[0]; \
    name##_HEAP[0] = name##_HEAP[--name##_HEAP_SIZE]; \
    size_t curr = 0; \
    while (1) { \
        size_t left = 2 * curr + 1, right = 2 * curr + 2, target = curr; \
        if (left < name##_HEAP_SIZE && CMP_FUNC(&name##_HEAP[left], &name##_HEAP[target])) target = left; \
        if (right < name##_HEAP_SIZE && CMP_FUNC(&name##_HEAP[right], &name##_HEAP[target])) target = right; \
        if (target != curr) { \
            type tmp = name##_HEAP[curr]; \
            name##_HEAP[curr] = name##_HEAP[target]; \
            name##_HEAP[target] = tmp; \
            curr = target; \
        } else break; \
    } \
    return root; \
} \
type* name##_heap_peek() { \
    if (name##_HEAP_SIZE == 0) return NULL; \
    return &name##_HEAP[0]; \
}

#endif
