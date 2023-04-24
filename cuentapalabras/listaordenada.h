/*
* Archivo encabezado del TDA Lista.
* El TDA LISTA almacenará elementos pares de la forma <cadena_de_caracteres, entero>.
* La lista es una lista sin centinela con posición indirecta.
*
* Comision N°XX (David Emanuel Latouquette - Otto)
*/
#ifndef LISTAORDENADA_H_INCLUDED
#define LISTAORDENADA_H_INCLUDED

#define ERROR_LISTA_BUSQUEDA -1
#define ERROR_LISTA_ELIMINAR -2
#define ERROR_LISTA_MEMORIA -3

#define TRUE 1
#define FALSE 0

typedef enum{
    ELEM1_MAYOR_QUE_ELEM2,
    ELEM1_IGUAL_QUE_ELEM2,
    ELEM1_MENOR_QUE_ELEM2
} comparacion_resultado_t;

struct elemento {
    int a;
    char *b;
};
typedef struct elemento elemento_t;

typedef comparacion_resultado_t (funcion_comparacion_t)(
    elemento_t *elem1,
    elemento_t *elem2
);

typedef struct lista lista_t;

/**
* Crea una lista vacía y la devuelve.
*/
extern lista_t *lista_crear();

/**
* Inserta el elemento 'elem' en la posición 'pos' de la lista (Asumo que la posicion 'pos' comienza desde 0).
* Retorna:
*   ERROR_LISTA_MEMORIA si no se reservó memoria.
*   ERROR_LISTA_BUSQUEDA si se pasó una posicion fuera de rango.
*   TRUE si se insertó el elemento correctamente.
*/
extern int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos);

/**
* Elimina el elemento de la posición 'pos' de la lista.
*/
extern elemento_t *lista_eliminar(lista_t *l, unsigned int pos);

/**
* Devuelve un puntero al elemento que ocupa la posición 'pos' de la lista.
*/
extern elemento_t *lista_elemento(lista_t *l, unsigned int pos);

/**
* Dada la lista 'l' y la función 'comparar' ordena la lista de acuerdo al criterio de dicha función.
*/
extern int lista_ordenar(lista_t *l, funcion_comparacion_t comparar);

/**
* Devuelve la cantidad de elementos de la lista 'l'.
*/
extern unsigned int lista_cantidad(lista_t *l);

/**
* Devuelve verdadero (distinto de 0) si la lista está vacía, y falso (igual a 0) si la lista contiene al menos un elemento.
*/
extern int lista_vacia(lista_t lista);

#endif // LISTAORDENADA_H_INCLUDED
