/**
 * @file queue.h
 * @author Iván Mansilla, Ayrton Morrison, Milton Hernández
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>


/*----------------ESTRUCTURAS-----------------------*/


/*************************************** */
typedef struct process{
    unsigned int PID;/*!< Identificador del Proceso*/
    unsigned int arrivalTime;/*!< Tiempo de llegada del proceso*/
    unsigned int burstTime;/*!< Tiempo requerido por el proceso*/
    unsigned int memoryRequired;/*!< Memoria solicitada por el proceso*/
}process;
/*************************************** */



/* NODOS PARA LISTA CIRCULAR*/
typedef struct circularNode{
    process process; /*!< Proceso que contiene el nodo*/
    struct circularNode* next; /*!< Puntero a nodo siguiente*/
    struct circularNode* prev; /*!< Puntero a nodo anterior*/
} circularNode;
/*--------------------------*/


/* COLA */
typedef struct queueRecord{
    circularNode* centinel; /*!< Next del centinela = front, prev=rear*/
} queueRecord;



typedef queueRecord* queue; // puntero a la cola

/*---------------------FUNCIONES--------------------*/
bool is_empty(queue Q);

queue create_queue();
void make_empty(queue Q);
void free_queue(queue Q);


void enqueue( process X, queue Q );
process front( queue Q );
void dequeue( queue Q );

void print_queue(queue Q);
