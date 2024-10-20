/**
 * @file utilities.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Definiciones de funciones de micelania
*/

#include "utilities.h"

/**
 * @brief Calcula la potencia de 2 menor que n
 *
 * @param n Numero a calcular la potencia de 2
 * @return Mayor exponente que cumple que n = 2^exp
*/
int floor_log2(int n){
    if(n == 0){
        return -1;
    }
    int exp = 0;
    while(n >>= 1){
        exp++;
    }
    return exp;
}

/**
 * @brief Eleva dos a la potencia @p exp
 *
 * @param exp Exponente a elevar
 * @return Resultado de elevar dos a la potencia @p exp
*/
int two_power(int exp){
    return 1<<exp;
}