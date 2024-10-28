/**
 * @file files.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Cabecera relacionadas con el manejo de archivos (files.c)
*/
#ifndef FILES_H
#define FILES_H

typedef struct _systemConfig SystemConfig;
typedef struct _systemStatus SystemStatus;

#include <getopt.h> // Para ingreso de parametros por terminal
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "errors.h"
#include "queue.h"
#include "buddySystem.h"
#include "utilities.h"

/*! \struct _systemConfig
 * @brief Estructura de datos que representa las espeficiaciones del "sistema".
*/
struct _systemConfig
{
	unsigned int totalMemory;/*!< Memoria total disponible*/
	unsigned int minMemory;/*!< Memoria minima a utilizar */
	unsigned int timeQuantum;/*!< Rafagas de tiempo para el algoritmo RoundRobin */
	unsigned int totalProcesses;/*!< Cantidad de procesos que se van a simular */
	List processes; /*!< Lista de procesos a simular */
	ptrToNode nextProcess; /*!< Puntero al siguiente proceso para procesar */
	Queue waitingQueue; /*!< Cola de espera de procesos */
	Queue arrivalQueue; /*!< Cola de llegada de procesos */
	Queue rrQueue; /*!< Cola de Round Robin */
	Queue sjfQueue; /*!< Cola de Shortest Job First */
	BuddySystem buddySystem; /*!< Estructura de datos que representa el "sistema" de buddies */
};

/*! \struct _systemStatus
 * @brief Estructura de datos que representa el estado del "sistema".
*/
struct _systemStatus
{
	unsigned int ticks; /*!< Numero de ticks que han transcurrido desde que se inicio el programa */
	unsigned int arrivalQueueNumber; /*!< Numero de procesos presentes en la cola de llegada */
	unsigned int waitingQueueNumber; /*!< Numero de procesos presentes en la cola de espera */
	unsigned int rrQueueNumber; /*!< Numero de procesos presentes en la cola de Round Robin */
	unsigned int sjfQueueNumber; /*!< Numero de procesos presentes en la cola de Shortest Job First */
	unsigned int avaliableMemory; /*!< Memoria disponible para procesar los procesos */
	unsigned int totalProceses; /*!< Numero de procesos que se van a simular */
	unsigned int remainingProceses; /*!< Numero de procesos que aún no han sido procesados */
	unsigned int remainingQuantumTime; /*!< Tiempo restante del quantum (Round Robin) */
};

int read_input_file(char* file_name, SystemConfig* forkingConfig);
void print_program(SystemConfig* config, SystemStatus status);
char* get_terminal_parameters(int argc, char* argv[], bool *clean);

#endif