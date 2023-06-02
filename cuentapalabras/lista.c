/**
* @file lista.c
* @brief Implementación del TDA Lista.
* El TDA LISTA almacenará elementos pares de la forma <cadena_de_caracteres, entero>.
* La lista es una lista sin centinela con posición indirecta.
*
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "lista.h"

/**
 * @struct celda
 * @brief La celda permite almacenar un elemento 'elem' y una referencia a la siguiente celda.
*/
struct celda{
    elemento_t *elem;
    struct celda *siguiente;
};

lista_t *lista_crear(){
    //Reservación de memoria para una lista.
    lista_t *L = (struct lista*)malloc(sizeof(struct lista));
    //Si no se realiza la reservación de memoria, entonces se interrumpe la ejecución del programa.
    if (L==NULL){
        printf("Error %d: No se pudo reservar memoria para la lista.\n", ERROR_LISTA_MEMORIA);
        exit(ERROR_LISTA_MEMORIA);
    }
    //Actualiza los atributos de la lista para modelar una lista vacía.
    L->primera = NULL;
    L->cantidad = 0;

    return L;
}

/**
* @brief Construye una nueva celda con el elemento e contenido en dicha celda y celda siguiente en NULL.
* @param e Puntero a elemento.
* @throw ERROR_LISTA_MEMORIA Si no se puedo reservar memoria para una celda o elemento.
@ @return Una celda con el elemento contenido.
*/
static celda_t * aux_construir_celda_nueva(elemento_t * e){
    /*
    * Puesto que en lista_insertar se pasa la estructura y por ende se copia el elemento, entonces procedo a reservar espacio de memoria
        para que dicho elemento se puede conservar en la lista y a la hora de recuperarlo no devuelve basura.
    */
    ///Reservo memoria para la celda nueva.
    celda_t * celda_nueva = (struct celda*) malloc(sizeof(struct celda));
    ///Si no se ha reservado memoria para la nueva celda, salir del programa.
    if (celda_nueva==NULL){
        printf("Error %d: No se pudo reservar memoria para la lista.\n", ERROR_LISTA_MEMORIA);
        exit(ERROR_LISTA_MEMORIA);
    }
    ///Asignar los valores correspondientes.

    celda_nueva->elem = (elemento_t*) malloc(sizeof(struct elemento));
    if (celda_nueva->elem==NULL){
        printf("Error %d: No se pudo reservar memoria para la lista.\n", ERROR_LISTA_MEMORIA);
        exit(ERROR_LISTA_MEMORIA);
    }

    *(celda_nueva->elem) = *e;
    celda_nueva->siguiente = NULL;

    return celda_nueva;
}

int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos){
    int to_return = TRUE;
    int long_list = l->cantidad;
    celda_t * celda_nueva;
    celda_t * celda_actual;
    celda_t * celda_siguiente;

    ///Si la posicion es menor o igual, entonces se procede a insertar el elemento.
    if (pos<=long_list){
        ///Se construye la nueva celda con el elemento en cuestión.
        celda_nueva = aux_construir_celda_nueva(&elem);

        ///Caso 1: Lista vacía
        if (long_list==0){
            l->primera = celda_nueva;
        }
        else{
            ///Caso 2: Lista con elementos y posicion=0 (al inicio de la lista)
            if (pos==0){
                celda_siguiente = l->primera;
                celda_nueva->siguiente = celda_siguiente;
                l->primera = celda_nueva;
            }
            else{
                celda_actual = l->primera;

                ///Caso 3: Insertar al final de la lista
                if (pos==long_list){
                    //Recorro hasta el final de la lista.
                    while(celda_actual->siguiente!=NULL){
                        celda_actual = celda_actual->siguiente;
                    }
                    celda_actual->siguiente = celda_nueva;
                }
                else{
                    ///Caso 4: Otra posicion tal que 0<=pos<long_lista
                    for (int i=1; i<pos; i++){
                        celda_actual = celda_actual->siguiente;
                    }

                    celda_siguiente = celda_actual->siguiente;
                    celda_actual->siguiente = celda_nueva;
                    celda_nueva->siguiente = celda_siguiente;
                }
            }
        }
        l->cantidad = long_list + 1;
    }
    else{
        to_return = FALSE;
    }
    return to_return;
}

elemento_t *lista_eliminar(lista_t *l, unsigned int pos){
    elemento_t * to_return = NULL;
    int long_list = l->cantidad;
    celda_t * celda_actual;
    celda_t * celda_siguiente;
    celda_t * celda_eliminar;

    //Si elimino una posicion dentro de la lista.
    if (pos<long_list){
        celda_actual = l->primera;

        //Si es la primera posicion.
        if (pos==0){
            to_return = celda_actual->elem;
            celda_siguiente = celda_actual->siguiente;
            celda_eliminar = celda_actual;
            l->primera = celda_siguiente;
        }
        else{
            for (int i=1; i<pos; i++){
                celda_actual = celda_actual->siguiente;
            }
            //Me posiciono en la posicion que será removida.
            celda_eliminar = celda_actual->siguiente;
            //Obtiene la celda que apunta la celda a eliminar.
            celda_siguiente = celda_eliminar->siguiente;
            //Finalmente, se procede a hacer los cambios de asignaciones.
            celda_actual->siguiente = celda_siguiente;
        }
        celda_eliminar->siguiente = NULL;
        //Recupero el valor a remover de la lista y quito la referencia de la celda.
        to_return = celda_eliminar->elem;
        celda_eliminar->elem = NULL;
        //Finalemente libero el espacio para la celda.
        free(celda_eliminar);
        //Disminuye la cantidad de elementos en la lista.
        l->cantidad = long_list-1;
    }

    return to_return;
}

elemento_t *lista_elemento(lista_t *l, unsigned int pos){
    elemento_t * to_return = NULL;
    int long_list = l->cantidad;
    celda_t * celda_actual;

    if (pos<long_list){
        celda_actual = l->primera;

        if (pos==0){
            to_return = celda_actual->elem;
        }
        else{
            for (int i=1; i<pos; i++){
                celda_actual = celda_actual->siguiente;
            }
            celda_actual = celda_actual->siguiente;
            to_return = celda_actual->elem;
        }
    }

    return to_return;
}

/**
* @brief Realiza el procedimiento de ordenar la lista recursivamente.
* @param celda_actual Puntero a la celda actual.
* @param pos_inicial Entero positivo con la posicion inicial.
* @param pos_final Entero positivo con la posicion final.
* @param comparar Funcion de comparacion para realizar el ordenamiento según el criterio que establezca.
* @return Puntero a elemento con el eleemtno ordenado.
*/
static elemento_t *aux_ordenar_lista(celda_t *celda_actual, int pos_inicial, int pos_final, funcion_comparacion_t comparar){
    ///Si se llegó al final
    if (pos_inicial==pos_final){
        return celda_actual->elem;
    }
    else{
        ///Si no llegó al final.
        celda_t *celda_sgte = celda_actual->siguiente;
        elemento_t * elem_actual = celda_actual->elem;
        //Se procede a ordenar el siguiente elemento en una posicion mas adelante.
        elemento_t * elem_ordenar = aux_ordenar_lista(celda_actual->siguiente, pos_inicial+1, pos_final, comparar);
        //Obtenido el elemento ordenado, se realiza la comparación de elementos para verificar si es necesario hacer el swap (cambio).
        //Si el elemento1 es mayor al elemento2, entonces se realiza el swap.
        if (comparar(elem_actual, elem_ordenar)==ELEM1_MAYOR_QUE_ELEM2){
            celda_sgte->elem = elem_actual;
            celda_actual->elem = elem_ordenar;
        }
        else{
            elem_ordenar = elem_actual;
        }

        return elem_ordenar;
    }
}


int lista_ordenar(lista_t *l, funcion_comparacion_t comparar){
    int n = l->cantidad;

    ///Cantidad de repeticiones igual a la cantidad de elementos a ordenar.
    for (int i=0; i<n; i++){
        aux_ordenar_lista(l->primera, 0, i, comparar);
    }

    return TRUE;
}


unsigned int lista_cantidad(lista_t *l){
    return l->cantidad;
}

int lista_vacia(lista_t lista){
    return (lista.cantidad==0) ? TRUE : FALSE;
}
