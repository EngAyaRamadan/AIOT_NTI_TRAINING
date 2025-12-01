
#define stack_size 10

typedef struct stack
{
    int arr_member[stack_size];
    int top;

} ST_Stack_T;

void createstack(ST_Stack_T *stack);
void push(ST_Stack_T *stack, int member);
int Pop(ST_Stack_T *stack, int member);
void printStack(ST_Stack_T *stack);
int getStackTop(ST_Stack_T *stack);
int S_isFull(ST_Stack_T *stack);
int isEmpty(ST_Stack_T *stack);