/**
 * @file queue.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Funciones relacionadas con el manejo de colas
 */
#include "queue.h"

/**
 * @brief Función para verificar si una cola está vacia
 *
 * @param Q Cola entregada
 * @return si una cola es vacia o no
*/
bool is_empty_queue(Queue Q)
{
	return Q->centinel->process->PID == 0;
}

 /**
  * @brief Función para crear una cola
  * @return Se retorna una nueva cola vacia.
  */
Queue create_queue()
{
	//pedir memoria
	Queue Q=(Queue)malloc(sizeof(QueueRecord));
	if(!Q){
		print_error(200, NULL, NULL);
		exit(1);
	}
	//asignar valores iniciales
	Q->centinel=malloc(sizeof(CircularNode));
	if(Q->centinel==NULL){
		print_error(200, NULL, NULL);
		exit(1);
	}

	Q->centinel->process=malloc(sizeof(Process));
	if(Q->centinel==NULL){
		print_error(200, NULL, NULL);
		exit(1);
	}

	Q->centinel->process->PID=0; //PID del centinela será el tamaño de la cola!!!
	Q->centinel->prev = Q->centinel->next = Q->centinel;

	return Q;
}

/**
 * @brief Función para eliminar los ELEMENTOS de una cola
 * @param Q Cola entregada
 */
void make_empty_queue(Queue Q)
{
	// caso la cola ya está vacia o es nula
	if(!Q || is_empty_queue(Q)){
		return;
	}

	// caso la cola no está vacia
	CircularNode *position = Q->centinel->next;
	CircularNode *nextPosition;

	// liberar memoria de la cola
	do{
		nextPosition = position->next;
		free(position);
		position=nextPosition;
	} while(position != Q->centinel);
	// volver a valores iniciales
	Q->centinel->process->PID = 0;
	Q->centinel->prev = Q->centinel->next = Q->centinel;
}

/**
 * @brief Funcion encargada de eliminar una cola
 * @param Q Cola a eliminar
 */
void free_queue(Queue Q)
{
	if(Q){
		make_empty_queue(Q);
		free(Q);
	}
}

/**
 * @brief Funcion que inserta un proceso en la cola
 * @param P Proceso entregado
 * @param Q Cola a agregar el proceso
 * @note Agrega el proceso al final de la cola
 */
void enqueue(Process* P, Queue Q)
{

	CircularNode *newNode = (CircularNode*)malloc(sizeof(CircularNode));
	// error de memoria
	if(!newNode){
		printf("ERROR: No hay memoria disponible");
		exit(1);
	}

	// asignar al nodo nuevo el proceso
	newNode->process=P;

	// variable auxilar para no perder la cola anterior

	CircularNode *position=Q->centinel->prev;
	Q->centinel->prev=newNode; // hacer que el nodo nuevo sea la cola
	newNode->next=Q->centinel; // conexión cola -> centinela
	newNode->prev=position; // conectar cola nueva con cola anterior
	position->next=newNode;
	if(is_empty_queue(Q)){
		Q->centinel->next=newNode; //si la cola está vacia hacer ese nodo el front
	}
	Q->centinel->process->PID++; // aumentar contador del tamaño de la cola}

}

/**
 * @brief Funcion para ordenar decrecientemente una cola segun un criterio
 * @param P Proceso a ingresar
 * @param Q Cola a ingresar el proceso
 * @param criterion Critero con el cual se trabaja
 */
void decreasing_sorting_enqueue(Process *P, Queue Q, unsigned int(*criterion)(Process))
{
	CircularNode *newNode = (CircularNode*)malloc(sizeof(CircularNode));
	// error de memoria
	if(!newNode){
		print_error(200, NULL, NULL);
		exit(1);
	}
	newNode->process = P;

	if(is_empty_queue(Q)){
		enqueue(P, Q); // si la cola está vacia, agregar el proceso al final
		return;
	}
	else if(criterion(*Q->centinel->next->process) <= criterion(*newNode->process)){ // Insertar al inicio
		newNode->next = Q->centinel->next;
		newNode->prev = Q->centinel;
		Q->centinel->next->prev = newNode;
		Q->centinel->next = newNode;
		Q->centinel->process->PID++;
		return;
	}
	else if(criterion(*Q->centinel->prev->process) >= criterion(*newNode->process)){ // Insertar al final
		newNode->next = Q->centinel;
		newNode->prev = Q->centinel->prev;
		Q->centinel->prev->next = newNode;
		Q->centinel->prev = newNode;
		Q->centinel->process->PID++;
		return;
	}
	else{ // Recorrer la cola para insertar en el lugar correcto
		CircularNode *position = Q->centinel->next;
		while(position != Q->centinel){
			if(criterion(*position->process) <= criterion(*newNode->process)){
				newNode->next = position;
				newNode->prev = position->prev;
				position->prev->next = newNode;
				position->prev = newNode;
				Q->centinel->process->PID++;
				return;
			}
			position = position->next;
		}
	}
}

/**
 * @brief Funcion para ordenar en orden creciente una cola segun un criterio
 * @param P Proceso a ingresar
 * @param Q Cola a ingresar el proceso
 * @param criterion Critero con el cual se trabaja
 */
void increasing_sorting_enqueue(Process *P, Queue Q, unsigned int(*criterion)(Process))
{
	CircularNode *newNode = (CircularNode*)malloc(sizeof(CircularNode));
	// error de memoria
	if(!newNode){
		print_error(200, NULL, NULL);
		exit(1);
	}
	newNode->process = P;

	if(is_empty_queue(Q)){
		enqueue(P, Q); // si la cola está vacia, agregar el proceso al final
		return;
	}
	else if(criterion(*Q->centinel->next->process) >= criterion(*newNode->process)){ // Insertar al inicio
		newNode->next = Q->centinel->next;
		newNode->prev = Q->centinel;
		Q->centinel->next->prev = newNode;
		Q->centinel->next = newNode;
		Q->centinel->process->PID++;
		return;
	}
	else if(criterion(*Q->centinel->prev->process) <= criterion(*newNode->process)){ // Insertar al final
		newNode->next = Q->centinel;
		newNode->prev = Q->centinel->prev;
		Q->centinel->prev->next = newNode;
		Q->centinel->prev = newNode;
		Q->centinel->process->PID++;
		return;
	}
	else{ // Recorrer la cola para insertar en el lugar correcto
		CircularNode *position = Q->centinel->next;
		while(position != Q->centinel){
			if(criterion(*position->process) >= criterion(*newNode->process)){
				newNode->next = position;
				newNode->prev = position->prev;
				position->prev->next = newNode;
				position->prev = newNode;
				Q->centinel->process->PID++;
				return;
			}
			position = position->next;
		}
	}
}

/**
 * @brief Funcion para mostrar el frente de una cola
 * @param Q Cola entregada
 * @return Proceso Devuelve el proceso en el frente de la cola
 */
Process* front(Queue Q)
{
	if(!is_empty_queue(Q)){
		return Q->centinel->next->process;
	}
	print_error(302, NULL, NULL);
	exit(1);
}

/**
 * @brief Funcion para eliminar el frente de una cola
 * @param Q Cola entregada
 */
void dequeue (Queue Q)
{

	if(is_empty_queue(Q)){
		print_error(302, NULL, NULL);
	}

	else {
		CircularNode* aux = Q->centinel->next->next; // auxiliar para no perder el 2do nodo en la cola
		if(aux == Q->centinel){
			make_empty_queue(Q);
		} else {
			free(Q->centinel->next); // liberar memoria del frente antiguo
			Q->centinel->next=aux; // hacer que el 2do nodo sea el nuevo frente
			Q->centinel->process->PID--; //descontar contador
		}
	}

}

/**
* @brief Funcion para imprimir la cola
 *
 * @param Q Cola Entregada
 */
void print_queue(Queue Q)
{

	if(!is_empty_queue(Q)){

		CircularNode *position=Q->centinel->next;
		int i=1; //contador posición en la cola

		while (position != Q->centinel) //mientras no se de vuelta a la lista circular
		{
			print_process(*position->process);
			position=position->next;
			i++;
		}
	}
	else {
		print_error(302, NULL, NULL);
	}
}

