/**
 * @file queue.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Cabecera relacionada con colas (queue.c)
 */

#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct _circularNode CircularNode;
typedef struct _queueRecord QueueRecord;
typedef CircularNode* PtrToCircularNode;
typedef QueueRecord* Queue; // puntero a la cola

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errors.h"
#include "process.h"

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
void enqueue( Process* X, Queue Q );
bool decreasing(unsigned int(*criterion)(Process), Process A, Process B);
bool increasing( unsigned int(*criterion)(Process), Process A, Process B);
void sorting_enqueue(Process *P, Queue Q, unsigned int(*criterion)(Process), bool (*order)(unsigned int(*)(Process), Process, Process));
void dequeue( Queue Q );
void print_queue(Queue Q);
Queue create_queue();
Queue list_to_queue(List L);
Process* front( Queue Q );



#endif