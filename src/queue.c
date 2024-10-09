#include "queue.h"




/**
 * @brief Función para verificar si una cola está vacia
 * 
 * @param Q Cola
 * @return true: está vacia. false: no está vacia
 */
bool is_empty(queue Q){
    return Q->centinel->process.PID == 0;
}
/*------------------END------------ */

 /**
  * @brief Función para crear una cola
  * @note Se crea una cola vacía, con un centinela
  */
queue create_queue(){
    //pedir memoria
    queue Q=(queue)malloc(sizeof(queueRecord));
    if(!Q){
        printf("ERROR: No hay memoria disponible");
        exit(1);
    }
    //asignar valores iniciales
    Q->centinel=malloc(sizeof(circularNode));
    if(Q->centinel==NULL){
        printf("ERROR: No hay memoria disponible");
        exit(1);
    }

    Q->centinel->process.PID=0; //PID del centinela será el tamaño de la cola!!!
    Q->centinel->prev = Q->centinel->next = Q->centinel;

    return Q;
}
/*---------END---------*/


/**
 * @brief Función para vaciar una cola con elementos
 * 
 * @param Q Cola
 * @note No elimina la cola, solo los elementos de ella. La vacia.
 */ 
void make_empty(queue Q){

    // caso la cola ya está vacia o es nula
    if(!Q || is_empty(Q)){
        return;
    }

    // caso la cola no está vacia
    circularNode *position = Q->centinel->next;
    circularNode *nextPosition;

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
/*--------END------*/

/**
 * @brief Liberar memoria de la cola
 * 
 * @param Q Cola
 * @note Elimina la cola
 */
void free_queue(queue Q){
    if(Q){
        make_empty(Q);
        free(Q);
    }
}
/*-----------END----------------*/


/**
 * @brief Agregar proceso a la cola
 * 
 * @param P Proceso
 * @param Q Cola a agregar el proceso
 * 
 * @note Agrega el proceso al final de la cola
 */
void enqueue(process P, queue Q){

    circularNode *newNode = (circularNode*)malloc(sizeof(circularNode));
    // error de memoria
    if(!newNode){
        printf("ERROR: No hay memoria disponible");
        exit(1);
    }

    // asignar al nodo nuevo el proceso
    newNode->process=P;
    
    // variable auxilar para no perder la cola anterior
    circularNode *position=Q->centinel->prev;
    
    Q->centinel->prev=newNode; // hacer que el nodo nuevo sea la cola
    newNode->next=Q->centinel; // conexión cola -> centinela
    newNode->prev=position; // conectar cola nueva con cola anterior
    position->next=newNode;
    if(is_empty(Q)){
        Q->centinel->next=newNode; //si la cola está vacia hacer ese nodo el front
    }

    Q->centinel->process.PID++; // aumentar contador del tamaño de la cola}

}
/*--------------END---------*/



/**
 * @brief Devuelve el proceso que está en frente de la cola
 * @param Q Cola
 * @return Proceso front
 * 
 */
process front(queue Q){

    if(!is_empty(Q)){
        return Q->centinel->next->process;
    }

    printf("ERROR: la cola está vacía");
    exit(1);
}
/* *****************END******************** */


/**
 * @brief Sacar de la cola el proceso en front
 * 
 * @param Q Cola
 */
void dequeue (queue Q){

    if(is_empty(Q)){
        printf("ERROR: la cola está vacía");
    }
    else {

        circularNode* aux = Q->centinel->next->next; // auxiliar para no perder el 2do nodo en la cola

        free(Q->centinel->next); // liberar memoria del frente antiguo
        Q->centinel->next=aux; // hacer que el 2do nodo sea el nuevo frente
        Q->centinel->process.PID--; //descontar contador

    }
}
/************************************************ */


/**
 * @brief Imprimir la cola
 * 
 * @param Q Cola
 */
void print_queue(queue Q){

    if(!is_empty(Q)){

        circularNode *position=Q->centinel->next;
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
