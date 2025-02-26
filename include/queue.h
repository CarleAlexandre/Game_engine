
#define MAX_QUEUE_SIZE 100

typedef struct s_queue{
	int		data[MAX_QUEUE_SIZE];
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
}	queue_t;

static void init_queue(queue_t *q) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

//push new value at end of queue
static int push_queue(queue_t *q, int value) {
    if (q->size == MAX_QUEUE_SIZE) {
        return -1;
    }
    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->size++;
    return 0;
}

//pop value at start of queue
static int pop_queue(queue_t *q, int *value) {
    if (q->size == 0) {
        return -1;
    }
    *value = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->size--;
    return 0;
}
