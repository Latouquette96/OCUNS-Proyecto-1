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

//---FUNCIONES PRINCIPALES-----

/**
 * @brief Función de comparación de dos elementos.
 * @param elem1 Puntero a un elemento_t.
 * @param elem2 Puntero a un elemento_t.
 * @return ELEM1_MAYOR_QUE_ELEM2 si elem1>elem2, ELEM1_MENOR_QUE_ELEM2 si elem1<elem2 y ELEM1_IGUAL_QUE_ELEM2 si elem1=elem2.
*/
int funcion_comparacion(elemento_t elem1, elemento_t elem2){
    int to_return;
    int valor_elem_1 = elem1.a;
    int valor_elem_2 = elem2.a;

    //Si la cantidad de repeticiones de ELEM1 es mayor a ELEM2
    if (valor_elem_1>valor_elem_2){
        to_return = ELEM1_MAYOR_QUE_ELEM2;
    }
    else{
        //Si la cantidad de repeticiones de ELEM1 es menor a ELEM2
        if (valor_elem_1<valor_elem_2){
            to_return = ELEM1_MENOR_QUE_ELEM2;
        }
        else{
            //En caso de igualdad, comparar por las cadenas.
            //Si el texto de ELEM1 es mayor a ELEM2
            if (strcmp(elem1.b, elem2.b)>0){
                to_return = ELEM1_MAYOR_QUE_ELEM2;
            }
            else{
                //Si el texto de ELEM1 es menor a ELEM2
                if (strcmp(elem1.b, elem2.b)==0){
                    to_return = ELEM1_IGUAL_QUE_ELEM2;
                }
                else{
                    //Si ambos textos son iguales.
                    to_return = ELEM1_MENOR_QUE_ELEM2;
                }
            }
        }
    }

    return to_return;
}

static void aux_liberar_memoria_elemento(elemento_t * elem){
    elem->a = 0;
    free(elem->b);
    free(elem);
    elem = NULL;
}


//----FUNCIONES PARA LA COMPROBACIÓN DEL DIRECTORIO Y ARCHIVOS------


DIR* cuentapalabras_abrir_directorio(char *path){
    DIR *d = opendir(path);
    return d;
}

/**
 * @brief Comprueba si un puntero a una cadena de caracteres es un el nombre de un archivo con extensión .txt.
 * @param name Puntero a cadena de caracteres.
 * @return TRUE si es un archivo .txt y FALSE en caso contrario.
*/
static int aux_es_archivo_txt(char *name){
    //Inicialización de variables.
    int to_return = FALSE;
    int longitud_name = strlen(name);

    if (longitud_name>3){
        if (name[longitud_name-3]=='t' && name[longitud_name-2]=='x' && name[longitud_name-1]=='t'){
            to_return=TRUE;
        }
    }

    return to_return;
}

char** cuentapalabras_recopilar_nombres_archivos_txt(DIR *d, int*cant_filas){
    int cant = 0;
    int cursor = 0;
    struct dirent * dir = readdir(d);

    //Recupera la cantidad de repeticiones a realizar.
    while(dir!=NULL){
        if (aux_es_archivo_txt(dir->d_name)==TRUE){
            cant = cant + 1;
        }
        dir = readdir(d);
    }

    char** arreglo_nombre = (char**) malloc(cant*sizeof(char*));
    //Restablece la posicion del DIR.
    rewinddir(d);
    dir = readdir(d);

    while(dir!=NULL){
        if (aux_es_archivo_txt(dir->d_name)==TRUE){
            int longitud_cadena = strlen(dir->d_name);
            arreglo_nombre[cursor] = malloc(longitud_cadena*sizeof(char)+1);
            strcpy(arreglo_nombre[cursor], dir->d_name);
            cursor = cursor + 1;
        }
        dir = readdir(d);
    }

    //Finalmente asigno el contador al puntero.
    *cant_filas = cant;

    return arreglo_nombre;
}

void cuentapalabras_liberar_memoria_nombres_archivos(char** C, int cant_filas){
    //Libera el espacio para cada una de las filas de caracteres.
    for (int i=0; i<cant_filas; i++){
        free(C[i]);
    }

    //Libera el espacio final del C
    free(C);
    C = NULL;
}

/**
 * @brief Comprueba si es una palabra sin caracteres especiales, esto es, solo está compuesta por letras del alfabeto.
 * @param ch Puntero a cadena de caracteres.
 * @return TRUE si es una cadena sin caracteres especiales, de lo contrario, retorna FALSE.
*/
static int aux_es_palabra_sin_caracteres_especiales(char* ch){
    int to_return = TRUE;

    while(*ch!='\0' && to_return==TRUE){
        //Si es un caracter alfabetico en MAYUSCULAS o minusculas.
        if (((*ch)>=65 && (*ch)<=90) || ((*ch)>=97 && (*ch)<=122)){
            //Apuntar al siguiente char.
            ch++;
        }
        else{
            to_return = FALSE;
        }
    }

    return to_return;
}

/**
 * @brief De acuerdo a la ruta al archivo dada, se procede a leer el archivo de texto y se recopila cada palabra y se las contabiliza.
 * @param path Puntero a cadena de caracteres que conforma la ruta hacia el archivo a leer.
 * @param m_total Multiset donde se cargarán las palabras leidas en el documento.
 * @return Multiset con las palabras contadas pertenecientes al archivo dado.
*/
static multiset_t* aux_cargar_multiset(char*path, multiset_t *m_total){
    /*
    * Se hace uso de la función strtok para dividir las lineas de acuerdo a un arreglo de delimitadores (separadores).
    * Fuente: https://parzibyte.me/blog/2018/11/13/separar-cadena-delimitadores-c-strtok/
    */

    //Crea el multiset a retornar con las palabras contabilizadas del archivo dado.
    multiset_t *m_return = multiset_crear();
    //Abro el archivo en modo de lectura.
    FILE* f = fopen(path, "r");
    //Si no se abre el archivo, entonces ha ocurrido un error.
    if (f==NULL){
        exit(CUENTAPALABRAS_ERROR_APERTURA_ARCHIVO);
    }

    char linea_actual[MAXCHAR];
    char filtro[] = {' ', '\n', '\t', '.', ';', ',', '\0'};

    //Mientras que haya algo que leer en el archivo.
    while (!feof(f)) {
        //Almacema todos los caracteres hasta el salto de línea o hasta el fin del archivo.
        fgets(linea_actual, MAXCHAR, f);
        //Punteros de inicio de archivo y de siguiente línea.
        char * pt_linea = linea_actual;
        //Tokeniza la línea de caracteres y devuelve un puntero a un arreglo de palabras.
        char * token_palabra = NULL;
        //Primer token (primera palabra)
        token_palabra = strtok(pt_linea, filtro);
        //Mientras el token no sea nulo
        while(token_palabra!=NULL){
            //Si la cadena es valida, entonces se carga el token al mapeo
            if (aux_es_palabra_sin_caracteres_especiales(token_palabra)==TRUE){
                multiset_insertar(m_return, token_palabra);
                multiset_insertar(m_total, token_palabra);
            }
            //Recupera el siguiente token
            token_palabra = NULL;
            token_palabra = strtok(NULL, filtro);
        }
        //Fin de línea.
        token_palabra = NULL;
        *linea_actual = 0;
    }

    //Cierro el archivo
    fclose(f);

    return m_return;
}

/**
 * @brief Escribe en el archivo indicado el nombre del archivo seguido del contenido del multiset.
 * @param file Puntero al manejador de archivo. Requiere que esté abierto el archivo para poder ser escrito.
 * @param nombre_archivo Puntero a cadena de caracteres que conforman el nombre del archivo.
 * @param multiset_archivo Puntero a multiset de palabras ordenadas.
*/
static void aux_exportar_multiset_a_archivo(FILE *file, char* nombre_archivo, multiset_t* multiset_archivo){
    //Si la cadena recibida es distinta de una cadena vacía.
    if (*nombre_archivo!='\n'){
        fprintf(file, "%s\n", nombre_archivo);
    }

    lista_t L = multiset_elementos(multiset_archivo, &funcion_comparacion);

    //Si la lista no está vacia, entonces se procede a recorrerla para obtener los elementos.
    if (lista_vacia(L)==FALSE){
        int size_lista = lista_cantidad(&L);

        //Para cada elemento del multiset, se lo escribe en el formato "cant_repeticion palabra".
        for (int i=0; i<size_lista; i++){
            elemento_t * elem_i = lista_elemento(&L, i);
            fprintf(file, "%d\t%s", elem_i->a, elem_i->b);
        }

        //Elimino todos los elementos de la lista L.
        for (int i=0; i<size_lista; i++){
            elemento_t * elem = lista_eliminar(&L, 0);
            aux_liberar_memoria_elemento(elem);
        }
    }
}

void cuentapalabras_construir_archivos_salida(char* directorio, char** nombre_archivo, int cant_filas){
    //Crea un multiset donde se almacenarán todas las palabras leidas.
    multiset_t* multiset_total = multiset_crear();
    //Ruta hacia los archivos cadauno.out y otro para totales.out.
    char path_cadauno[100];
    strcpy(path_cadauno, directorio);
    strcat(path_cadauno, "\\cadauno.out");

    char path_totales[100];
    strcpy(path_totales, directorio);
    strcat(path_totales, "\\totales.out");

    //Crea dos punteros a archivos, uno para el archivo cadauno.out y otro para totales.out.
    FILE * f_cadauno = fopen(path_cadauno, "w"); //Solo escribe.
    if (f_cadauno==NULL){
        exit(CUENTAPALABRAS_ERROR_APERTURA_ARCHIVO);
    }
    FILE * f_totales = fopen(path_totales, "w"); //Solo escribe.
    if (f_totales==NULL){
        exit(CUENTAPALABRAS_ERROR_APERTURA_ARCHIVO);
    }

    //Para cada archivo_i.
    for (int i=0; i<cant_filas; i++){
        //Creo el path para la ruta del archivo.
        char path[260];
        strcpy(path, directorio);
        strcat(path, "\\");
        strcat(path, nombre_archivo[i]);

        //Lee el archivo i y carga las palabras en el multiset_total, devolviendo un multiset cargado con las palabras leidas en la iteración I.
        multiset_t* multiset_archivo = aux_cargar_multiset(path, multiset_total);
        //Escribir el contenido del multiset_archivo en el archivo de salida.
        aux_exportar_multiset_a_archivo(f_cadauno,  nombre_archivo[i], multiset_archivo);
        //Elimina el multiset i
        multiset_eliminar(multiset_archivo);
    }

    //Finalmente, para el multiset_total es cargado en el archivo totales.out
    aux_exportar_multiset_a_archivo(f_totales, "", multiset_total);

    //Cerrar archivos iniciales.
    fclose(f_cadauno);
    fclose(f_totales);

    //Liberar memoria reservadas para multisets
    multiset_eliminar(multiset_total);
}
