#ifndef STACK_H 
#define STACK_H 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct {
    uint16_t *collections;
    int capacity;
    uint16_t size;
    
} Stack;



Stack *create_stack(int capacity);
void destroy_stack(Stack *stack);
bool is_full(Stack *stack);
bool is_empty(Stack *stack);
bool push(Stack *stack, uint16_t item);
bool pop(Stack *stack, uint16_t *item);


#endif
