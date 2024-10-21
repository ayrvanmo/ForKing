/**
 * @file utilities.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones de micelania
*/
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

// Colores para texto
#define CLEAR_SCREEN "\033[H\033[J"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"

// Funciones para trabajar con numeros binarios y potencias de 2
int floor_log2(int n);
int two_power(int exp);

#endif