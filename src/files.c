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

	int parametros[4];
	/*
		0: Memoria total
		1: Memoria minima
		2: Nucleos del procesador
		3: Quantum para Round Robin
	*/

	if(!fscanf(inputInfo, "%d", &parametros[0]) || parametros[0] <= 0){
		print_error(205, NULL, NULL);
	}

	forkingConfig->totalMemory = parametros[0];

	if(forkingConfig->totalMemory == (unsigned int) two_power(floor_log2(forkingConfig->totalMemory))){
		forkingConfig->totalMemory = two_power(floor_log2(forkingConfig->totalMemory));
	}
	else{
		forkingConfig->totalMemory = two_power(floor_log2(forkingConfig->totalMemory)+1);
	}

	if(!fscanf(inputInfo, "%d", &parametros[1]) || parametros[1] <= 0 || parametros[1] > parametros[0]){
		print_error(206, NULL, NULL);
	}

	forkingConfig->minMemory = parametros[1];
	forkingConfig->minMemory = two_power(floor_log2(forkingConfig->minMemory));

	if(!fscanf(inputInfo, "%d", &parametros[2]) || parametros[2] <= 0){
		print_error(207, NULL, NULL);
	}
	forkingConfig->cpuCores = parametros[2];

	if(!fscanf(inputInfo, "%d", &parametros[3]) || parametros[3] <= 0){
		print_error(208, NULL, NULL);
	}

	forkingConfig->timeQuantum = parametros[3];

	forkingConfig->processes = load_process_list(inputInfo, forkingConfig->totalMemory, forkingConfig->minMemory);

	fclose(inputInfo);

	return 0;
}

/**
 * @brief Recibe parametros por terminal y los manejas segun corresponda
 * @param argc Cantidad de parametros recibidos
 * @param argv Parametros recibidos
 * @return char* Nombre del archivo a leer
 */
char* get_terminal_parameters(int argc, char* argv[], bool *clean){

	int opt; // Variable para el manejo de opciones
	int opt_index = 0;

	char* filename = NULL;
    *clean = false;

	// estructura para las opciones
	static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
		{"clean", no_argument, 0, 'c'},
        {"file", required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };


	while((opt = getopt_long(argc, argv, ":hcf:", long_options, &opt_index)) != -1){

		switch(opt){
			case 'h':
				printf("forking -f <archivo>      Ejecuta el programa\nforking -f <archivo> -c	  Ejecutar sin imprimir la simulacion\n");
				return NULL;
			case 'f':
                filename = optarg;
                break;
            case 'c':
                *clean = true;
                break;
			case ':':
				print_error(100, NULL , NULL);
				break;
			default:
				printf("Uso: 'forking -f <archivo>'\n'forking -h' para mostrar ayuda\n");
				break;
		}
	}

    if(filename == NULL){
        if(*clean){
            printf("'clean' solo puede utilizarse al ejecutar un archivo.\n");
        }
		else{
            printf("Uso: 'forking -f <archivo>'\n'forking -h' para mostrar ayuda\n");
        }
        return NULL;
    }

    return filename;
}

/**
 * @brief Funcion que imprime por pantalla el funcionamiento del programa
 * @param config Parametros del "sistema"
 * @param status Estado del "sistema"
 * @param waitingQueue Cola de espera de procesos
 * @param arrivalQueue Cola de llegada de procesos
 * @param rrQueue Cola de Round Robin
 * @param sjfQueue Cola de Shortest Job First
 */
void print_program(SystemConfig* config, SystemStatus status){

	int percentage = (status.avaliableMemory*100)/(float)config->totalMemory;

	// Imprimir Informacion del tick
	printf(CLEAR_SCREEN);
	printf(ANSI_COLOR_YELLOW"Ticks: %u\n"ANSI_COLOR_RESET, status.ticks);
	printf("Memoria disponible: %u%%   (%u/%u)\n", percentage, status.avaliableMemory, config->totalMemory);
	printf("Procesos restantes: %u/%u\n", status.remainingProceses, config->totalProcesses);
	printf(ANSI_COLOR_BLUE"------------------ ArrivalQueue ------------------\n"ANSI_COLOR_RESET);
	if(!is_empty_queue(config->arrivalQueue))print_queue(config->arrivalQueue);
	printf("\n");
	printf(ANSI_COLOR_GREEN"------------------ WaitingQueue ------------------\n"ANSI_COLOR_RESET);
	if(!is_empty_queue(config->waitingQueue))print_queue(config->waitingQueue);
	printf("\n");
	printf(ANSI_COLOR_CYAN"------------------    RRQueue   ------------------\n"ANSI_COLOR_RESET);
	if(!is_empty_queue(config->rrQueue))print_queue(config->rrQueue);
	printf("\n");
	printf(ANSI_COLOR_MAGENTA"------------------   SJFQueue   ------------------\n"ANSI_COLOR_RESET);
	if(!is_empty_queue(config->sjfQueue))print_queue(config->sjfQueue);
	printf("\n");
}