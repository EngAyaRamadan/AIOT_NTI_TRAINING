
#define queue_size 10

typedef struct queue
{
    int arr_member[queue_size];
    int front, rear;

} ST_queue_T;

void createqueue(ST_queue_T *queue);
void enqueue(ST_queue_T *queue, int member);
void dequeue(ST_queue_T *queue, int *member);
void printqueue(ST_queue_T *queue);
int getqueuefront(ST_queue_T *queue);
int getqueuerear(ST_queue_T *queue);
int Q_isFull(ST_queue_T *queue);
int Q_isempty(ST_queue_T *queue);