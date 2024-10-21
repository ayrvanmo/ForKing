/**
 * @file files.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones relacionadas con el manejo de archivos
*/
#ifndef FILES_H
#define FILES_H

#include <getopt.h> // Para ingreso de parametros por terminal
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "errors.h"
#include "queue.h"
#include "utilities.h"

/*! \struct System
 * @brief Estructura de datos que representa las espeficiaciones del "sistema".
*/
typedef struct SystemConfig
{
	unsigned int totalMemory;/*!< Memoria total disponible*/
	unsigned int minMemory;/*!< Memoria minima a utilizar */
	unsigned int cpuCores;/*!< Cantidad de "nucleos" del "procesador" */
	unsigned int timeQuantum;/*!< Rafagas de tiempo para el algoritmo RoundRobin */
	unsigned int totalProcesses;/*!< Cantidad de procesos que se van a simular */
	List processes;
} SystemConfig;


typedef struct SystemStatus
{
	unsigned int ticks;
	unsigned int arrivalQueueNumber;
	unsigned int waitingQueueNumber;
	unsigned int rrQueueNumber;
	unsigned int sjfQueueNumber;
	unsigned int avaliableMemory;
	unsigned int totalProceses;
	unsigned int remainingProceses;
	unsigned int remainingQuantumTime;
} SystemStatus;

int read_input_file(char* file_name, SystemConfig* forkingConfig);
void print_program(SystemConfig config, SystemStatus status, Queue waitingQueue, Queue arrivalQueue, Queue rrQueue, Queue sjfQueue);

char* get_terminal_parameters(int argc, char* argv[], bool *clean);

#endif