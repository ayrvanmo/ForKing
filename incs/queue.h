/**
 * @file queue.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 */

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"

typedef struct _circularNode CircularNode;
typedef struct _queueRecord QueueRecord;
typedef CircularNode* PtrToCircularNode;

typedef QueueRecord* Queue; // puntero a la cola

/**
 * @struct _circularNode
 * @brief Estructura de datos que representa un nodo de la cola
 */
struct _circularNode{
    Process* process; /*!< Proceso que contiene el nodo*/
    PtrToCircularNode next; /*!< Puntero a nodo siguiente*/
    PtrToCircularNode prev; /*!< Puntero a nodo anterior*/
};

/**
 * @struct _queueRecord
 * @brief Estructura de datos que representa la cola
 */
struct _queueRecord{
    PtrToCircularNode centinel; /*!< Next del centinela = front, prev=rear*/
};

bool is_empty_queue(Queue Q);
void make_empty_queue(Queue Q);
void free_queue(Queue Q);
void enqueue( Process X, Queue Q );
void dequeue( Queue Q );
void print_queue(Queue Q);
Process* front( Queue Q );
Queue create_queue();
Queue list_to_queue(List L);
#endif