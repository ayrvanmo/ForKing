/**
 * @file errors.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones para manejo de errores
*/
#include "errors.h"

/**
 * @brief Funcion para imprimir un error
 *
 * @param num Codigo de error
 * @param target Algun parametro que puede ser necesario para el error
 * @param obs Algun texto que puede acompanhar el error
*/
void print_error(int num, char* target, char* obs) {
    if(num>=100 && num<200){
        printf(ANSI_COLOR_RED "Error %d: " ANSI_COLOR_RESET, num);
    }
    else
    if(num>=200 && num<300){
        printf(ANSI_COLOR_RED "Fatal Error %d: " ANSI_COLOR_RESET, num);
    }
    else if(num>=300 && num<400){
        printf(ANSI_COLOR_YELLOW "Warning %d: " ANSI_COLOR_RESET, num);
    }
    switch(num) {
        case 100:
            printf("No se recibio ningun archivo\n");
            break;
        case 101:
            printf("No se pudo leer el archivo %s\n", target);
            break;
        case 200:
            printf("No hay memoria disponible\n");
            exit(-1);
            break;
        case 201:
            printf("No se entrego un archivo abierto\n");
            exit(-1);
            break;
        case 202:
            printf("No se pudo acceder al sistema\n");
            exit(-1);
            break;
        case 203:
            printf("No se ingresaron procesos\n");
            exit(-1);
            break;
        case 204:
            printf("Cantidad invalida, la cantidad ingresada de be ser mayor a 0\n");
            exit(-1);
            break;
        case 205:
            printf("No se ha podido leer la memoria maxima \n");
            exit(-1);
        case 206:
            printf("No se ha podido leer la memoria minima\n");
            exit(-1);
            break;
        case 207:
            printf("No se ha podido leer el quantum\n");
            exit(-1);
            break;
        case 208:
            printf("Proceso invalido, uno o mas parametros contienen valores negativos\n");
            exit(-1);
            break;
        case 300:
            printf("Lista vacia\n");
            break;
        case 301:
            printf("Inconsistencia, Buddy encontrado tiene hijos\n");
            break;
        case 302:
            printf("Cola vacia\n");
            break;
        case 303:
            printf("El siguiente proceso no es valido; sera eliminado\n");
            break;
        default:
            printf("Codigo de error desconocido\n");
    }
    if (obs != NULL) {
        printf("%s\n", obs);
    }
}