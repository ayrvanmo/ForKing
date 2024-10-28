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
#include <unistd.h>

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
    /*
        0: Tiempo de llegada
        1: burst time
        2: Memoria requerida
    */
   int parameters[8];
    /*
        0: Memoria total
        1: Memoria minima
        2: Quantum
        3: Numero de procesos
        4: Maximo de tiempo de llegada
        5: Maximo de burst time
        6: Maximo de memoria requerida
        7: Minimo de memoria requerida
    */
    srand(time(NULL));

    printf("Ingrese el numero de procesos a generar: \n");
    scanf("%d", &parameters[3]);
    while(parameters[3] <= 0){
        printf("Ingrese un numero valido de procesos a generar (mayor a 0): \n");
        scanf("%d", &parameters[3]);
    }

    printf("Ingrese la memoria total: \n");
    scanf("%d", &parameters[0]);
    while(parameters[0] <= 1){
        printf("Ingrese un numero valido de memoria total (mayor a 1): \n");
        scanf("%d", &parameters[0]);
    }

    printf("Ingrese la memoria minima: \n");
    scanf("%d", &parameters[1]);
    while(parameters[1] <= 1 || parameters[1] > parameters[0]){
        printf("Ingrese un numero valido de memoria minima (mayor a 1) y menor o igual al total de memoria: %u\n", parameters[0]);
        scanf("%d", &parameters[1]);
    }

    printf("Ingrese el Quantum: \n");
    scanf("%d", &parameters[2]);
    while(parameters[2] <= 1){
        printf("Ingrese un numero valido de Quantum (mayor a 1): \n");
        scanf("%d", &parameters[2]);
    }

    FILE *outputInfo;
    if(argc >= 2){
        if((outputInfo = fopen(argv[1], "w")) == NULL){
            printf("No se ha podido abrir el archivo de salida\n");
            return 1;
        }
    }
    else{
        if((outputInfo = fopen("input.txt", "w")) == NULL){
            printf("No se ha podido abrir el archivo de salida\n");
            return 1;
        }
    }

    printf("Ingrese el maximo de tiempo de llegada: \n");
    scanf("%d", &parameters[4]);
    while(parameters[4] < 0){
        printf("Ingrese un numero valido de tiempo de llegada (mayor o igual a 0): \n");
        scanf("%d", &parameters[4]);
    }

    printf("Ingrese el maximo de burst time: \n");
    scanf("%d", &parameters[5]);
    while(parameters[5] <= 0){
        printf("Ingrese un numero valido de burst time (mayor a 0): \n");
        scanf("%d", &parameters[5]);
    }

    printf("Ingrese el maximo de memoria requerida: \n");
    scanf("%d", &parameters[6]);
    while(parameters[6] <= 0 || parameters[6] > parameters[0] || parameters[6] < parameters[1]){
        printf("Ingrese un numero valido de memoria requerida (mayor a la memoria minima %u y menor o igual al total de memoria %u): \n", parameters[1], parameters[0]);
        scanf("%d", &parameters[6]);
    }

    printf("Ingrese el minimo de memoria requerida: \n");
    scanf("%d", &parameters[7]);
    while(parameters[7] <= 0 || parameters[7] > parameters[6] || parameters[7] < parameters[1]){
        printf("Ingrese un numero valido de memoria requerida (mayor a la memoria minima %u y menor o igual al maximo de memoria requerida %u): \n", parameters[1], parameters[6]);
        scanf("%d", &parameters[7]);
    }

    printf("Se han recibido los parametros de los procesos!\n");
    printf("Generando archivo de entrada...\n");

    /*Imprimir valores para
        - Total de memoria
        - Memoria minima
        - Quantum
    */
    fprintf(outputInfo, "%d %d %d\n\n", parameters[0], parameters[1], parameters[2]);

    /*Imprimir valores de entrada
        - Arrival time
        - Burst time
        - Memoria requerida
    */
    for(int i = 0; i < parameters[3]; i++){
        values[0] = rand() % parameters[4];
        values[1] = (rand() % (parameters[5]-1))+1;
        values[2] = (rand() % (parameters[6] - parameters[7] + 1)) + parameters[7];
        fprintf(outputInfo, "%u %u %u\n", (unsigned int) values[0], (unsigned int) values[1], (unsigned int) values[2]);
    }

    printf("Archivo de entrada generado con exito!\n");

    return 0;
}
