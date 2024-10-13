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
    BuddySystem forkingBuddySystem = empty_buddy_system(NULL);
    Position listPosition;
    Process auxProcess;
    Process* auxProcessPointer;

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

    // inicio de la simulacion
    while(forkingStatus.remainingProceses > 0)
    {
        printf("Ticks: %u\n", forkingStatus.ticks);

        // Llevar los procesos que corresponda al ArrivalQueue
        while(1){ // Funciona solo si la lista de procesos esta ordenada
            listPosition = first(forkingConfig.processes);
            if(!listPosition || (listPosition->data.arrivalTime != forkingStatus.ticks)){
                break;
            }
            enqueue(&listPosition->data, arrivalQueue);
            auxProcess = first(forkingConfig.processes)->data;
            delete_element(listPosition->data, forkingConfig.processes);
            insert_element_end(auxProcess, forkingConfig.processes);
        }

        // Llevamos los procesos en espera de nuevo al Arrival con la esperanza de que sean procesados
        /* while(!is_empty_queue(waitingQueue))
        {
            auxProcessPointer = front(waitingQueue);
            dequeue(waitingQueue);
            enqueue(auxProcessPointer, arrivalQueue);
        } */

        // Liberar el ArrivalQueue
        while(!is_empty_queue(arrivalQueue)){
            auxProcessPointer = front(arrivalQueue);
            if(!insert_buddy(auxProcessPointer, forkingBuddySystem)){
                enqueue(auxProcessPointer, waitingQueue);
            }
            else if(auxProcessPointer->burstTime > forkingConfig.timeQuantum){
                enqueue(auxProcessPointer, rrQueue);
            }
            else{
                enqueue(auxProcessPointer, sjfQueue);
            }
            dequeue(arrivalQueue);
        }


        // Procesamiento de los procesos en las colas de listo RR y SJF
        if(!is_empty_queue(sjfQueue)){ //Sjf
            front(sjfQueue)->burstTime--; // Trabajo en el proceso
            if(front(sjfQueue)->burstTime == 0){
                free_buddy(front(sjfQueue), forkingBuddySystem);
                forkingStatus.remainingProceses--;
                dequeue(sjfQueue);
            }
        }
        else if(!is_empty_queue(rrQueue)){ // RoundRobin
            printf("QUANTUM TIME: %u\n", forkingStatus.remainingQuantumTime);
            if(forkingStatus.remainingQuantumTime > 0){
                front(rrQueue)->burstTime--; // Trabajo en el proceso
                forkingStatus.remainingQuantumTime--;
                if(front(rrQueue)->burstTime == 0){ // Proceso terminado
                    free_buddy(front(rrQueue), forkingBuddySystem);
                    forkingStatus.remainingProceses--;
                    dequeue(rrQueue);
                }
            }
            else if(front(rrQueue)->burstTime < forkingConfig.timeQuantum){
                auxProcessPointer = front(rrQueue);
                dequeue(rrQueue);
                enqueue(auxProcessPointer, sjfQueue);
                forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
            }
            else{
                auxProcessPointer = front(rrQueue);
                dequeue(rrQueue);
                enqueue(auxProcessPointer, rrQueue);
                forkingStatus.remainingQuantumTime = forkingConfig.timeQuantum;
            }
        }

        // Imprimir TODOOOO
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

    printf("Proceso terminado luego de %u ticks\n\n", forkingStatus.ticks);

    return 0;
}
