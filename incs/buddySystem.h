/**
 * @file buddySystem.h
 * @author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 * @brief Cabecera relacionada al archivo buddySystem.c
 */

#ifndef BUDDY_SYSTEM
#define BUDDY_SYSTEM

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "errors.h"
#include "files.h"
#include "process.h"
#include "utilities.h"

typedef struct _buddy Buddy;
typedef struct _treeNode TreeNode;
typedef TreeNode* PrtToTreeNode;
typedef PrtToTreeNode TreePosition;
typedef PrtToTreeNode BuddySystem;

/*! \struct Buddy
  * @brief Estructura para manejar los buddys dentro del BuddySystem
  */
struct _buddy{
	Process* process; /*!< Representa el proceso asociado al buddy, PID -1 en caso de no existir */
	unsigned int order; /*!< Representa la altura de un Buddy */
	bool isUsed; /*!< Indica si el espacio de memoria de un buddy esta siendo ocupado */
};

/** \struct TreeNode
 * @brief Estructura encargada de generar los nodos de un BuddySystem
 */
struct _treeNode{
	Buddy element; /*!<Elemento del nodo es un Buddy*/
	TreePosition parent; /*!< Padre del nodo en cuestion*/
	TreePosition left; /*!<Hijo izquierdo del nodo*/
	TreePosition right; /*!<Hijo derecho del nodo*/
};

BuddySystem empty_buddy_system(BuddySystem T, SystemConfig config);
BuddySystem delete_buddy_system(BuddySystem T);
BuddySystem insert_buddy(Process* P, BuddySystem T, SystemConfig config, SystemStatus* status);
BuddySystem free_buddy(Process* P, BuddySystem T, SystemConfig config, SystemStatus* status);
TreePosition find_buddy(Process* P, BuddySystem T);
TreePosition find_space(BuddySystem T, unsigned int order);
Process* retrieve_buddy(Buddy B);
void print_buddy_system(BuddySystem B);
void merge_buddy(TreePosition T);

#endif