/**
 * @file files.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones relacionadas con el manejo de archivos
*/
#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "process.h"

/*! \struct System
 * @brief Estructura de datos que representa las espeficiaciones del "sistema".
*/
struct System
{
    unsigned int totalMemory;/*!< Memoria total disponible*/
    unsigned int minMemory;/*!< Memoria minima a utilizar */
    unsigned int cpuCores;/*!< Cantidad de "nucleos" del "procesador" */
    unsigned int timeQuantum;/*!< Rafagas de tiempo para el algoritmo RoundRobin */
    List processes;
};

extern struct System forkingConfig;

int read_input_file(char* file_name);

#endif