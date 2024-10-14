#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "process.h"
#include "buddySystem.h"
#include "files.h"

/* Se usa una funcion llamada forkingConfig declarada en el archivo files.h */

int main(int argc, char* argv[]) {
    SystemConfig forkingConfig;

    // Creacion de colas
    Queue arrivalQueue = create_queue();
    Queue waitingQueue = create_queue();
    Queue rrQueue = create_queue();
    Queue sjfQueue = create_queue();
    BuddySystem forkingBuddySystem;
    Position listPosition;
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
    sort_list(forkingConfig.processes, get_process_arrival_time);
    print_list(forkingConfig.processes);
    SystemStatus forkingStatus = {0, 0, 0, 0, 0, forkingConfig.totalMemory, forkingConfig.processes->data.PID, forkingConfig.processes->data.PID, forkingConfig.timeQuantum};

    // Creamos el Buddy system
    forkingBuddySystem = empty_buddy_system(NULL, forkingConfig);

    // inicio de la simulacion
    while(forkingStatus.remainingProceses > 0)
    {
        printf("Ticks: %u\n", forkingStatus.ticks);

        while(!is_empty_queue(waitingQueue) && (forkingStatus.avaliableMemory >= front(waitingQueue)->memoryRequired) && insert_buddy(front(waitingQueue), forkingBuddySystem, forkingConfig, &forkingStatus)){
            if(front(waitingQueue)->burstTime > forkingConfig.timeQuantum){
                enqueue(front(waitingQueue), rrQueue);
                forkingStatus.rrQueueNumber++;
            }
            else{
                enqueue(front(waitingQueue), sjfQueue);
                forkingStatus.sjfQueueNumber++;
            }
            dequeue(waitingQueue);
            forkingStatus.waitingQueueNumber--;
        }

        // Llevar los procesos que corresponda al ArrivalQueue
        while(first(forkingConfig.processes) && (first(forkingConfig.processes)->data.arrivalTime == forkingStatus.ticks)){ // Funciona solo si la lista de procesos esta ordenada
            auxProcess = first(forkingConfig.processes)->data;
            delete_element(first(forkingConfig.processes)->data, forkingConfig.processes);
            auxProcessPtr = insert_element_end(auxProcess, forkingConfig.processes);
            enqueue(auxProcessPtr, arrivalQueue);
            forkingStatus.arrivalQueueNumber++;
        }
/*         printf("Luego de sacar los procesos de la lista de procesos:\n");
        print_queue(arrivalQueue);
        printf("\n\n\n"); */

        // Liberar el ArrivalQueue
        while(!is_empty_queue(arrivalQueue)){
            auxProcessPtr = front(arrivalQueue);

            if(!insert_buddy(auxProcessPtr, forkingBuddySystem, forkingConfig, &forkingStatus)){
                enqueue(auxProcessPtr, waitingQueue);
                forkingStatus.waitingQueueNumber++;
            }
            else if(auxProcessPtr->burstTime > forkingConfig.timeQuantum){
                enqueue(auxProcessPtr, rrQueue);
                forkingStatus.rrQueueNumber++;
            }
            else{
                enqueue(auxProcessPtr, sjfQueue);
                forkingStatus.sjfQueueNumber++;
            }
            dequeue(arrivalQueue);
            forkingStatus.arrivalQueueNumber--;
        }


        // Procesamiento de los procesos en las colas de listo RR y SJF
        if(!is_empty_queue(sjfQueue)){ //Sjf
            printf("Burst del SJF actual es: %u\n",front(sjfQueue)->burstTime);
            front(sjfQueue)->burstTime--; // Trabajo en el proceso
            if(front(sjfQueue)->burstTime == 0){
                free_buddy(front(sjfQueue), forkingBuddySystem, forkingConfig, &forkingStatus);
                forkingStatus.remainingProceses--;
                dequeue(sjfQueue);
                forkingStatus.sjfQueueNumber--;
            }
        }
        else if(!is_empty_queue(rrQueue)){ // RoundRobin
            printf("ACTUAL QUANTUM TIME: %u\n", forkingStatus.remainingQuantumTime);
            front(rrQueue)->burstTime--; // Trabajo en el proceso
            forkingStatus.remainingQuantumTime--;
            if(front(rrQueue)->burstTime == 0){ // Proceso terminado
                printf("Burst 0\n");
                free_buddy(front(rrQueue), forkingBuddySystem, forkingConfig, &forkingStatus);
                forkingStatus.remainingProceses--;
                dequeue(rrQueue);
                forkingStatus.rrQueueNumber--;
                forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
            }
            else if(forkingStatus.remainingQuantumTime == 0){
                forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
                if(front(rrQueue)->burstTime < forkingConfig.timeQuantum){
                    auxProcessPtr = front(rrQueue);
                    dequeue(rrQueue);
                    forkingStatus.rrQueueNumber--;
                    enqueue(auxProcessPtr, sjfQueue);
                    forkingStatus.sjfQueueNumber++;
                    forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
                }
                else{
                    auxProcessPtr = front(rrQueue);
                    dequeue(rrQueue);
                    enqueue(auxProcessPtr, rrQueue); //Como hay un dequeue y un enqueue a la vez, no se altera la cantidad en cola
                    forkingStatus.rrQueueNumber++;
                    forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
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
