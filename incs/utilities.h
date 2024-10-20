/**
 * @file utilities.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones de micelania
*/
#ifndef UTILITIES_H
#define UTILITIES_H

// Colores para texto
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RESET    "\x1b[0m"

// Funciones para trabajar con numeros binarios y potencias de 2
int floor_log2(int n);
int two_power(int exp);

#endif