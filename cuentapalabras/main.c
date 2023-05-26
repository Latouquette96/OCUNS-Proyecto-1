/**
* @file main.c
* @brief Encabezado del Main.
* Modela la función que inicia la ejecución de todo el programa. Además, acá es donde se capturan los argumentos de entrada al programa.
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
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


static int funcion_comparacion(elemento_t elem1, elemento_t elem2){
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


int main(){
    printf("Prueba de lista\n");
    lista_t * L = lista_crear();
    printf("Insertando numeros del 1 al 10 en la posicion 0\n\n");

    elemento_t ** elem = (elemento_t**) malloc(4*sizeof(struct elemento*));
    for (int i=0; i<4; i++){
        elem[i] = (elemento_t*) malloc(sizeof(struct elemento));
        int * value = (int*) malloc(sizeof(int));
        *value = -1*i*i;
        elem[i]->a = *value;
        elem[i]->b = (char*) malloc(11*sizeof(char));
        strcpy(elem[i]->b, "Hola");
        printf("pos=%d, a=%d, b: %s\n", i, elem[i]->a, elem[i]->b);
    }

    for (int i=0; i<4; i++){
        int resultado = lista_insertar(L, *(elem[i]), i);
    }

    for (int i=0; i<4; i++){
        free(elem[i]->b);
        free(elem[i]);
    }
    free(elem);

    elemento_t * elem0 = lista_elemento(L, 0);
    printf("ELEMENTO:: pos=0, a=%d, b: %s\n", elem0->a, elem0->b);
    elemento_t * elem1 = lista_elemento(L, 1);
    printf("ELEMENTO:: pos=1, a=%d, b: %s\n", elem1->a, elem1->b);
    elemento_t * elem2 = lista_elemento(L, 2);
    printf("ELEMENTO:: pos=2, a=%d, b: %s\n", elem2->a, elem2->b);
    elemento_t * elem3 = lista_elemento(L, 3);
    printf("ELEMENTO:: pos=3, a=%d, b: %s\n", elem3->a, elem3->b);


    lista_ordenar(L, funcion_comparacion);


    for (int i=0; i<4; i++){
        elemento_t * elem_remove = lista_eliminar(L, 0);
        printf("Remove:: pos=i, a=%d, b: %s\n", elem_remove->a, elem_remove->b);
    }

    printf("Longitud lista: %d\n", L->cantidad);


    return 0;
}

/*
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
*/
