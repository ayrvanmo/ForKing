/**
 * @file forking.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones principales de forKing
*/
#include "forking.h"


/**
 * @brief Procesa la cola de espera de procesos, intentando llevarlos a sus colas respectivas
 *
 * @param config Estructura encargada de contener informacion de configuracion, asi como punteros a estructuras importantes
 * @param status Estructura encargada de contener informacion de estado actual del sistema
*/
void process_waiting_queue(SystemConfig* config, SystemStatus* status)
{
    while(!is_empty_queue(config->waitingQueue) && (status->avaliableMemory >= front(config->waitingQueue)->memoryRequired) && insert_buddy(front(config->waitingQueue), config->buddySystem, *config, status)){
        // Se maneja el proceso para ingresarlo en la rr_queue o sjf
        if(front(config->waitingQueue)->burstTime > config->timeQuantum){
            enqueue(front(config->waitingQueue), config->rrQueue);
            status->rrQueueNumber++;
        }
        else{
            increasing_sorting_enqueue(front(config->waitingQueue), config->sjfQueue, get_process_burst_time);
            status->sjfQueueNumber++;
        }
        // Se elimina el proceso de la waiting queue
        dequeue(config->waitingQueue);
        status->waitingQueueNumber--;
    }
}

/**
 * @brief Revisa si hay procesos en la lista que lleguen al programa en el tick actual
 *
 * @param config Estructura encargada de contener informacion de configuracion, asi como punteros a estructuras importantes
 * @param status Estructura encargada de contener informacion de estado actual del sistema
*/
void process_list_by_tick(SystemConfig* config, SystemStatus* status){
    while(config->nextProcess && (config->nextProcess->data.arrivalTime == status->ticks)){
        // Funciona solo si la lista de procesos esta ordenada
        enqueue(&(config->nextProcess->data), config->arrivalQueue);
        status->arrivalQueueNumber++;
        config->nextProcess = config->nextProcess->next;
    }
}

/**
 * @brief Llevamos los procesos que lleguen a la arrivalqueue a las diferentes colas
 *
 * @param config Estructura encargada de contener informacion de configuracion, asi como punteros a estructuras importantes
 * @param status Estructura encargada de contener informacion de estado actual del sistema
*/
void process_arrival_queue(SystemConfig* config, SystemStatus* status){
    Process* auxProcessPtr;
    while(!is_empty_queue(config->arrivalQueue)){
        auxProcessPtr = front(config->arrivalQueue);
        if(auxProcessPtr->burstTime == 0){
            print_error(305, NULL, NULL);
            print_process(*auxProcessPtr);
            delete_element(*auxProcessPtr, config->processes);
        } // Asignacion de "memoria" y paso a waiting_queue
        else if(!insert_buddy(auxProcessPtr, config->buddySystem, *config, status)){
            increasing_sorting_enqueue(auxProcessPtr, config->waitingQueue, get_process_memory_required);
            status->waitingQueueNumber++;
        }
        // Asignacion de cola de procesamiento
        else if(auxProcessPtr->burstTime < config->timeQuantum){
            increasing_sorting_enqueue(auxProcessPtr, config->sjfQueue, get_process_burst_time);
            status->sjfQueueNumber++;
        }
        else{
            enqueue(auxProcessPtr, config->rrQueue);
            status->rrQueueNumber++;
        }
        //Se libera de arrival_queue
        dequeue(config->arrivalQueue);
        status->arrivalQueueNumber--;
    }
}

/**
 * @brief Procesamos un proceso de la sjfQueue
 *
 * @param config Estructura encargada de contener informacion de configuracion, asi como punteros a estructuras importantes
 * @param status Estructura encargada de contener informacion de estado actual del sistema
 * @param outputFile Archivo de salida
 * @warning El archivo de salida debe de estar anteriormente abierto
*/
void process_sjf_queue(SystemConfig* config, SystemStatus* status, FILE* outputFile){
    front(config->sjfQueue)->burstTime--; // Trabajo en el proceso

    if(fprintf(outputFile, "%d,%d\n", status->ticks, front(config->sjfQueue)->PID) < 0){
        print_error(201, NULL, NULL);
    }

    // Termino y liberacion de la memoria de un procso
    if(front(config->sjfQueue)->burstTime == 0){
        free_buddy(front(config->sjfQueue), config->buddySystem, *config, status);
        status->remainingProceses--;
        dequeue(config->sjfQueue);
        status->sjfQueueNumber--;
    }
}

/**
 * @brief Procesamos un proceso de la rrqueue
 *
 * @param config Estructura encargada de contener informacion de configuracion, asi como punteros a estructuras importantes
 * @param status Estructura encargada de contener informacion de estado actual del sistema
 * @param outputFile Archivo de salida
 * @warning El archivo de salida debe de estar anteriormente abierto
 * @note Si un proceso luego de ser procesado termina con el quantum y el tiempo restante es menor que el quantum, se lleva a la cola de sjf
*/
void process_rr_queue(SystemConfig* config, SystemStatus* status, FILE* outputFile){
    Process* auxProcessPtr;

    //printf("ACTUAL QUANTUM TIME: %u\n", status->remainingQuantumTime);

    status->remainingQuantumTime--;
    front(config->rrQueue)->burstTime--; // Trabajo en el proceso

    if(fprintf(outputFile, "%d,%d\n", status->ticks, front(config->rrQueue)->PID) < 0){
        print_error(201, NULL, NULL);
    }

    // Termino y liberacion de memoria de un proceso
    if(front(config->rrQueue)->burstTime == 0){
        free_buddy(front(config->rrQueue), config->buddySystem, *config, status);
        status->remainingProceses--;
        status->rrQueueNumber--;
        // Reiniciar quantum
        status->remainingQuantumTime = config->timeQuantum;
        dequeue(config->rrQueue);
    }

    // Termino de quantum
    if(status->remainingQuantumTime == 0){
        auxProcessPtr = front(config->rrQueue);
        //Si el procceso no ha terminado se ingresa nuevamente a la cola correspondiente
        if(auxProcessPtr->burstTime != 0){
            if(auxProcessPtr->burstTime < config->timeQuantum){
                increasing_sorting_enqueue(auxProcessPtr, config->sjfQueue, get_process_burst_time);
                status->sjfQueueNumber++;
                status->rrQueueNumber--;
            }
            else{
                enqueue(auxProcessPtr, config->rrQueue);
            }
            status->remainingQuantumTime = config->timeQuantum;
            dequeue(config->rrQueue);
        }
    }
}