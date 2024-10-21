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

int main(int argc, char* argv[]){

    int values[4];

    srand(time(NULL));

    if(argc < 2){
        printf("No se ha pasado un numero de procesos\n");
        return 1;
    }

    int numProcesses = atoi(argv[1]);

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
        values[0] = i;
        values[1] = rand() % 15;
        values[2] = (rand() % 4)+1;
        values[3] = (rand() % (5000 -2 +1)) + 2;
        fprintf(outputInfo, "%u %u %u %u\n", values[0], values[1], values[2], values[3]);
    }

    return 0;
}