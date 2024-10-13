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
    SystemStatus forkingStatus = {0, 0, 0, 0, 0, forkingConfig.totalMemory, forkingConfig.processes->data.PID, forkingConfig.processes->data.PID};

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
            enqueue(listPosition->data, arrivalQueue);
            delete_element(listPosition->data, forkingConfig.processes);
        }

        sleep(1);
        forkingStatus.ticks++;
    }

    return 0;
}
