/**
 * @file process.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones y estructuras relacionadas a los procesos
*/
#include "process.h"

/**
 * @brief Verifica si la lista esta vacia
 * @param L Lista entregada
 * @return int Devuelve 1 si esta vacia y 0 si no lo esta
 */
int is_empty_list(List L)
{
	return L->next == NULL;
}

/**
 * @brief Imprime en pantalla la lista de procesos
 * @param L Lista entregada
 */
void print_list(List L)
{
	//printf("La lista de procesos es: \n");
	Position P;
	P = L->next;
	int i = 1;
	while(P != NULL){
		print_process(P->data);
		P = P->next;
		i++;
	}
	if(is_empty_list(L)){
		print_error(300, NULL, NULL);
	}
}

/**
 * @brief Imprime en pantalla un proceso
 * @param P Proceso a imprimir
 */
void print_process(Process P)
{
	printf("PID:%5d ", P.PID);
	printf("Arrival:%5d ",P.arrivalTime);
	printf("Burst:%5d ",P.burstTime);
	printf("Memory:%5d \n",P.memoryRequired);
}

/**
 * @brief Elimina y libera la memoria de la lista de procesos
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
 * @param P
 * @return int
 */
int is_last(Position P)
{
	return P->next == NULL;
}

/**
 * @brief Inicializa una lista vacia o vacia una ya existente
 * @param L Lista entregada
 * @return List Retorna la lista vacia
 */
List make_empty_list(List L)
{
	if(L != NULL){
		delete_list(L);
	}
	L = malloc(sizeof(ProcessNode));
	if(L == NULL){
		print_error(200, NULL, NULL);
		return NULL;
	}
	L->data.PID = 0;
	L->next = NULL;
	return L;
}

/**
 * @brief Encuentra un proceso dentro de la lista de procesos
 * @param X Proceso que se busca
 * @param L Lista
 * @return Position Retorna la posicion del proceso
 */
Position find_process(Process X, List L)
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
	L->data.PID--;
	return P;
}

/**
 * @brief Inserta un proceso dentro de una lista de procesos
 * @param X Proceso dado
 * @param L Lista entregada
 * @param P Posicion de la lista en donde se insertara el proceso
 * @return Position
 */
Position insert_element(Process X, List L, Position P)
{
	Position Tmpcell;
	Tmpcell = malloc(sizeof(ProcessNode));
	if(Tmpcell == NULL){
		print_error(200, NULL, NULL);
	}
	Tmpcell->data = X;
	Tmpcell->next = P->next;
	P->next = Tmpcell;
	L->data.PID++;
	return P->next;
}

/**
 * @brief Inserta un proceso al final de la lista
 * @param X Proceso entregado
 * @param L Lista entregada
 * @return Position Retorna la posicion del proceso insertado
 */
Position insert_element_end(Process X, List L){
	Position P = L;
	while(P->next != NULL){
		P = P->next; //LLEGA HASTA EL FINAL DE LA LISTA GUARDANDO LA POSICION
	}
	return insert_element(X, L, P); //RETORNA UNA INSERCION EN LA POSICION AL FINAL DE LA LISTA
}


/**
 * @brief Retorna el centinela de la lista
 * @param L Lista entregada
 * @return List el puntero centinela
 */
List header(List L)
{
	return L;
}

/**
 * @brief Entrega el primer elemento de la lista
 * @param L Lista entregada
 * @return List Retorna el primer elemento de la lista
 */
List first(List L)
{
	return L->next;
}

/**
 * @brief Entrega la posicion contigua a una posicion en la lista
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
 * @param P Posicion entregada
 * @return  Devuelve el proceso
 */
Process retrieve(Position P)
{
	return P->data;
}

/**
 * @brief Ordena una lista de procesos segun un criterio
 * @param L Lista entregada
 * @param criterion Criterio de ordenamiento
 * @return List Retorna la lista ordenada
 */
List bubble_sort(List L, unsigned int(*criterion)(Process)){
	if(is_empty_list(L)){
		print_error(300, NULL, NULL);
		return NULL;
	}
	unsigned int listSize = L->data.PID;
	unsigned int toOrder = listSize-1;
	Position P;

	P=L->next;
	while(toOrder > 0){
		for(unsigned int i = 0; i < toOrder; i++){
			unsigned int criterion1 = criterion(P->data);
			unsigned int criterion2 = criterion(P->next->data);
			if(criterion1 > criterion2){
				swap_proceses(P, P->next);
			}
			P = P->next;
		}
		P=L->next;
		toOrder--;

	}
	return L;
}

/**
 * @brief Encuentra el punto medio de una lista
 * @param L Lista entregada
 * @return List Retorna el punto medio de la lista
 */
List midPoint(List L)
{
	Position slow = L;
	Position fast = L->next;

	while(fast != NULL && fast->next != NULL){
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;

}

/**
 * @brief Realiza el merge de las listas a y b segun un criterio
 * @note Se utiliza en el merge_sort
 * @param a Lista entregada (1)
 * @param b Lista entregada (2)
 * @param criterion Criterio de ordenamiento
 * @return * List Lista mezclada
 */
List merge(Position a, Position b, unsigned int(*criterion)(Process))
{

    Position tmpNode = (Position)malloc(sizeof(ProcessNode)); // Nodo temporal
	if(tmpNode == NULL){
		print_error(200, NULL, NULL);
		return NULL;
	}
    Position tail = tmpNode; // Cola de la lista fusionada

    while(a!=NULL && b!=NULL) {
        if (criterion(a->data) <= criterion(b->data)) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    if(a != NULL){
        tail->next = a;
	}
    else{
        tail->next = b;
	}

    Position result = tmpNode->next;
    free(tmpNode);
    return result;
}
/**
 * @brief Ordena una lista de procesos segun un criterio
 * @note Divide y venceras!!!
 * @param L Lista entregada
 * @param criterion Critero de ordenamiento
 * @return List Lista ordenada
 */

List merge_sort(List L, unsigned int(*criterion)(Process))
{
	if(L == NULL || L->next == NULL){
		return L;
	}
	// Encuentra punto medio
	Position median = midPoint(L);

	// Divide la lista en dos
	Position a = L;
	Position b = median->next;
	median->next = NULL;

	a = merge_sort(a, criterion);
	b = merge_sort(b, criterion);

	return merge(a, b, criterion);
}

/**
 * @brief Carga una lista de procesos desde un archivo
 * @param file Archivo entregado
 * @return List Retorna la lista de procesos
 * @warning El archivo debe estar abierto
 */
List load_process_list(FILE *file, unsigned int maxMemory, unsigned int minMemory)
{
	unsigned int pidCount = 0;
	// Comprobamos que el archivo sea abierto
	if(file == NULL){
		print_error(202, NULL, NULL);
		return NULL;
	}

	List L = make_empty_list(NULL);
	Process P;

	int parametros[3];
	/*
		0: Tiempo de llegada
		1: Tiempo de rafaga
		2: Memoria requerida
	*/

	while(!feof(file)){
		if(fscanf(file, "%d %d %d", &parametros[0], &parametros[1], &parametros[2]) == 3){

			// Inicializamos el proceso
			if(parametros[0] < 0 || parametros[1] < 0 || parametros[2] < 0){
				print_error(208, NULL, NULL);
			}

			P.arrivalTime = parametros[0]; P.burstTime = parametros[1]; P.memoryRequired = parametros[2];
			P.PID = pidCount;
			if(validate_process(&P, maxMemory, minMemory)){
				insert_element(P, L, header(L));
			}
			else{
				print_error(303, NULL, NULL);
				print_process(P);
			}
			pidCount++;
		}
	}

	if(is_empty_list(L)){
		print_error(203,NULL,NULL);
	}

	// No se hace fclose() dentro de esta funcion.
	return L;
}

// -------- Getters --------
/**
 * @brief Obtiene el PID de un proceso
 * @param P Proceso entregado
 * @return unsigned int Devuelve el PID del proceso
 */
unsigned int get_process_pid(Process P){
	return P.PID;
}
/**
 * @brief Obtiene el tiempo de llegada de un proceso
 * @param P Proceso entregado
 * @return unsigned int Devuelve el tiempo de llegada del proceso
 */
unsigned int get_process_arrival_time(Process P){
	return P.arrivalTime;
}
/**
 * @brief Obtiene el tiempo requerido por un proceso
 * @param P Proceso entregado
 * @return unsigned int Devuelve el tiempo de rafaga del proceso
 */
unsigned int get_process_burst_time(Process P){
	return P.burstTime;
}
/**
 * @brief Obtiene la memoria requerida por un proceso
 * @param P Proceso entregado
 * @return unsigned int Devuelve la memoria requerida por el proceso
 */
unsigned int get_process_memory_required(Process P){
	return P.memoryRequired;
}

/**
 * @brief Intercambia dos procesos dentro de una lista de procesos
 * @param a Proceso 1 para intercambiar
 * @param b Proceso 2 para intercambiar
*/
void swap_proceses(Position a, Position b){
	Process tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}

/**
 * @brief Valida si un proceso es apto para entrar en el programa
 *
 * @param P Puntero a proceso a validar
 * @param config Configuracion del sistema
 * @return true en caso que el proceso sea apto
 * @return false en caso que el proceso no cumpla alguna condicion
*/
bool validate_process(Process* P, unsigned int maxMemory, unsigned int minMemory){
	return 	!((P->memoryRequired > maxMemory) ||
		   	(P->memoryRequired < minMemory) ||
			(P->burstTime == 0));
}