/**
 * @file main.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funcion principal en donde se ejecuta y gestiona el programa
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include "queue.h"
#include "process.h"
#include "buddySystem.h"
#include "files.h"
#include "forking.h"
#include "errors.h"

int main(int argc, char* argv[]) {
	char* file_name; /*\var file_name Nombre del archivo de configuracion*/
	bool clean = false; /*\var clean Indica si se debe ejecutar el programa sin imprimir la simulacion*/
	/*Obtener parametros por terminal*/
	if(!(file_name = get_terminal_parameters(argc, argv, &clean))){
		return 1;
	}

	/*Inicialización de variables del programa*/
	SystemConfig forkingConfig;
	Queue arrivalQueue = create_queue();
	Queue waitingQueue = create_queue();
	Queue rrQueue = create_queue();
	Queue sjfQueue = create_queue();
	BuddySystem forkingBuddySystem;
	FILE* gant;

	/*Crear archivo de la carta gant*/
	if((gant = fopen("gant.csv", "w")) == NULL){
		print_error(101, "gant.csv", NULL);
		exit(-1);
	}
	fprintf(gant, "Tick,Process\n");

	if(!clean) printf(CLEAR_SCREEN ANSI_COLOR_RED"\t\tIniciando Forking...\n"ANSI_COLOR_RESET);

	// Se crea la lista de procesos y se inicializan las variables necesarias para su manejo
	if(read_input_file(file_name, &forkingConfig)){
		print_error(101, file_name , NULL); // No se ha podido leer el archivo
		return 1;
	}

	if(!clean)
	{
		// Se imprime la informacion del programa
		printf(CLEAR_SCREEN ANSI_COLOR_BLUE"\t\tInformacion recopilada:\n\n"ANSI_COLOR_RESET);
		printf("Total memoria: %u\n", forkingConfig.totalMemory);
		printf("Minima memoria: %u\n", forkingConfig.minMemory);
		printf("Nucleos del procesador: %u\n", forkingConfig.cpuCores);
		printf("Quantum para Round Robin: %u\n\n", forkingConfig.timeQuantum);
		printf(ANSI_COLOR_RED"\t\tOrdenando lista de procesos...\n"ANSI_COLOR_RESET);
		sleep(1);
	}

	// Se ordena el arrival_queue segun su arrival time
	merge_sort(forkingConfig.processes, get_process_arrival_time);

	if(!clean){
		printf(ANSI_COLOR_BLUE"\t\tLista de procesos...\n"ANSI_COLOR_RESET);
		print_list(forkingConfig.processes);
	}

	// Inicializacion config, status y buddySystem
	forkingConfig.totalProcesses = forkingConfig.processes->data.PID;

	SystemStatus forkingStatus = {0, 0, 0, 0, 0, forkingConfig.totalMemory, forkingConfig.totalProcesses, forkingConfig.totalProcesses, forkingConfig.timeQuantum};
	forkingBuddySystem = empty_buddy_system(NULL, forkingConfig);

	// Apuntamos los punteros de la configuracion a las colas y al BuddySystem
	forkingConfig.waitingQueue = waitingQueue;
	forkingConfig.arrivalQueue = arrivalQueue;
	forkingConfig.rrQueue = rrQueue;
	forkingConfig.sjfQueue = sjfQueue;
	forkingConfig.buddySystem = forkingBuddySystem;

	// inicio de la simulacion

	if(!clean){
		printf(ANSI_COLOR_RED"\n\t\tIniciando simulacion...\n"ANSI_COLOR_RESET);
		sleep(2);
	}

	while(forkingStatus.remainingProceses > 0)
	{
		// Intenta llevar los procesos en espera a su respectiva cola de procesamiento
		process_waiting_queue(&forkingConfig, &forkingStatus);

		// Llevar los procesos que corresponda al ArrivalQueue
		process_list_by_tick(&forkingConfig, &forkingStatus);

		if(!clean){
			// Imprimir Informacion del tick
			print_program(forkingConfig, forkingStatus, waitingQueue, arrivalQueue, rrQueue, sjfQueue);
		}

		// Manejar arrival_queue
		process_arrival_queue(&forkingConfig, &forkingStatus);

		// Procesamiento de los procesos en las colas de listo RR y SJF
		if(!is_empty_queue(sjfQueue)){ //Sjf
			process_sjf_queue(&forkingConfig, &forkingStatus, gant);
		}
		else if(!is_empty_queue(rrQueue)){ // RoundRobin
			process_rr_queue(&forkingConfig, &forkingStatus, gant);
		}
		forkingStatus.ticks++;

		if(!clean) sleep(1);
	}
	fclose(gant);

	if(!clean){
		// Imprimir Informacion final
		print_program(forkingConfig, forkingStatus, waitingQueue, arrivalQueue, rrQueue, sjfQueue);
	}
	printf("Proceso terminado luego de %u ticks\n\n", forkingStatus.ticks-1);

	// Se imprime la Carta Gantt con ayuda de Python
	if(forkingStatus.totalProceses > 25){
		printf(ANSI_COLOR_RED"Demasiados procesos para imprimir carta Gantt\n\n"ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN "\n\t\tGracias por usar Forking!\n\n"ANSI_COLOR_RESET);
		return 1;
	}
	printf(ANSI_COLOR_BLUE"\t\tImprimiendo carta Gantt..." ANSI_COLOR_RESET"\n");

	if(!fork()) // Creamos un proceso hijo para crear la Carta Gant :D
		execl("/usr/bin/python3", "python3", "gant_creator.py", NULL);
	wait(NULL);

	printf(ANSI_COLOR_CYAN "\n\t\tGracias por usar Forking!\n\n"ANSI_COLOR_RESET);

	return 0;
}
