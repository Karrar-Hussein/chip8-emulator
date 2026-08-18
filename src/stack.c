#include "stack.h"


Stack *create_stack(int capacity){
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    stack->collections = malloc(sizeof(uint16_t) * capacity);
    if (stack->collections == NULL){
        free(stack);
        return NULL;
    }

    stack->capacity = capacity;
    stack->size = 0;

    return stack;
}

void destroy_stack(Stack *stack){
    free(stack);
}

bool is_full(Stack *stack){
    return stack->capacity == stack->size;
}

bool is_empty(Stack *stack){
    return stack->size == 0;
}

bool push(Stack *stack, uint16_t item){
    if (is_full(stack)){
        fprintf(stderr, "The stack is full.");
        return false;
    }
    stack->collections[stack->size] = item;
    stack->size++;

    return true;
}

bool pop(Stack *stack, uint16_t *item){
    if (is_empty(stack)){
        fprintf(stderr, "cannot pop the stack is empty!");
        return false;
    }
    stack->size--;
    *item = stack->collections[stack->size];

    return true;
}


