/**
 * @file main.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funcion principal en donde se ejecuta y gestiona el programa
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "queue.h"
#include "process.h"
#include "buddySystem.h"
#include "files.h"
#include "errors.h"

/* Se usa una funcion llamada forkingConfig declarada en el archivo files.h */

int main(int argc, char* argv[]) {

	char* file_name;
	if(!(file_name = get_terminal_parameters(argc, argv))){
		print_error(100, NULL, NULL);
		return 1;
	}

	/*Comienzo de ejecución del programa*/

	SystemConfig forkingConfig;
	// Creacion de colas
	Queue arrivalQueue = create_queue(); // Es una lista pero esta utilizada como uan cola por comodidad
	Queue waitingQueue = create_queue();
	Queue rrQueue = create_queue();
	Queue sjfQueue = create_queue();
	BuddySystem forkingBuddySystem;
	Process* auxProcessPtr;
	Process auxProcess;
	FILE* gant;

	if((gant = fopen("gant.csv", "w")) == NULL){
		print_error(101, "gant.csv", NULL);
		exit(-1);
	}
	fprintf(gant, "Tick,Process\n");

	if(system("clear")){
		print_error(202,NULL,NULL);
	}
	printf(ANSI_COLOR_RED"\t\tIniciando Forking...\n"ANSI_COLOR_RESET);

	// Se crea la lista de procesos y se inicializan las variables necesarias para su manejo
	if(read_input_file(file_name, &forkingConfig)){
		print_error(101, file_name , NULL); // No se ha podido leer el archivo
		return 1;
	}
	if(system("clear")){
		print_error(202,NULL,NULL);
	}
	printf(ANSI_COLOR_BLUE"\t\tInformacion recopilada:\n\n"ANSI_COLOR_RESET);
	printf("Total memoria: %u\n", forkingConfig.totalMemory);
	printf("Minima memoria: %u\n", forkingConfig.minMemory);
	printf("Nucleos del procesador: %u\n", forkingConfig.cpuCores);
	printf("Quantum para Round Robin: %u\n\n", forkingConfig.timeQuantum);
	printf(ANSI_COLOR_RED"\t\tOrdenando lista de procesos...\n"ANSI_COLOR_RESET);
	sleep(1);

	// Se ordena el arrival_queue segun su arrival time
	merge_sort(forkingConfig.processes, get_process_arrival_time);
	printf(ANSI_COLOR_BLUE"\t\tLista de procesos...\n"ANSI_COLOR_RESET);
	print_list(forkingConfig.processes);
	forkingConfig.totalProcesses = forkingConfig.processes->data.PID;
	// Inicializacion status
	SystemStatus forkingStatus = {0, 0, 0, 0, 0, forkingConfig.totalMemory, forkingConfig.processes->data.PID, forkingConfig.processes->data.PID, forkingConfig.timeQuantum};

	// Creamos el Buddy system
	forkingBuddySystem = empty_buddy_system(NULL, forkingConfig);

	printf(ANSI_COLOR_RED"\n\t\tIniciando simulacion...\n"ANSI_COLOR_RESET);
	sleep(2);
	// inicio de la simulacion
	print_buddy_system(forkingBuddySystem);
	while(forkingStatus.remainingProceses > 0)
	{
		// Intenta llevar los procesos en espera a su respectiva cola de procesamiento
		while(!is_empty_queue(waitingQueue) && (forkingStatus.avaliableMemory >= front(waitingQueue)->memoryRequired) && insert_buddy(front(waitingQueue), forkingBuddySystem, forkingConfig, &forkingStatus)){
			// Se maneja el proceso para ingresarlo en la rr_queue o sjf
			if(front(waitingQueue)->burstTime > forkingConfig.timeQuantum){
				enqueue(front(waitingQueue), rrQueue);
				forkingStatus.rrQueueNumber++;
			}
			else{
				increasing_sorting_enqueue(front(waitingQueue), sjfQueue, get_process_burst_time);
				forkingStatus.sjfQueueNumber++;
			}
			// Se elimina el proceso de la waiting queue
			dequeue(waitingQueue);
			forkingStatus.waitingQueueNumber--;
		}
		// Llevar los procesos que corresponda al ArrivalQueue
		unsigned int auxPID = first(forkingConfig.processes)->data.PID;
		bool workedAuxPID = 0;
		while(first(forkingConfig.processes) && (first(forkingConfig.processes)->data.arrivalTime == forkingStatus.ticks)){ // Funciona solo si la lista de procesos esta ordenada

			// Para evitar caso en que entran solo procesos con el mismo arrival
			if(workedAuxPID && first(forkingConfig.processes)->data.PID == auxPID){
				break;
			}
			else{
				workedAuxPID = 1;
			}

			auxProcess = first(forkingConfig.processes)->data;
			delete_element(first(forkingConfig.processes)->data, forkingConfig.processes);
			auxProcessPtr = &insert_element_end(auxProcess, forkingConfig.processes)->data;
			enqueue(auxProcessPtr, arrivalQueue);
			forkingStatus.arrivalQueueNumber++;
		}

		// Imprimir Informacion del tick
		print_program(forkingConfig, forkingStatus, waitingQueue, arrivalQueue, rrQueue, sjfQueue);

		// Manejar arrival_queue
		while(!is_empty_queue(arrivalQueue)){
			auxProcessPtr = front(arrivalQueue);
			if(auxProcessPtr->burstTime == 0){
				print_error(305, NULL, NULL);
				print_process(*auxProcessPtr);
				delete_element(*auxProcessPtr, forkingConfig.processes);
			} // Asignacion de "memoria" y paso a waiting_queue
			else if(!insert_buddy(auxProcessPtr, forkingBuddySystem, forkingConfig, &forkingStatus)){
				increasing_sorting_enqueue(auxProcessPtr, waitingQueue, get_process_memory_required);
				forkingStatus.waitingQueueNumber++;
			}
			// Asignacion de cola de procesamiento
			else if(auxProcessPtr->burstTime < forkingConfig.timeQuantum){
				increasing_sorting_enqueue(auxProcessPtr, sjfQueue, get_process_burst_time);
				forkingStatus.sjfQueueNumber++;
			}
			else{
				enqueue(auxProcessPtr, rrQueue);
				forkingStatus.rrQueueNumber++;
			}
			//Se libera de arrival_queue
			dequeue(arrivalQueue);
			forkingStatus.arrivalQueueNumber--;
		}
		// Procesamiento de los procesos en las colas de listo RR y SJF
		if(!is_empty_queue(sjfQueue)){ //Sjf
			front(sjfQueue)->burstTime--; // Trabajo en el proceso
			fprintf(gant, "%d,%d\n",forkingStatus.ticks, front(sjfQueue)->PID);

			// Termino y liberacion de la memoria de un procso
			if(front(sjfQueue)->burstTime == 0){
				free_buddy(front(sjfQueue), forkingBuddySystem, forkingConfig, &forkingStatus);
				forkingStatus.remainingProceses--;
				dequeue(sjfQueue);
				forkingStatus.sjfQueueNumber--;
			}
		}
		else if(!is_empty_queue(rrQueue)){ // RoundRobin
			//printf("ACTUAL QUANTUM TIME: %u\n", forkingStatus.remainingQuantumTime);
			forkingStatus.remainingQuantumTime--;
			front(rrQueue)->burstTime--; // Trabajo en el proceso
			fprintf(gant, "%d,%d\n",forkingStatus.ticks, front(rrQueue)->PID);
			// Termino y liberacion de memoria de un proceso
			if(front(rrQueue)->burstTime == 0){
					free_buddy(front(rrQueue), forkingBuddySystem, forkingConfig, &forkingStatus);
					forkingStatus.remainingProceses--;
					forkingStatus.rrQueueNumber--;
					// Reiniciar quantum
					forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
					dequeue(rrQueue);
			}

			// Termino de quantum
			if(forkingStatus.remainingQuantumTime == 0){
				auxProcessPtr = front(rrQueue);
				//Si el procceso no ha terminado se ingresa nuevamente a la cola correspondiente
				if(auxProcessPtr->burstTime != 0){
					if(auxProcessPtr->burstTime < forkingConfig.timeQuantum){
						increasing_sorting_enqueue(auxProcessPtr, sjfQueue, get_process_burst_time);
						forkingStatus.sjfQueueNumber++;
						forkingStatus.rrQueueNumber--;
					}
					else{
						enqueue(auxProcessPtr, rrQueue);
					}
					forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
					dequeue(rrQueue);
				}
			}
		}
		forkingStatus.ticks++;
		sleep(1);
	}

	// Imprimir Informacion final
	print_program(forkingConfig, forkingStatus, waitingQueue, arrivalQueue, rrQueue, sjfQueue);
	printf("Proceso terminado luego de %u ticks\n\n", forkingStatus.ticks-1);
	fclose(gant);

	if(forkingStatus.totalProceses > 25){
		printf(ANSI_COLOR_RED"Demasiados procesos para imprimir carta Gantt\n\n"ANSI_COLOR_RESET);
		return 1;
	}
	printf(ANSI_COLOR_BLUE"\t\tImprimiendo carta Gantt..." ANSI_COLOR_RESET"\n");
	execl("/usr/bin/python3", "python3", "gant_creator.py", NULL);

	return 0;
}
