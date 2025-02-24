
#define MAX_QUEUE_SIZE 100

typedef struct {
    int data[MAX_QUEUE_SIZE];
    unsigned int front;
    unsigned int rear;
    unsigned int size;
    unsigned int max_size;
} ArrayQueue;

void initQueue(ArrayQueue *q, int _max_size) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->max_size = _max_size;
}

int enqueueArray(ArrayQueue *q, int value) {
    if (q->size == MAX_QUEUE_SIZE) {
        return -1;
    }
    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->size++;
    return 0;
}

int dequeueArray(ArrayQueue *q, int *value) {
    if (q->size == 0) {
        return -1;
    }
    *value = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->size--;
    return 0;
}
