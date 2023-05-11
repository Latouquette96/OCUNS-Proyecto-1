/**
* @file main.c
* @brief Encabezado del Main.
* Modela la función que inicia la ejecución de todo el programa. Además, acá es donde se capturan los argumentos de entrada al programa.
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> //Utilizada para obtener los archivos de los directorios.
#include "multiset.h"
#include "lista.h"

//----FUNCIONES PARA LA COMPROBACIÓN DEL DRECTORIO Y ARCHIVOS------

/**
 * @brief Comprueba si un puntero a una cadena de caracteres es un el nombre de un archivo con extensión .txt.
 * @param name Puntero a cadena de caracteres.
 * @return TRUE si es un archivo .txt y FALSE en caso contrario.
*/
static int tiene_extension_txt(char *name){
    //Inicialización de variables.
    int to_return = FALSE;
    char * p_ch = name;
    char * p_extension = NULL;

    //Mientras no sea el caracter fin de cadena.
    while((*p_ch)!='\0'){
        /**
         * Si el caracter es un punto, se lo almacena como posible inicio de extension de archivo.
         * Esto se hace puesto que podría existir archivos del nombre "archivo.1.txt" y el primer punto no sería el inicio de la extensión pero si el segundo.
        */ 
        if ((*p_ch)=='.'){
            p_extension = p_ch;
        }
        //Apunta al próximo caracter.
        p_ch++;
    }

    //Finalmente, si el puntero a la extensión no es nulo, entonces se verifica si el nombre dado finaliza con .txt
    if (p_extension!=NULL){
        //Si la extensión dada es igual a .txt, entonces es un archivo .txt.
        if (strcmp(p_extension, ".txt")==0){
            to_return = TRUE;
        }
    }

    return to_return;
}

/**
 * @brief Abre el directorio indicado en path y devuelve un puntero al manejador de archivos de dicho directorio.
 * @param path Puntero a una cadena de caracteres que indica una ruta a un directorio.
 * @return Puntero al manejador de archivos del directorio en caso de que la ruta sea válida, en caso contrario, retorna NULL (y por lo tanto, la ruta dada será errónea).
*/
static DIR* abrir_directorio(char *path){
    DIR *d = opendir(path);
    return d;
}

/**
 * @brief Analiza el directorio dado en 'path' en busca de archivos .txt y devuelve el resultado de la operación.
 * @param parh Puntero a una cadena de caracteres.
 * @return TRUE si el directorio es correcto y se analizó correctamente, de lo contrario FALSE.
*/
static int analizar_directorio(DIR *d){
    //Inicializa los punteros.
    struct dirent *dir;
    int to_return = TRUE;

    //Si no devuelve nulo, entonces el directorio es correcto.
    if (d!=NULL){
        while((dir=readdir(d))!=NULL){

            if (tiene_extension_txt(dir->d_name)==TRUE){
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    else{
        to_return = FALSE;
    }

    return to_return;
}



//----FUNCIONES PARA MOSTRAR MENSAJES INICIALES EN CONSOLA----- 

/**
 * @brief Muestra un mensaje con los parámetros a emplear para invocar el programa correctamente.
*/
static void mostrar_mensaje_opciones(){
    printf("Bienvenido al programa cuentapalabras. El programa admite los siguientes parametros opcionales:\n\n");
    printf("[-h] [directorio de entrada]: Dado el directorio de archivos de texto, se procesa cada archivo contabilizando las palabras de cada uno de los archivos.\n");
    printf("  -Genera un archivo 'cadauno.out' que contiene la cantidad de veces que aparece cada palabra en en cada uno de los archivos.\n");
    printf("  -Genera un archivo 'totales.out' que contiene la cantidad de veces que aparece cada palabra entre todos los archivos.\n");
}



int main(int argc, char *argv[]){

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
            DIR* dir = abrir_directorio(argv[2]);
            //Si el puntero no es nulo, esto es, que el directorio era válido.
            if (dir!=NULL){
                analizar_directorio(dir);
                printf("Directorio abierto correctamente.");
            }

        }
        else{
            //Si la cadena es distinta a "-h", entonces mostrar mensaje con opciones.
            mostrar_mensaje_opciones();
        }
    }

    return 0;
}
