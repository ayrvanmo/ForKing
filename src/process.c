/**
 * @file process.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones y estructuras relacionadas a los procesos
*/
#include "process.h"
/**
 * @brief Verifica si la lista esta vacia
 * 
 * @param L Lista entregada
 * @return int Devuelve 1 si esta vacia y 0 si no lo esta
 */
int is_empty(List L)
{
    return L->next == NULL;
}
/**
 * @brief Imprime en pantalla la lista de procesos
 * 
 * @param L Lista entregada
 */
void print_list(List L)
{
    printf("La lista de procesos es: \n");
    Position P;
    P = L->next;
    int i = 1;
    while(P != NULL){
        if(is_last(P)){
            printf("El ultimo proceso es: \n");
        }
        printf("PID[%d]: %d ", i ,  P->data.PID);
        printf("Arrival Time[%d]: %d ", i , P->data.arrivalTime);
        printf("Burst Time[%d]: %d ", i , P->data.burstTime);
        printf("Memory Required[%d]: %d \n", i , P->data.memoryRequired);
        P = P->next;
        i++;
    }
    if(is_empty(L)){
        printf("La lista esta vacia\n");
    }
}
/**
 * @brief Elimina y libera la memoria de la lista de procesos
 * 
 * @param L Lista entregada
 */
void delete_list(List L)
{
    Position P, Tmp;
    P = L->next; 
    L->next = NULL;  
    while(P!= NULL){
        Tmp = P->next;
        free(P);
        P = Tmp;
    }   
}
/**
 * @brief Verifica si un proceso es el ultimo elemento de su lista.
 * 
 * @param P 
 * @return int 
 */
int is_last(Position P)
{
    return P->next == NULL;
}
/**
 * @brief Inicializa una lista vacia o vacia una ya existente
 * 
 * @param L Lista entregada
 * @return List Retorna la lista vacia
 */
List make_empty(List L)
{
    if(L != NULL){
        delete_list(L);
    }
    L = malloc(sizeof(ProcessNode));
    if(L == NULL){
        printf("ERROR FATAL, NO HAY MEMORIA");
    }
    L->next = NULL;
    return L;
}
/**
 * @brief Encuentra un proceso dentro de la lista de procesos
 * 
 * @param X Proceso que se busca
 * @param L Lista 
 * @return Position Retorna la posicion del proceso 
 */
Position find(Process X, List L)
{
    Position P;
    P = L->next;
    while(P != NULL && P->data.PID != X.PID){
        P = P->next;
    }
    return P;
}
/**
 * @brief Encuentra el proceso anterior a uno dado
 * 
 * @param X Proceso que se busca
 * @param L Lista entregada 
 * @return Position Retorna la posicion del proceso
 */
Position find_previous(Process X, List L)
{
    Position P;
    P = L;
    while(P->next != NULL && P->next->data.PID != X.PID){
        P = P->next;
    }
    return P;
}
/**
 * @brief Se encarga de eliminar un proceso de la lista de procesos
 * 
 * @param X Proceso a eliminar
 * @param L Lista entregada
 * @return Position Retorna la posicion eliminada
 */
Position delete_element(Process X, List L)
{
    Position P, TmpCell;
    P = find_previous(X, L);
    if(!is_last(P)){
        TmpCell = P->next;
        P->next = TmpCell->next;
        free(TmpCell);
    }
    return P;
}
/**
 * @brief Inserta un proceso dentro de una lista de procesos
 * 
 * @param X Proceso dado
 * @param L Lista entregada
 * @param P Posicion de la lista en donde se insertara el proceso
 * @return Position 
 */
Position insert_element(Process X, List L, Position P)
{
    Position Tmpcell;
    Tmpcell = malloc(sizeof(ProcessNode));
    if(Tmpcell == NULL && X.PID == 0){
        printf("ERROR FATAL, NO HAY MEMORIA O SE INTENTO INSERTAR UN PID 0");
    }
    Tmpcell->data = X;
    Tmpcell->next = P->next;
    P->next = Tmpcell;  
    return L; 
}
/**
 * @brief Retorna el centinela de la lista
 * 
 * @param L Lista entregada
 * @return List el puntero centinela
 */
List header(List L)
{
    return L;
}
/**
 * @brief Entrega el primer elemento de la lista
 * 
 * @param L Lista entregada
 * @return List Retorna el primer elemento de la lista
 */
List first(List L)
{
    return L->next;
}
/**
 * @brief Entrega la posicion contigua a una posicion en la lista
 * 
 * @param P Posicion entregada
 * @return Position Retorna la siguiente posicion
 * @note No utilizar esta funcion en posiciones inicializadas sin enlace con la lista
 */
Position advance(Position P)
{
    return P->next;
}
/**
 * @brief Devuelve el proceso dentro de una posicion
 * 
 * @param P Posicion entregada
 * @return  Devuelve el proceso
 */
Process retrieve(Position P)
{
    return P->data;
}