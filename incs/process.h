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
int is_last(Position P);
void print_list(List L);
void print_process(Process P);
void delete_list(List L);
void swap_proceses(Position a, Position b);
Position find_process(Process X, List);
Position find_previous(Process X, List L);
Position advance(Position P);
Process retrieve(Position P);
Position delete_element(Process X, List L);
Position insert_element(Process X, List L, Position P);
Position insert_element_end(Process X, List L);
List make_empty_list(List L);
List header(List L);
List first(List L);
List sort_list(List L, unsigned int(*criterion)(Process));
List load_process_list(FILE *file);
unsigned int get_process_pid(Process P);
unsigned int get_process_arrival_time(Process P);
unsigned int get_process_burst_time(Process P);
unsigned int get_process_memory_required(Process P);

#endif