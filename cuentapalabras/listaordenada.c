#include <stdio.h>
#include <stdlib.h>
#include "listaordenada.h"

/*
* Archivo implementación del TDA Lista.
* El TDA LISTA almacenará elementos pares de la forma <cadena_de_caracteres, entero>.
* La lista es una lista sin centinela con posición indirecta.
* La lista a implementar es una lista ordenada.
*
* Comision N°XX (David Emanuel Latouquette - Otto)
*/

typedef struct celda{
    elemento_t elem;
    struct celda *siguiente;
} celda_t;

typedef struct celda celda_t;

struct lista {
    celda_t *primera; ///Puntero a la primera celda de la lista.
    int cantidad; ///Cantidad de elementos de la lista.
};

/**
* Crea una lista vacía y la devuelve.
*/
lista_t *lista_crear(){
    lista_t *L = (struct lista*)malloc(sizeof(struct lista));
    L->primera = NULL;
    L->cantidad = 0;

    return L;
}

/**
* Inserta el elemento 'elem' en la posición 'pos' de la lista (Asumo que la posicion 'pos' comienza desde 0).
* Retorna:
*   ERROR_LISTA_MEMORIA si no se reservó memoria.
*   ERROR_LISTA_BUSQUEDA si se pasó una posicion fuera de rango.
*   TRUE si se insertó el elemento correctamente.
*/
int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos){
    ///Variables a emplear
    celda_t *celda_nueva;
    celda_t *celda_actual;
    celda_t *celda_siguiente;
    unsigned int cant = l->cantidad;
    int valor_retornar = FALSE;

    ///Si posicion es mayor a la cantidad de productos actual, directamente se devuelve
    if (pos>cant){
        valor_retornar = ERROR_LISTA_BUSQUEDA;
    }
    else{
        ///Reservo el espacio en memoria para la nueva celda.
        celda_nueva = (struct celda*) malloc(sizeof(struct celda));
        //Si no se reservó memoria para la nueva celda, entonces se realiza una salida del sistema.
        if (celda_nueva==NULL){
            exit(ERROR_LISTA_MEMORIA);
        }
        //Se almacena el elemento en cuestión en la nueva celda.
        celda_nueva->elem = elem;

        ///Recupero la celda primera de la lista.
        celda_actual = l->primera;

        ///Busca la posicion en donde se debe insertar.
        //Si la posicion es 0, entonces el elemento se inserta al principio de la lista.
        if (pos==0){
            l->primera = celda_nueva;
        }
        else{
            //Busca la posicion en cuestion (empleando posicion indirecta)
            for(int posicion_actual=1; posicion_actual<=pos-1; posicion_actual++){
                celda_actual = celda_actual->siguiente;
            }
        }

        ///Actualizar las referencias de manera consistente.
        //La celda_siguiente será la siguiente celda apuntada por celda_actual, la cual será la siguiente celda de celda_nueva.
        celda_siguiente = celda_actual->siguiente;
        celda_nueva->siguiente = celda_siguiente;
        celda_actual->siguiente = celda_nueva;

        ///Finalmente se incrementa la cantidad de elementos y se actualiza el valor a retornar.
        l->cantidad = cant+1;
        valor_retornar = TRUE;
    }

    return valor_retornar;
}

/**
* Elimina el elemento de la posición 'pos' de la lista.
*/
elemento_t *lista_eliminar(lista_t *l, unsigned int pos){
    elemento_t *elemento_eliminado = NULL;

    ///Buscar solo si la posicion no excede el tamaño de la lista.
    if (pos<(l->cantidad)){
        //Recupera la primera posicion para recorrer
        celda_t *celda_actual = l->primera;
        celda_t *celda_eliminar;

        //Si es la primera posicion de la lista, entonces la celda a remover es la del inicio de la lista.
        if (pos==0){
            //Actualiza la celda a eliminar.
            celda_eliminar = celda_actual;
            //Actualiza como primer celda a la siguiente de celda_actual.
            l->primera = celda_actual->siguiente;
        }
        else{
            //Busca la posicion en cuestion (empleando posicion indirecta)
            for(int posicion_actual=1; posicion_actual<=pos-1; posicion_actual++){
                celda_actual = celda_actual->siguiente;
            }
            //Actualiza la celda a eliminar.
            celda_eliminar = celda_actual->siguiente;
            //Actualiza como siguiente de la celda_actual al siguiente de la celda_eliminar.
            celda_actual->siguiente = celda_eliminar->siguiente;
        }

        ///Recupera el elemento a retornar.
        (*elemento_eliminado) = celda_eliminar->elem;
        ///Elimina la celda en cuestion, liberando el espacio ocupado por dicha celda.
        celda_eliminar->siguiente = NULL;
        free(celda_eliminar);

        ///Disminuye la cantidad de elementos en la lista en 1.
        l->cantidad = l->cantidad - 1;
    }

    return elemento_eliminado;
}

/**
* Devuelve un puntero al elemento que ocupa la posición 'pos' de la lista.
*/
elemento_t *lista_elemento(lista_t *l, unsigned int pos){
    elemento_t *elemento_a_retornar = NULL;

    ///Buscar solo si la posicion no excede el tamaño de la lista.
    if (pos<(l->cantidad)){
        //Recupera la primera posicion para recorrer
        celda_t *celda_actual = l->primera;
        celda_t *celda_elemento;

        //Si la celda del elemento es la primera posicion.
        if (pos==0){
            celda_elemento = celda_actual;
        }
        else{
            //Busca la posicion en cuestion (empleando posicion indirecta)
            for(int posicion_actual=1; posicion_actual<=pos-1; posicion_actual++){
                celda_actual = celda_actual->siguiente;
            }
            //Almacena la celda del elemento.
            celda_elemento = celda_actual->siguiente;
        }

        ///Recupera el elemento a retornar.
        (*elemento_a_retornar) = celda_elemento->elem;
    }

    return elemento_a_retornar;
}

/**
* Dada la lista 'l' y la función 'comparar' ordena la lista de acuerdo al criterio de dicha función.
*/
int lista_ordenar(lista_t *l, funcion_comparacion_t comparar);

/**
* Devuelve la cantidad de elementos de la lista 'l'.
*/
unsigned int lista_cantidad(lista_t *l){
    unsigned int cant = (unsigned int) l->cantidad;
    return cant;
}

/**
* Devuelve verdadero (distinto de 0) si la lista está vacía, y falso (igual a 0) si la lista contiene al menos un elemento.
*/
int lista_vacia(lista_t lista){
    int esta_vacia = (lista.cantidad == 0) ? TRUE : FALSE;
    return esta_vacia;
}
