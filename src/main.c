#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "process.h"
#include "buddySystem.h"
#include "files.h"

/* Se usa una funcion llamada forkingConfig declarada en el archivo files.h */

int main(int argc, char* argv[]) {
    // Comprobamos que se haya pasado un archivo
    if (argc < 2) {
        printf("No se ha pasado un archivo\n");
        return 1;
    }

    // Se crea la lista de procesos y se inicializan las variables necesarias para su manejo
    if(read_input_file(argv[1])){
        printf("No se ha podido leer el archivo\n");
        return 1;
    }

    // Se imprime la lista de procesos
    print_list(forkingConfig.processes);


    return 0;
}
