/**
 *	@file files.c
 *	@author Milton Hernandez, Ivan Mansilla, Ayrton Morrison
 *	@brief Funciones relacionadas con archivos y "sistema"
*/
#include "files.h"
/**
 * @brief Lee el archivo de configuracion de los procesos
 * @param file_name Nombre del archivo a leer_name
 * @return 0 en caso de exito, 1 en caso de error
 */
int read_input_file(char* file_name, SystemConfig* forkingConfig) {
	// Abrimos el archivo
	FILE *inputInfo;

	inputInfo = fopen(file_name, "r");
	if (inputInfo == NULL) {
		print_error(101, NULL, NULL);
		return 1;
	}
	// Memoria maxima
	if(!fscanf(inputInfo, "%u", &forkingConfig->totalMemory)){
		printf("No se ha podido leer la memoria maxima\n");
	}
	forkingConfig->totalMemory = pow(2, floor(log2(forkingConfig->totalMemory)));

	// Memoria minima
	if(!fscanf(inputInfo, "%u", &forkingConfig->minMemory)){
		printf("No se ha podido leer la memoria minima\n");
	}
	forkingConfig->minMemory = pow(2, floor(log2(forkingConfig->minMemory)));

	// Nucleos del procesador
	if(!fscanf(inputInfo, "%u", &forkingConfig->cpuCores)){
		printf("No se ha podido leer el numero de nucleos del procesador\n");
	}

	// Quantum para Round Robin
	if(!fscanf(inputInfo, "%u", &forkingConfig->timeQuantum)){
		printf("No se ha podido leer el quantum para Round Robin\n");
	}

	forkingConfig->processes = load_process_list(inputInfo);

	fclose(inputInfo);

	printf("\tInformacion recopilada:\n\n");
	printf("Total memoria: %u\n", forkingConfig->totalMemory);
	printf("Minima memoria: %u\n", forkingConfig->minMemory);
	printf("Nucleos del procesador: %u\n", forkingConfig->cpuCores);
	printf("Quantum para Round Robin: %u\n\n", forkingConfig->timeQuantum);

	return 0;
}
