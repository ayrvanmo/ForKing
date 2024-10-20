/**
 * @file main.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funcion principal en donde se ejecuta y gestiona el programa
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "process.h"
#include "buddySystem.h"
#include "files.h"
#include "errors.h"

/* Se usa una funcion llamada forkingConfig declarada en el archivo files.h */

int main(int argc, char* argv[]) {

	SystemConfig forkingConfig;
	// Creacion de colas
	Queue arrivalQueue = create_queue(); // Es una lista pero esta utilizada como uan cola por comodidad
	Queue waitingQueue = create_queue();
	Queue rrQueue = create_queue();
	Queue sjfQueue = create_queue();
	BuddySystem forkingBuddySystem;
	Process* auxProcessPtr;
	Process auxProcess;

	// Se comprueba si se ha pasado un archivo
	if (argc < 2) {
		printf("No se ha pasado un archivo\n");
		return 1;
	}

	// Se crea la lista de procesos y se inicializan las variables necesarias para su manejo
	if(read_input_file(argv[1], &forkingConfig)){
		printf("No se ha podido leer el archivo\n");
		return 1;
	}
	// Se ordena el arrival_queue segun su arrival time
	//bubble_sort(forkingConfig.processes, get_process_arrival_time);
	merge_sort(forkingConfig.processes, get_process_arrival_time);
	print_list(forkingConfig.processes);
	sleep(10);
	// Inicializacion status
	SystemStatus forkingStatus = {0, 0, 0, 0, 0, forkingConfig.totalMemory, forkingConfig.processes->data.PID, forkingConfig.processes->data.PID, forkingConfig.timeQuantum};

	// Creamos el Buddy system
	forkingBuddySystem = empty_buddy_system(NULL, forkingConfig);

	// inicio de la simulacion
	while(forkingStatus.remainingProceses > 0)
	{
		printf("Ticks: %u\n", forkingStatus.ticks);
		// Intneta llevar los procesos en espera a su respectiva cola de procesamiento
		while(!is_empty_queue(waitingQueue) && (forkingStatus.avaliableMemory >= front(waitingQueue)->memoryRequired) && insert_buddy(front(waitingQueue), forkingBuddySystem, forkingConfig, &forkingStatus)){
			// Se maneja el proceso para ingresarlo en la rr_queue o sjf
			if(front(waitingQueue)->burstTime > forkingConfig.timeQuantum){
				enqueue(front(waitingQueue), rrQueue);
				forkingStatus.rrQueueNumber++;
			}
			else{
				enqueue(front(waitingQueue), sjfQueue);
				forkingStatus.sjfQueueNumber++;
			}
			// Se elimina el proceso de la waiting queue
			dequeue(waitingQueue);
			forkingStatus.waitingQueueNumber--;
		}
		// Llevar los procesos que corresponda al ArrivalQueue
		while(first(forkingConfig.processes) && (first(forkingConfig.processes)->data.arrivalTime == forkingStatus.ticks)){ // Funciona solo si la lista de procesos esta ordenada
			auxProcess = first(forkingConfig.processes)->data;
			delete_element(first(forkingConfig.processes)->data, forkingConfig.processes);
			auxProcessPtr = &insert_element_end(auxProcess, forkingConfig.processes)->data;
			enqueue(auxProcessPtr, arrivalQueue);
			forkingStatus.arrivalQueueNumber++;
		}
		// Manejar arrival_queue
		while(!is_empty_queue(arrivalQueue)){
			auxProcessPtr = front(arrivalQueue);
			// Asignacion de "memoria" y paso a waiting_queue
			if(!insert_buddy(auxProcessPtr, forkingBuddySystem, forkingConfig, &forkingStatus)){
				enqueue(auxProcessPtr, waitingQueue);
				forkingStatus.waitingQueueNumber++;
			}
			// Asignacion de cola de procesamiento
			else if(auxProcessPtr->burstTime < forkingConfig.timeQuantum){
				enqueue(auxProcessPtr, sjfQueue);
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
			printf("Burst del SJF actual es: %u\n",front(sjfQueue)->burstTime);
			front(sjfQueue)->burstTime--; // Trabajo en el proceso
			// Termino y liberacion de la memoria de un procso
			if(front(sjfQueue)->burstTime == 0){
				free_buddy(front(sjfQueue), forkingBuddySystem, forkingConfig, &forkingStatus);
				forkingStatus.remainingProceses--;
				dequeue(sjfQueue);
				forkingStatus.sjfQueueNumber--;
			}
		}
		else if(!is_empty_queue(rrQueue)){ // RoundRobin
			printf("ACTUAL QUANTUM TIME: %u\n", forkingStatus.remainingQuantumTime);
			forkingStatus.remainingQuantumTime--;
			front(rrQueue)->burstTime--; // Trabajo en el proceso
			// Termino y liberacion de memoria de un proceso
			if(front(rrQueue)->burstTime == 0){
					printf("Burst 0\n");
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
						enqueue(auxProcessPtr, sjfQueue);
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

		// Imprimir TODOOOO
		// printf("Espacio disponible %u, Cola Arrival %u, Cola Waiting %u, Cola rr %u Colaa SJF %u\n",forkingStatus.avaliableMemory, forkingStatus.arrivalQueueNumber, forkingStatus.waitingQueueNumber, forkingStatus.rrQueueNumber, forkingStatus.sjfQueueNumber);
		printf("------------------ ArrivalQueue ------------------\n");
		print_queue(arrivalQueue);
		printf("------------------ WaitingQueue ------------------\n");
		print_queue(waitingQueue);
		printf("------------------ RRQueue ------------------\n");
		print_queue(rrQueue);
		printf("------------------ SJFQueue ------------------\n");
		print_queue(sjfQueue);
		//printf("------------------ BuddySystem ------------------\n");
		//print_buddy_system(forkingBuddySystem);
		printf("\n\n\n\n");

		sleep(1);
		forkingStatus.ticks++;
	}

	printf("Proceso terminado luego de %u ticks\n\n", forkingStatus.ticks-1);

	return 0;
}
