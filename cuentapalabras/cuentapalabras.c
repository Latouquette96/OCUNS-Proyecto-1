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

#define CUENTAPALABRAS_ERROR_CONTADOR           -5
#define CUENTAPALABRAS_ARCHIVOS_GENERADOS       -6
#define CUENTAPALABRAS_ERROR_APERTURA_ARCHIVO   -7
#define MAXCHAR 500

//---FUNCIONES PRINCIPALES-----

/**
 * @brief Función de comparación de dos elementos que permite identificar cual elemento es mayor, menor o igual que otro.
 * @param elem1 Puntero a un elemento_t.
 * @param elem2 Puntero a un elemento_t.
 * @return ELEM1_MAYOR_QUE_ELEM2 si elem1>elem2, ELEM1_MENOR_QUE_ELEM2 si elem1<elem2 y ELEM1_IGUAL_QUE_ELEM2 si elem1=elem2.
*/
static comparacion_resultado_t funcion_comparacion(elemento_t * elem1, elemento_t * elem2){
    int to_return;
    int valor_elem_1 = elem1->a;
    int valor_elem_2 = elem2->a;

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
            if (strcmp(elem1->b, elem2->b)>0){
                to_return = ELEM1_MAYOR_QUE_ELEM2;
            }
            else{
                //Si el texto de ELEM1 es menor a ELEM2
                if (strcmp(elem1->b, elem2->b)==0){
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

/**
* @brief Libera la memoria reservada por el elemento en cuestion.
* @param elem Puntero a elemento.
*/
static void aux_liberar_memoria_elemento(elemento_t * elem){
    elem->a = 0;
    free(elem->b);
    free(elem);
    elem = NULL;
}

//----FUNCIONES PARA MOSTRAR MENSAJES INICIALES EN CONSOLA-----

/**
* Imprime un mensaje de bienvenida al programa.
*/
static void mostrar_mensaje_bienvenida(){
    printf("+-----------------------------------------------------------------+\n");
    printf("|          Proyecto: Programacion en C - Cuentapalabras           |\n");
    printf("| Autores: Comision #17 (David Emanuel Latouquette - Otto Krause) |\n");
    printf("+-----------------------------------------------------------------+\n\n");
    printf("FUNCIONAMIENTO:\n");
    printf("El presente proyecto recibe un directorio, lee los archivos de texto y contabiliza las palabras en dos archivos:\n");
    printf("  -'cadauno.out' que contiene la cantidad de veces que aparece cada palabra en en cada uno de los archivos.\n");
    printf("  -'totales.out' que contiene la cantidad de veces que aparece cada palabra entre todos los archivos.\n");
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

/**
* @brief Imprime un mensaje mostrando los nombres de los archivos de texto a analizar en el directorio dado.
* @param directorio Puntero a cadena de caracteres que representa el directorio.
* @param nombre Puntero de punteros de cadenas de caracteres que contiene los nombres de los archivos a analizar.
* @param cant_nombres Entero que representa la cantidad de archivos a analizar.
*/
static void mostrar_mensaje_archivos_a_analizar(char* directorio, char ** nombre, int cant_nombres){
    printf("ANALISIS DE ARCHIVOS\n");
    printf("Directorio '%s' analizado. Se encontraron los siguientes archivos de texto:\n", directorio);
    for (int i=0; i<cant_nombres; i++){
        printf("  -%s\n", nombre[i]);
    }
}



//----FUNCIONES PARA LA COMPROBACIÓN DEL DIRECTORIO Y ARCHIVOS------

/**
* @brief Abre el directorio dado por la ruta path y retorna su controlador.
* @param path Puntero a cadena de caracteres que representa el directorio.
* @return Puntero al controlador de directorio o NULL si el directorio dado no es válido.
*/
static DIR* cuentapalabras_abrir_directorio(char *path){
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

/**
* @brief Dado un controlador del directorio, se recopila todos los archivos de textos existentes en dicho directorio y se retorna un puntero a punteros de cadenas de caracteres.
* @param d Puntero al controlador de directorio.
* @param cant_filas Puntero a un entero, donde se almacenará la cantidad de archivos de texto encontrados.
* @param
*/
static char** cuentapalabras_recopilar_nombres_archivos_txt(DIR *d, int*cant_filas){
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

/**
* @brief Libera la memoria reservada para los nombres de los archivos recuperados.
* @param C Puntero a punteros de cadenas de caracteres.
* @param cant_filas Entero positivo que representa la cantidad de archivos a eliminar.
*/
static void cuentapalabras_liberar_memoria_nombres_archivos(char** C, int cant_filas){
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

    //Si la cadena es distinto a una cadena vacía.
    if ((*ch)!='\0'){
        //Mientras el caracter recuperado no sea el fin de cadena y se mantenga la condicion de que es palabra.
        while(*ch!='\0' && to_return==TRUE){
            //Si es un caracter alfabetico en MAYUSCULAS o minusculas.
            if ((*ch)>=97 && (*ch)<=122){
                //Apuntar al siguiente char.
                ch++;
            }
            else{
                to_return = FALSE;
            }
        }
    }
    else{
        to_return = FALSE;
    }

    return to_return;
}

/**
* @brief Recorre recursivamente la próxima cadena a recuperar por del archivo y la devuelve.
* @param f Puntero a archivo.
* @param longitud_cadena Longitud de la cadena a leer.
* @return Puntero a cadena creada o NULL si la cadena en cuestión en un caracter separador de palabras.
*/
static char * aux_recuperar_cadena(FILE *f, int longitud_cadena){
    char ch_actual = fgetc(f);

    if ((feof(f)) || (ch_actual==' ') || (ch_actual=='\n') || (ch_actual=='.') || (ch_actual==':') || (ch_actual==';') || (ch_actual==',') || (ch_actual=='\0')){
        //CB: Si la longitud es 0, entonces no hay cadena que crear.
        if (longitud_cadena==0){
            return NULL;
        }
        else{
            //CB: Si la longitud es mayor a 1, entonces hay una palabra de por lo menos un char.
            char * cadena = (char*) malloc((longitud_cadena+1)*sizeof(char));
            cadena[longitud_cadena] = '\0';

            return cadena;
        }
    }
    else{
        char * cadena = aux_recuperar_cadena(f, longitud_cadena+1);
        cadena[longitud_cadena] = ch_actual;

        return cadena;
    }
}

/**
 * @brief De acuerdo a la ruta al archivo dada, se procede a leer el archivo de texto y se recopila cada palabra y se las contabiliza.
 * @param path Puntero a cadena de caracteres que conforma la ruta hacia el archivo a leer.
 * @param m_total Multiset donde se cargarán las palabras leidas en el documento.
 * @return Multiset con las palabras contadas pertenecientes al archivo dado.
*/
static multiset_t* aux_cargar_multiset(char*path, multiset_t *m_total){
    //Crea el multiset a retornar con las palabras contabilizadas del archivo dado.
    multiset_t *m_return = multiset_crear();
    //Abro el archivo en modo de lectura.
    FILE* f = fopen(path, "r");
    //Si no se abre el archivo, entonces ha ocurrido un error.
    if (f==NULL){
        printf("Error -7: Error en apertura de archivo");
        exit(CUENTAPALABRAS_ERROR_APERTURA_ARCHIVO);
    }

    //Mientras que haya algo que leer en el archivo.
    while (!feof(f)) {
        char * cadena = aux_recuperar_cadena(f, 0);

        if ((cadena!=NULL) && (aux_es_palabra_sin_caracteres_especiales(cadena)==TRUE)){
            multiset_insertar(m_return, cadena);
            multiset_insertar(m_total, cadena);
        }

        free(cadena);
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
    if (nombre_archivo!=NULL){
        fprintf(file, "%s\n", nombre_archivo);
    }

    //Recupera la lista de elementos del multiset.
    lista_t L = multiset_elementos(multiset_archivo, NULL);
    lista_ordenar(&L, funcion_comparacion);

    for (int i=0; i<lista_cantidad(&L); i++){
        elemento_t * elem = lista_elemento(&L, i);
        printf("%d   %s\n", elem->a, elem->b);
    }

    //Si la lista no está vacia, entonces se procede a recorrerla para obtener los elementos.
    if (lista_vacia(L)==FALSE){
        int size_lista = lista_cantidad(&L);

        //Elimino todos los elementos de la lista L.
        for (int i=0; i<size_lista; i++){
            elemento_t * elem = lista_eliminar(&L, 0);
            fprintf(file, "%d   %s\n", elem->a, elem->b);
            aux_liberar_memoria_elemento(elem);
        }
    }

    L.cantidad = 0;
    L.primera = NULL;
}

void cuentapalabras_construir_archivos_salida(char* directorio, char** nombre_archivo, int cant_filas){
    //
    multiset_t **m = (multiset_t**) malloc(sizeof(multiset_t*));

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
        m[0] = aux_cargar_multiset(path, multiset_total);
        //Escribir el contenido del multiset_archivo en el archivo de salida.
        aux_exportar_multiset_a_archivo(f_cadauno,  nombre_archivo[i], m[0]);
        //Elimina el multiset i
        multiset_eliminar(m);
    }

    //Finalmente, para el multiset_total es cargado en el archivo totales.out
    aux_exportar_multiset_a_archivo(f_totales, NULL, multiset_total);

    //Cerrar archivos iniciales.
    fclose(f_cadauno);
    fclose(f_totales);

    //Liberar memoria reservadas para multisets
    //multiset_eliminar(multiset_total);
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
                cuentapalabras_liberar_memoria_nombres_archivos(nombre_archivo, cant_filas);
                free(p_cant_filas);

                printf("\nARCHIVOS GENERADOS\n");
                printf("Archivos 'cadauno.out' y 'totales.out' creados con exito en el directorio '%s'.\n", argv[2]);
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

