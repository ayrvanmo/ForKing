/**
 * @file buddySystem.c
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Todas las funciones relacionadas con BuddySystem (gestion de memoria)
 */
#include "buddySystem.h"

/**
 * @brief Genera un BuddySystem vacio
 * @param T BuddySystem a generar
 * @return BuddySystem vacio
 */
BuddySystem empty_buddy_system(BuddySystem T, SystemConfig config)
{
	if(T != NULL){
		delete_buddy_system(T);
	}

	T = malloc(sizeof(struct _treeNode));
	if(T == NULL){
		print_error(200, NULL, NULL);
	}
	T->element.order = floor_log2(config.totalMemory)-floor_log2(config.minMemory);
	T->element.isUsed = 0;
	T->element.process = NULL;
	T->parent = T->left = T->right = NULL;

	return T;
}

/**
 * @brief Elimina un BuddySystem por completo
 * @param T BuddySystem a eliminar
 * @return BuddySystem Eliminado
 */
BuddySystem delete_buddy_system(BuddySystem T)
{
	if(T == NULL){
		return NULL;
	}

	// Eliminamos la referencia del padre a este hijo
	if (T->parent){
		if (T->parent->left == T){
			T->parent->left = NULL;
		}
		else if (T->parent->right == T){
			T->parent->right = NULL;
		}
	}
	// Eliminacion PostOrder
	T->left = delete_buddy_system(T->left);
	T->right = delete_buddy_system(T->right);
	free(T);
	return NULL;
}

/**
 * @brief Encuentra el buddy en que se encuentra un proceso
 * @param P Proceso a buscar dentro del BuddySystem
 * @param T BuddySystem en el que se desea buscar el proceso @p P
 * @return TreePosition Puntero al nodo en que se encuentra el proceso @p P , NULL en caso de no encontrar
 */
TreePosition find_buddy(Process* P, BuddySystem T)
{
	TreePosition buddyNode;
	if(T == NULL){
		return NULL;
	}
	else if((T->element.process != NULL) && (T->element.process->PID == P->PID)){
		return T;
	}
	buddyNode = find_buddy(P, T->left);
	if(buddyNode){
		return buddyNode;
	}
	buddyNode = find_buddy(P, T->right);
	return buddyNode;
}

/**
 * @brief Inserta un proceso en el Buddy adecuado dentro del BuddySystem
 * @param P Proceso a almacenar
 * @param T BuddySystem en el que se insertara el proceso
 * @return BuddySystem Nodo en el que se almaceno el proceso @p P
 */
BuddySystem insert_buddy(Process* P, BuddySystem T, SystemConfig config, SystemStatus* status)
{
	//printf("Proceso recibido:\n");
	//print_process(*P);

	// Calculo de orden requerida
	unsigned int processOrder =  floor_log2(P->memoryRequired)-floor_log2(config.minMemory);
	// En caso de que el proceso requiera justo la memoria de su orden
	if(P->memoryRequired > (unsigned int)two_power(processOrder+floor_log2(config.minMemory))){
		processOrder++;
	}

	// printf("El proceso de PID %d requiere %d bytes de memoria, orden requerido: %d\n", P->PID, P->memoryRequired, processOrder);

	if(T->element.isUsed){
		// printf("Memoria Llena\n");
		return NULL;
	}
	TreePosition processNode = find_space(T, processOrder);
	if(processNode == NULL){
		//printf("No se encontro espacio para el proceso %u\n", P->PID);
		return NULL;
	}
	processNode->element.process = P;
	processNode->element.order = processOrder;
	processNode->element.isUsed = 1;
	status->avaliableMemory-=(config.totalMemory)/two_power(T->element.order - processOrder);

	return processNode;
}

/**
 * @brief Busca (y si es posible genera) un espacio de orden @p order en el buddySystem @p T
 * @param T BuddySystem
 * @param order Orden a buscar
 * @return TreePosition Puntero a espacio encontrado, NULL en caso de no encontrar nada
*/
TreePosition find_space(BuddySystem T, unsigned int order)
{
	// Nodo no valido
	if(T == NULL || T->element.isUsed || T->element.order < order){
		return NULL;
	}
	// Si el nodo es el que buscamos
	if(!T->element.isUsed && (T->element.order == order) && (T->left == NULL) && (T->right == NULL)){
		return T;
	}
	// Crear hijos de ser necesario
	if((T->left == NULL) && (T->right == NULL)){
		if(T->element.order == 0){ // El nodo no puede tener mas hijos porque la memoria no puede fragmentarse mas
			return NULL;
		}
		// Creamos hijo izquierdo
		T->left = malloc(sizeof(TreeNode));
		if(T->left == NULL){
			print_error(200, NULL, NULL);
			return NULL;
		}
		T->left->element.order = T->element.order-1;
		T->left->element.process = NULL;
		T->left->element.isUsed = 0;
		T->left->parent = T;
		T->left->left = T->left->right = NULL;
		// Creamos hijo derecho
		T->right = malloc(sizeof(TreeNode));
		if(T->right == NULL){
			print_error(200, NULL, NULL);
			return NULL;
		}
		T->right->element.order = T->element.order-1;
		T->right->element.process = NULL;
		T->right->element.isUsed = 0;
		T->right->parent = T;
		T->right->left = T->right->right = NULL;
	}

	// Buscar nodo en subarboles con preorder
	TreePosition foundedSpace = find_space(T->left, order);
	if(foundedSpace != NULL){
		return foundedSpace;
	}
	return find_space(T->right, order);
}

/**
 * @brief Elimina un Proceso del BuddySystem @p T dejando el espacio libre para otro proceso.
 * @param P Proceso a eliminar
 * @param T BuddySystem
 * @return NULL en caso de exito
 */
BuddySystem free_buddy(Process* P, BuddySystem T, SystemConfig config, SystemStatus* status)
{
	TreePosition buddyNode = find_buddy(P, T);

	if(buddyNode == NULL){
		char pid_str[12];
		sprintf(pid_str, "%d", P->PID);
		print_error(102, pid_str, NULL);
		return NULL;
	}

	if(buddyNode->left || buddyNode->right){
		print_error(301, NULL, NULL);
		return NULL;
	}
	status->avaliableMemory +=  (config.totalMemory)/two_power(T->element.order - buddyNode->element.order);
	buddyNode->element.process = NULL;
	buddyNode->element.isUsed = 0;
	merge_buddy(buddyNode);

	return NULL;
}

/**
 * @brief Entrega el proceso asociado a un Buddy
 * @param B BUddy a consultar
 * @return Informacion del proceso asociado al buddy @p B
 */
Process* retrieve_buddy(Buddy B)
{
	return B.process;
}

/**
 * @brief Imprime el estado del buddySystem @p T
 *
 * @param T Estructura de datos que representa el buddySystem
*/
void print_buddy_system(BuddySystem T)
{
	if(T == NULL){
		return;
	}
	if(T->element.process != NULL){
		printf("%d , %2d\n",T->element.order,T->element.process->PID);
	}
	else{
		printf("%d , -1\n",T->element.order);
	}
	print_buddy_system(T->left);
	print_buddy_system(T->right);
}

/**
 * @brief Combina el Buddy @p T de ser posible con su Buddy de manera recursiva
 * @param T Buddy a combinar
 */
void merge_buddy(TreePosition T){
	TreePosition parent = T->parent;
	if(T->element.process != NULL){
		printf("El buddy está ocupado\n");
		return;
	}

	if(parent == NULL){
		// Si parent es NULL hemos terminado
		return;
	}

	if(parent->left->left || parent->left->right || parent->right->left || parent->right->right){
		// Si alguno de los hijos de parent tiene hijos hemos terminado
		return;
	}

	if(parent->left->element.isUsed || parent->right->element.isUsed){
		// Si alguno de los hijos esta ocupado entonces hemos terminado
		return;
	}

	// En este punto sabemos que T y su Buddy están libres
	free(parent->left);
	free(parent->right);
	parent->left = NULL;
	parent->right = NULL;

	// Hacemos lo mismo con el padre, de forma recursiva
	merge_buddy(parent);
}