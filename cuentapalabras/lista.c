/**
* @file lista.c
* @brief Implementación del TDA Lista.
* El TDA LISTA almacenará elementos pares de la forma <cadena_de_caracteres, entero>.
* La lista es una lista sin centinela con posición indirecta.
* La lista a implementar es una lista ordenada.
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
typedef struct celda{
    elemento_t elem;
    struct celda *siguiente;
};


lista_t *lista_crear(){
    //Reservación de memoria para una lista.
    lista_t *L = (struct lista*)malloc(sizeof(struct lista));
    //Si no se realiza la reservación de memoria, entonces se interrumpe la ejecución del programa.
    if (L==NULL){
        exit(ERROR_LISTA_MEMORIA);
    }
    //Actualiza los atributos de la lista para modelar una lista vacía.
    L->primera = NULL;
    L->cantidad = 0;

    return L;
}

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

elemento_t ordenar_auxiliar(celda_t *celda_actual, int pos_inicial, int pos_final, funcion_comparacion_t comparar){
    //Si se llegó al final
    if (pos_inicial==pos_final){
        return celda_actual->elem;
    }
    else{
        celda_t *celda_sgte = celda_actual->siguiente;
        elemento_t elem_actual = celda_actual->elem;
        elemento_t elem_ordenar = ordenar_auxiliar(celda_actual->siguiente, pos_inicial+1, pos_final, comparar);

        if (comparar(&elem_actual, &elem_ordenar)==ELEM1_MAYOR_QUE_ELEM2){
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
        ordenar_auxiliar(l->primera, 0, i, comparar);
    }

    return TRUE;
}

unsigned int lista_cantidad(lista_t *l){
    unsigned int cant = (unsigned int) l->cantidad;
    return cant;
}

int lista_vacia(lista_t lista){
    int esta_vacia = (lista.cantidad == 0) ? TRUE : FALSE;
    return esta_vacia;
}
