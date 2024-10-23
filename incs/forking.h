/**
 * @file forking.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Cabecera de las funciones principales de Foking
*/
#ifndef FORKING_H
#define FORKING_H

#include "files.h"
#include "queue.h"
#include "process.h"
#include "buddySystem.h"

void process_waiting_queue(SystemConfig* config, SystemStatus* status);
void process_list_by_tick(SystemConfig* config, SystemStatus* status);
void process_arrival_queue(SystemConfig* config, SystemStatus* status);
void process_rr_queue(SystemConfig* config, SystemStatus* status, FILE* outputFile);
void process_sjf_queue(SystemConfig* config, SystemStatus* status, FILE* outputFile);

#endif