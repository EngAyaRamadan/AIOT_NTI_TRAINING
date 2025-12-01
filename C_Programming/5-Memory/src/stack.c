#include <stdio.h>
#include "stack.h"

void createstack(ST_Stack_T *stack)
{
    stack->top = -1;
}
void push(ST_Stack_T *stack, int member)
{
    stack->top++;
    stack->arr_member[stack->top] = member;
}
int Pop(ST_Stack_T *stack, int member)
{
    member = stack->arr_member[stack->top];
    --stack->top;

    return member;
}
void printStack(ST_Stack_T *stack)
{
    for (int i = 0; i <= stack->top; i++)
    {
        printf("%d\n", stack->arr_member[i]);
    }
}
int getStackTop(ST_Stack_T *stack)
{
    return ((stack->arr_member[stack->top]));
}
int S_isFull(ST_Stack_T *stack)
{
    if (stack->top == (stack_size - 1))
    {
        printf("The stack is full");
        return 1;
    }
    else
    {
        return 0;
    }
}
int S_isEmpty(ST_Stack_T *stack)
{
    if (stack->top == -1)
    {
        printf("The stack is embty");
        return 1;
    }
    else
    {
        return 0;
    }
}