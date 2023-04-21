#include <stdio.h>
//#include <stdlib.h>
#include "multiset.h"

/*
* Archivo implementación del TDA Multiset.
* Un Multiset es una colección sin orden establecido que acepta elementos repetidos.
*
* Comision N°XX (David Emanuel Latouquette - Otto)
*/

/**
* Crea un multiset vacio de palabras y lo devuelve.
*/
multiset_t *multiset_crear();

/**
* Inserta la palabra 's' al multiset 'm'.
*/
void multiset_insertar(multiset_t *m, char *s);

/**
* Devuelve la cantidad de repeticiones de la palabra 's' en el multiset m.
*/
int multiset_cantidad(multiset_t *m, char *s);

/**
* Devuelve una lista de tipo lista_t ordenada según la función 'f' con todos los elementos del multiset 'm' y la cantidad de apariciones de cada uno.
*/
lista_t multiset_elementos(multiset_t *m, int (*f)(elemento_t, elemento_t));

/**
* Elimina el multiset 'm' liberando el espacio de memoria reservado. Luego de la invocación 'm' debe NULL.
*/
void multiset_eliminar(multiset_t *m);
