/**
 * @file queue.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 */
#include "queue.h"

/**
 * @brief Función para verificar si una cola está vacia
 * @param Q Cola
 * @return true: está vacia. false: no está vacia
 */
bool is_empty_queue(Queue Q){
    return Q->centinel->process.PID == 0;
}

 /**
  * @brief Función para crear una cola
  * @note Se crea una cola vacía, con un centinela
  */
Queue create_queue(){
    //pedir memoria
    Queue Q=(Queue)malloc(sizeof(QueueRecord));
    if(!Q){
        printf("ERROR: No hay memoria disponible");
        exit(1);
    }
    //asignar valores iniciales
    Q->centinel=malloc(sizeof(CircularNode));
    if(Q->centinel==NULL){
        printf("ERROR: No hay memoria disponible");
        exit(1);
    }

    Q->centinel->process.PID=0; //PID del centinela será el tamaño de la cola!!!
    Q->centinel->prev = Q->centinel->next = Q->centinel;

    return Q;
}


/**
 * @brief Función para vaciar una cola con elementos
 * @param Q Cola
 * @note No elimina la cola, solo los elementos de ella. La vacia.
 */ 
void make_empty_queue(Queue Q){

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
    } while(position != Q->centinel->next);

    // volver a valores iniciales
    Q->centinel->process.PID = 0;
    Q->centinel->prev = Q->centinel->next = Q->centinel;
}

/**
 * @brief Liberar memoria de la cola
 * @param Q Cola
 * @note Elimina la cola
 */
void free_queue(Queue Q){
    if(Q){
        make_empty_queue(Q);
        free(Q);
    }
}


/**
 * @brief Agregar proceso a la cola
 * @param P Proceso
 * @param Q Cola a agregar el proceso
 * @note Agrega el proceso al final de la cola
 */
void enqueue(Process P, Queue Q){

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

    Q->centinel->process.PID++; // aumentar contador del tamaño de la cola}

}

/**
 * @brief Devuelve el proceso que está en frente de la cola
 * @param Q Cola
 * @return Proceso front
 */
Process front(Queue Q){

    if(!is_empty_queue(Q)){
        return Q->centinel->next->process;
    }

    printf("ERROR: la cola está vacía");
    exit(1);
}

/**
 * @brief Sacar de la cola el proceso en front
 * @param Q Cola
 */
void dequeue (Queue Q){

    if(is_empty_queue(Q)){
        printf("ERROR: la cola está vacía");
    }
    else {

        CircularNode* aux = Q->centinel->next->next; // auxiliar para no perder el 2do nodo en la cola

        free(Q->centinel->next); // liberar memoria del frente antiguo
        Q->centinel->next=aux; // hacer que el 2do nodo sea el nuevo frente
        Q->centinel->process.PID--; //descontar contador

    }
}

/**
* @brief Imprimir la cola
 * @param Q Cola
 */
void print_queue(Queue Q){

    if(!is_empty_queue(Q)){

        CircularNode *position=Q->centinel->next;
        int i=1; //contador posición en la cola

        while (position!= Q->centinel) //mientras no se de vuelta a la lista circular
        {
            printf("PROCESO %d\n",i);
            printf("PID: %u | Arrival time: %u | Burst time: %u | Memory required: %u\n\n", position->process.PID, position->process.arrivalTime, position->process.burstTime,position->process.memoryRequired);
            position=position->next;
            i++;
        }
    }
    else {
        printf("cola vacia");
    }
}
