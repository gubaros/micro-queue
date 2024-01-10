#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_MSG_SIZE 256
#define QUEUE_SIZE 10

typedef struct {
    char data[MAX_MSG_SIZE];
} Message;

typedef struct {
    Message messages[QUEUE_SIZE];
    int front;
    int rear;
} MessageQueue;

void initializeQueue(MessageQueue *q) {
    q->front = -1;
    q->rear = -1;
}

int isFull(MessageQueue *q) {
    return (q->rear + 1) % QUEUE_SIZE == q->front;
}

int isEmpty(MessageQueue *q) {
    return q->front == -1;
}

void enqueue(MessageQueue *q, char *msg) {
    if (isFull(q)) {
        return;
    }
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    strcpy(q->messages[q->rear].data, msg);
}

void dequeue(MessageQueue *q, char *msg) {
    if (isEmpty(q)) {
        return;
    }
    strcpy(msg, q->messages[q->front].data);
    if (q->front == q->rear) {
        initializeQueue(q);
    } else {
        q->front = (q->front + 1) % QUEUE_SIZE;
    }
}

int main() {
    pid_t pid, sid;
    
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
            
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Aquí comienza la lógica del daemon
    MessageQueue q;
    initializeQueue(&q);

    // Ejemplo de cómo se pueden encolar y desencolar mensajes
    enqueue(&q, "Mensaje 1");
    enqueue(&q, "Mensaje 2");

    char msg[MAX_MSG_SIZE];
    while (1) {
        if (!isEmpty(&q)) {
            dequeue(&q, msg);
            // Realizar acción con el mensaje, como loggearlo en un archivo
        }
        sleep(1); // Espera de 1 segundo para simular trabajo y evitar uso excesivo de CPU
    }

    return 0;
}

