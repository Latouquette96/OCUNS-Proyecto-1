/**
* @file main.c
* @brief Encabezado del Main.
* Modela la función que inicia la ejecución de todo el programa. Además, acá es donde se capturan los argumentos de entrada al programa.
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "cuentapalabras.h"
#include <dirent.h> //Utilizada para obtener los archivos de los directorios.


//----FUNCIONES PARA MOSTRAR MENSAJES INICIALES EN CONSOLA-----

/**
 * @brief Imprime un mensaje con los parámetros a emplear para invocar el programa correctamente.
*/
static void mostrar_mensaje_opciones(){
    printf("Bienvenido al programa cuentapalabras. El programa admite los siguientes parametros opcionales:\n\n");
    printf("[-h] [directorio de entrada]: Dado el directorio de archivos de texto, se procesa cada archivo contabilizando las palabras de cada uno de los archivos.\n");
    printf("  -Genera un archivo 'cadauno.out' que contiene la cantidad de veces que aparece cada palabra en en cada uno de los archivos.\n");
    printf("  -Genera un archivo 'totales.out' que contiene la cantidad de veces que aparece cada palabra entre todos los archivos.\n");
}

/**
 * @brief Imprime un mensaje que indica que la ruta de directorio no es válida.
*/
static void mostrar_mesnsaje_ruta_invalida(){
    printf("Ruta inválida. Ejecute el programa nuevamente con un directorio válido.\n");
}


//----MAIN----

int main(int argc, char *argv[]){
    printf("Prueba de lista\n");
    lista_t * L = lista_crear();
    printf("Insertando numeros del 1 al 10 en la posicion 0\n\n");

    elemento_t ** elem = (elemento_t**) malloc(10*sizeof(struct elemento*));
    for (int i=0; i<10; i++){
        elem[i] = (elemento_t*) malloc(sizeof(struct elemento));
        int * value = (int*) malloc(sizeof(int));
        *value = i*i;
        elem[i]->a = *value;
        elem[i]->b = (char*) malloc(11*sizeof(char));
        strcpy(elem[i]->b, "Hola");
    }

    for (int i=0; i<10; i++){
        int resultado = (i==0) ? lista_insertar(L, *(elem[i]), i) : lista_insertar(L, *(elem[i]), i);
    }

    for (int i=0; i<10; i++){
        elemento_t * elem = lista_elemento(L, i);
        printf("pos=%d, a: %d, b:%s\n", i, elem->a, elem->b);
    }

    for (int i=0; i<10; i++){
        elemento_t * elem = lista_eliminar(L, 0);
        printf("Eliminado: %d %s\n", elem->a, elem->b);
    }
}



int main2(int argc, char *argv[]){

    //Por defecto, siempre se pasa un parámetro que es el directorio en donde se ejecuta el programa.
    //Por lo tanto, argc es 1 o mayor a 1.
    if (argc==1){
        mostrar_mensaje_opciones();
    }
    else{
        //Si se ha definido algún parámetro
        printf("Cantidad de parametros: %i\n", argc);
        printf("%s", argv[1]);

        //Si la comparacion devuelve 0, entonces se tiene que ambas cadenas son iguales.
        if (strcmp(argv[1], "-h")==0){
            //Abre el directorio y recupera el puntero al manejador de archivos.
            DIR* dir = cuentapalabras_abrir_directorio(argv[2]);
            //Si el puntero no es nulo, esto es, que el directorio era válido.
            if (dir!=NULL){
                //Reservo memoria para el contador de filas (nombres) del directorio.
                int * p_cant_filas = malloc(sizeof(int));
                //Si no se reservó memoria, entonces terminar ejecución.
                if (p_cant_filas==NULL){
                    exit(CUENTAPALABRAS_ERROR_CONTADOR);
                }
                //Inicializo en cero el contenido de lo apuntado por el puntero cant_filas.
                (*p_cant_filas) = 0;

                //Recupero todos los nombres de archivos de texto.
                char** nombre_archivo = cuentapalabras_recopilar_nombres_archivos_txt(dir, p_cant_filas);

                //Ya no se necesita del DIR, por lo tanto, se cierra.
                closedir(dir);

                int cant_filas = *p_cant_filas;

                //Realizar la construcción de los archivos de salida.
                cuentapalabras_construir_archivos_salida(argv[2], nombre_archivo, cant_filas);

                printf("Pasa construir\n\n");

                //Libera la memoria utilizada por nombre_archivo y su respectivo contador.
                //cuentapalabras_liberar_memoria_nombres_archivos(nombre_archivo, cant_filas);
                free(p_cant_filas);
            }
            else{
                //Puesto que no existe o no se abrió el directorio, entonces se tiene que es una ruta inválida.
                mostrar_mesnsaje_ruta_invalida();
            }
        }
        else{
            //Si la cadena es distinta a "-h", entonces mostrar mensaje con opciones.
            mostrar_mensaje_opciones();
        }
    }

    return 0;
}
