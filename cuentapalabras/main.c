/**
* @file main.c
* @brief Encabezado del Main.
* Modela la función que inicia la ejecución de todo el programa. Además, acá es donde se capturan los argumentos de entrada al programa.
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include "cuentapalabras.h"
#include <dirent.h> //Utilizada para obtener los archivos de los directorios.


//----FUNCIONES PARA MOSTRAR MENSAJES INICIALES EN CONSOLA-----

static void mostrar_mensaje_bienvenida(){
    printf("Proyecto: Programacion en C - Cuentapalabras\n");
    printf("Autores: Comision #17 (David Emanuel Latouquette - Otto Krause)\n\n");
    printf("El presente proyecto recibe un directorio, lee los archivos de texto y contabiliza las palabras en dos archivos:\n");
    printf("  -Archivo cadauno.out: Contabiliza las palabras de cada archivo de manera ascendente por el numero de palabras.\n");
    printf("  -Archivo totales.out: Contabiliza TODAS las palabras entre todos los archivos de texto y se ascendente por el numero de palabras.\n\n");
}

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

static void mostrar_mensaje_archivos_a_analizar(char* directorio, char ** nombre, int cant_nombres){
    printf("Directorio '%s' analizado. Se encontraron los siguientes archivos de texto:\n", directorio);
    for (int i=0; i<cant_nombres; i++){
        printf("  -%s\n", nombre[i]);
    }
}

//----MAIN----

int main(int argc, char *argv[]){
    //Por defecto, siempre se pasa un parámetro que es el directorio en donde se ejecuta el programa.
    //Por lo tanto, argc es 1 o mayor a 1.
    if (argc==1){
        mostrar_mensaje_opciones();
    }
    else{
        //Si la comparacion devuelve 0, entonces se tiene que ambas cadenas son iguales.
        if (strcmp(argv[1], "-h")==0){
            mostrar_mensaje_bienvenida();
            //Abre el directorio y recupera el puntero al manejador de archivos.
            DIR* dir = cuentapalabras_abrir_directorio(argv[2]);
            //Si el puntero no es nulo, esto es, que el directorio era válido.
            if (dir!=NULL){
                printf("Directorio a analizar: %s\n\n", argv[2]);
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
                int cant_filas = *p_cant_filas;
                //Muestra los archivos de texto del directorio.
                mostrar_mensaje_archivos_a_analizar(argv[2], nombre_archivo, *p_cant_filas);
                //Ya no se necesita del DIR, por lo tanto, se cierra.
                closedir(dir);
                //Realizar la construcción de los archivos de salida.
                cuentapalabras_construir_archivos_salida(argv[2], nombre_archivo, cant_filas);
                //Libera la memoria utilizada por nombre_archivo y su respectivo contador.
                //cuentapalabras_liberar_memoria_nombres_archivos(nombre_archivo, cant_filas);
                free(p_cant_filas);

                printf("\nArchivos 'cadauno.out' y 'totales.out' creados con exito en el directorio '%s'\n", argv[2]);
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
