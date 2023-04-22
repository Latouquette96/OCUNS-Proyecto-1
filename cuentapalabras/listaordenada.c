#include <stdio.h>
//#include <stdlib.h>
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

struct lista {
    celda_t *primera; ///Puntero a la primera celda de la lista.
    int cantidad; ///Cantidad de elementos de la lista.
};

/**
* Crea una lista vacía y la devuelve.
*/
lista_t *lista_crear();

/**
* Inserta el elemento 'elem' en la posición 'pos' de la lista.
*/
int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos);

/**
* Elimina el elemento de la posición 'pos' de la lista.
*/
elemento_t *lista_eliminar(lista_t *l, unsigned int pos);

/**
* Dada la lista 'l' y la función 'comparar' ordena la lista de acuerdo al criterio de dicha función.
*/
int lista_ordenar(lista_t *l, funcion_comparacion_t comparar);

/**
* Devuelve la cantidad de elementos de la lista 'l'.
*/
unsigned int lista_cantidad(lista_t *l);

/**
* Devuelve verdadero (distinto de 0) si la lista está vacía, y falso (igual a 0) si la lista contiene al menos un elemento.
*/
int lista_vacia(lista_t lista);
