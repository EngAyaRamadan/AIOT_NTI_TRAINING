#include <stdio.h>
#include "queue.h"

void createqueue(ST_queue_T *queue)
{
    queue->front = -1;
    queue->rear = -1;
}
void enqueue(ST_queue_T *queue, int member)
{
    if (queue->front == -1 && queue->rear == -1)
    {
        queue->front++;
    }
    queue->rear++;
    queue->arr_member[queue->rear] = member;
}
void dequeue(ST_queue_T *queue, int *member)
{
    *member = queue->arr_member[queue->front];
    queue->front++;
}
void printqueue(ST_queue_T *queue)
{
    for (int i = queue->front; i <= queue->rear; i++)
    {
        printf("%d\n", queue->arr_member[i]);
    }
}
int getqueuefront(ST_queue_T *queue)
{
    return ((queue->arr_member[queue->front]));
}
int getqueuerear(ST_queue_T *queue)
{
    return ((queue->arr_member[queue->rear]));
}
int Q_isFull(ST_queue_T *queue)
{
    if ((queue->rear - queue->front) == (queue_size - 1))
    {
        printf("The queue is full");
        return 1;
    }
    else
    {
        return 0;
    }
}
int Q_isEmpty(ST_queue_T *queue)
{
    if (queue->front == -1)
    {
        printf("The queue is embty");
        return 1;
    }
    else
    {
        return 0;
    }
}