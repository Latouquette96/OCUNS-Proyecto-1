/**
* @file cuentapalabras.c
* @brief Implementación del TDA Cuenta Palabras.
* Modela las operaciones necesarias para poder leer un directorio de archivos .txt y 
poder contabilizar la cantidad de palabras que hay, y posteriormente plasmar dicha 
información en archivos de salida.
*
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> //Utilizada para obtener los archivos de los directorios.
#include "multiset.h"
#include "lista.h"
#include "cuentapalabras.h"

//----FUNCIONES PARA LA COMPROBACIÓN DEL DIRECTORIO Y ARCHIVOS------

/**
 * @brief Abre el directorio indicado en path y devuelve un puntero al manejador de archivos de dicho directorio.
 * @param path Puntero a una cadena de caracteres que indica una ruta a un directorio.
 * @return Puntero al manejador de archivos del directorio en caso de que la ruta sea válida, en caso contrario, retorna NULL (y por lo tanto, la ruta dada será errónea).
*/
DIR* cuentapalabras_abrir_directorio(char *path){
    DIR *d = opendir(path);
    return d;
}

/**
 * @brief Comprueba si un puntero a una cadena de caracteres es un el nombre de un archivo con extensión .txt.
 * @param name Puntero a cadena de caracteres.
 * @return TRUE si es un archivo .txt y FALSE en caso contrario.
*/
static int cuentapalabras_tiene_extension_txt(char *name){
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
 * @brief Recupera un arreglo de todos los nombres de archivos de texto que hay en el directorio apuntado por d.
 * @param d Puntero a DIR.
 * @return Puntero a puntero de cadenas de caracteres (relativo a un arreglo bidimensional de chars).
*/
char** cuentapalabras_recopilar_nombres_archivos_txt(DIR *d, int i, int*cant_filas){
    //Recupero el valor actual.
    struct dirent *dir = readdir(d);
    
    //Si no hay mas que leer, entonces se construye el arreglo de i filas.
    if (dir==NULL){
        char** arreglo = malloc(i*sizeof(char*));
        return arreglo;
    }
    else{
        if (tiene_extension_txt(dir->d_name)==TRUE){
            //Incrementa la cantidad de filas encontradas.
            (*cant_filas) = (*cant_filas)+1;
            //Recopila el nombre de los archivos que siguen.
            char** arreglo = cuentapalabras_recopilar_nombres_archivos_txt(d, i+1, cant_filas);
            //Obtiene la cantidad de caracteres (excepto el caracter nulo) que tiene el nombre del archivo.
            int long_name = strlen(dir->d_name);
            //Reservo memoria para almacenar el nombre del archivo en la fila i
            arreglo[i] = malloc(long_name*sizeof(char)+1); //+1 para el caracter nulo.
            //Copia el nombre del archivo en en la fila i
            strcpy(arreglo[i], dir->d_name);

            return arreglo;
        }
        else{
            return cuentapalabras_recopilar_nombres_archivos_txt(d, i, cant_filas);
        }
    }
}

/**
 * @brief Libera la memoria reservada para un puntero de punteros a cadenas de caracteres.
 * @param C Puntero a punteros de cadenas de caracteres.
 * @param cant_filas Cantidad de filas (o punteros a punteros) en C.
*/
void cuentapalabras_liberar_memoria_nombres_archivos(char** C, int cant_filas){
    //Libera el espacio para cada una de las filas de caracteres.
    for (int i=cant_filas-1; i>=0; i++){
        free(C[i]);
    }

    //Libera el espacio final del C
    free(C);
    C = NULL;
}