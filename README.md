# Documentación general de Forking

## Autores
Milton Hernandez, Ivan Mansilla, Ayrton Morrison

## Ejecución del programa
Para ejecutar el programa se debe seguir los siguientes pasos:
1. Generar las carpetas necesarias con el comando `make folders`
2. Compilar el programa con el comando `make`
3. Entrar al directorio `build` y generar su lista de procesos mediante el ejecutable `generator.out`
4. Ejecutar el programa con el comando `./forking.out -f <archivo de lista de procesos>`
5. De no tenerlo instalado se recomienda instalar `python3` con las siguientes dependencias
    - `random`
    - `matplotlib`
    - `pandas`
6. Al tener `python3` instalado se generará una carta Gantt con los procesos realizados si son menos de 25 procesos.
7. Puede usar la bandera `-c` para evitar todas las impresiones por pantalla y solamente ver el tiempo de ejecución y la Carta Gantt.

### Otros comandos
- `make doc` Generará una documentación con [Doxygen](https://www.doxygen.nl/), que se presentará en la carpeta `docs/doxygen`, se recomienda abrir el archivo `docs/doxygen/html/index.html` para visualizar la documentación completa del programa.
  - Alternativamente puede ejecutar `make` dentro de `docs/doxygen/latex/` para obtener un `refman.pdf` correspondiente a la documentación del proyecto en formato PDF.

## Manejo de errores
Dentro del archivo [errors.c](src/errors.c) se encuentra la función `print_error` que se encarga de manejar los errores que puedan ocurrir en el programa. Los códigos de error que se manejan son los siguientes:
| Código de error | Descripción |
| --- | --- |
| 100 | No se recibió ningún archivo |
| 101 | No se pudo leer el archivo \<target\> |
| 102 | Nodo <target> no encontrado |
| 200 | No hay memoria disponible |
| 201 | No se entrego un archivo abierto |
| 202 | No se pudo acceder al sistema |
| 203 | No se ingresaron procesos |
| 204 | No se ingreso una cantidad valida |
| 205 | No sea ha podido leer la memoria maxima |
| 206 | No sea ha podido leer la memoria minima |
| 207 | No sea ha podido leer el quantum|
| 208 | Proceso invalido, uno o mas parametros contienen valores negativos |
| 210 | No se ingreso un argumento, intente con './generator.c <numero de procesos>' |
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
- [X] Manejar casos limites (Manejo de procesos)
- [x] Asegurar que no entren procesos con mas memoria de la permitida -> Se optó por aproximar a la mayor potencia de dos la memoria
- [X] Solucionar el problema de la wating_queue **URGENTE**
- [X] Optimizar operaciones con potencias de dos en el BuddySystem (Binario)
- [X] Hacer que la waiting_queue se ordene según memoria de menor a mayor
- [X] Pasar main a funcion
- [X] Manejar paso de parametros negativos (sencillo)
- [X] Optimizar en general y dar el visto bueno
