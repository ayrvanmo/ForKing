## Autores
Milton Hernandez, Ivan Mansilla, Ayrton Morrison

## Manejo de errores
Dentro del archivo [errors.c](src/errors.c) se encuentra la función `print_error` que se encarga de manejar los errores que puedan ocurrir en el programa. Los códigos de error que se manejan son los siguientes:
| Código de error | Descripción |
| --- | --- |
| 100 | No se recibió ningún archivo |
| 101 | No se pudo leer el archivo \<target\> |
| 102 | Nodo <target> no encontrado |
| 200 | No hay memoria disponible |
| 201 | No se entrego un archivo abierto |
| 300 | Lista vacía |
| 301 | Inconsistencia, Buddy encontrado tiene hijos |
| 302 | Cola Vacia |

Donde:
- 100 - 199 corresponden a errores comunes
- 200 - 299 corresponden a errores fatales
- 300 - 399 corresponden a advertencias

## TODO - Lista de tareas
- [X] Generar una función para manipular todos los errores.
- [X] Optimizar el tiempo de demora del ordenamiento de Lista(Bubble sort a Merge Sort)
- [X] Optimizar el manejo de procesos con ticks iguales
- [X] Actualizar status y utilizarlos para la waiting_queue
- [ ] Trabajar con procesos paralelos **ALTA PRIORIDAD**
- [X] Manejar casos limites (Manejo de procesos)
- [ ] Manejar casos limites (Buddy system)
- [X] Solucionar el problema de la wating_queue **URGENTE**
- [X] Optimizar operaciones con potencias de dos en el BuddySystem (Binario)
- [X] Hacer que la waiting_queue se ordene según memoria de menor a mayor