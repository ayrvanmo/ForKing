/**
 * @file process.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Cabezera relacionada a process.c
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*! \struct _process
 * @brief Estructura de datos que contiene la informacion de un proceso
*/
typedef struct _process{
	unsigned int PID;/*!< Identificador del Proceso */
	unsigned int arrivalTime;/*!< Tiempo de llegada del proceso */
	unsigned int burstTime;/*!< Tiempo requerido por el proceso */
	unsigned int memoryRequired;/*!< Memoria solicitada por el proceso */
}Process;


typedef struct _processNode ProcessNode;
typedef ProcessNode *ptrToNode;
typedef ptrToNode Position;
typedef ptrToNode List;

/*! \struct _processNode
 * @brief Estructura de datos representa el Nodo de un proceso en una lista de procesos
*/

struct _processNode {
    Process data; /*!< Parámetros del proceso */
    Position next; /*!< Puntero al siguiente nodo en la lista enlazada */
};

int is_empty_list(List L);
void print_list(List L);
void delete_list(List L);
int is_last(Position P);
List make_empty_list(List L);
Position find(Process X, List);
Position find_previous(Process X, List L);
List delete_element(Process X, List L);
List insert_element(Process X, List L, Position P);
List header(List L);
List first(List L);
Position advance(Position P);
Process retrieve(Position P	);

#endif