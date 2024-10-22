/**
 * @file generator.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Genera un archivo de entrada para el programa
 *
 * Se le pasa por parametro un numero n que corresponde al numero de procesos que se generaran
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Colores para texto
#define CLEAR_SCREEN "\033[H\033[J"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"

int main(int argc, char* argv[]){

    int values[3];

    srand(time(NULL));
    int numProcesses = atoi(argv[1]);

    if (argc < 2 || numProcesses < 1) {
        printf(ANSI_COLOR_RED "Fatal Error 204:" ANSI_COLOR_RESET);
        printf("Cantidad invalida, la cantidad ingresada de be ser mayor a 0\n");
        return 1;
    }

    FILE *outputInfo;
    if((outputInfo = fopen("input.txt", "w")) == NULL){
        printf("No se ha podido abrir el archivo de salida\n");
        return 1;
    }

    /*Imprimir valores para
        - Total de memoria
        - Memoria minima
        - Numero de nucleos
        - Quantum
    */
   fprintf(outputInfo, "%u %u %u %u\n\n", 5000, 2, 4, 5);

    // Imprimir el archivo de entrada
    for(int i = 0; i < numProcesses; i++){
        values[0] = rand() % 15;
        values[1] = (rand() % 7)+1;
        values[2] = (rand() % (5000 -2 +1)) + 2;
        fprintf(outputInfo, "%u %u %u\n", values[0], values[1], values[2]);
    }

    return 0;
}
