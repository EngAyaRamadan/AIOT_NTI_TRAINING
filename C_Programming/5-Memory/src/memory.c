/* Date: 12/11/2025 */
#include <stdio.h>
#include "stack.h"
#include "queue.h"

#pragma pack(push, 1)
struct Packed
{
    char c;
    int i;
};
#pragma pack(pop)

int main()
{

    // 1 check not allignment
    struct Packed x;
    printf("size of %d\n", sizeof(x));

    // 2 make stack using array and print its data
    ST_Stack_T S_data;
    int S_member;
    createstack(&S_data);
    printf("Enter 5 number :");
    for (int i = 0; i < 5; i++)
    {
        scanf("%d", &S_member);
        push(&S_data, S_member);
    }
    printStack(&S_data);
    int S_top = getStackTop(&S_data);
    printf("The top element : %d\n", S_top);
    if (!(S_isFull(&S_data)))
    {
        printf("not full\n");
    }

    // 3  make queue using array and print its data
    ST_queue_T Q_data;
    int Q_member;
    createqueue(&Q_data);
    printf("printf Enter 5 number :");
    for (int i = 0; i < 5; i++)
    {
        scanf("%d", &Q_member);
        enqueue(&Q_data, Q_member);
    }

    printqueue(&Q_data);
    int Q_top = getqueuerear(&Q_data);
    printf("the last number is : %d \n", Q_top);

    if (!(Q_isFull(&Q_data)))
    {
        printf("not full\n");
    }

    return 0;
}