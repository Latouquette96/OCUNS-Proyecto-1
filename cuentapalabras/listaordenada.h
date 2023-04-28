/**
* @file listaordenada.h
* @brief Archivo encabezado del TDA Lista.
* El TDA LISTA almacenará elementos pares de la forma <cadena_de_caracteres, entero>.
* La lista es una lista sin centinela con posición indirecta.
*
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#ifndef LISTAORDENADA_H_INCLUDED
#define LISTAORDENADA_H_INCLUDED

#include "define.h"

//Constantes para representar los posibles errores de memoria en las operaciones del TDA Lista.
#define ERROR_LISTA_BUSQUEDA -1
#define ERROR_LISTA_ELIMINAR -2
#define ERROR_LISTA_MEMORIA -3

/**
 * @enum enum
 * @brief Modela los posibles resultados que puede implicar una comparación de dos elementos.
*/
typedef enum{
    ELEM1_MAYOR_QUE_ELEM2,
    ELEM1_IGUAL_QUE_ELEM2,
    ELEM1_MENOR_QUE_ELEM2
} comparacion_resultado_t;

/**
 * @struct elemento.
 * @brief Modela un elemento que tendrá como valor a la cantidad de repeticiones de una palabra 
 * y el valor b será un puntero a la palabra en cuestión.
*/
struct elemento {
    int a;
    char *b;
};
typedef struct elemento elemento_t;

/**
 * @typedef comparacion_resultado_t(funcion_comparacion_t)
 * @brief Plantilla de funcion para la comparación de dos elementos.
*/
typedef comparacion_resultado_t (funcion_comparacion_t)(
    elemento_t *elem1,
    elemento_t *elem2
);
typedef struct lista lista_t;


/**
* @brief Crea una lista vacía y la devuelve.
* Finaliza el programa con ERROR_LISTA_MEMORIA si no se logra reservar memoria para la lista.
* @return Puntero a una lista con memoria reservada.
*/
extern lista_t *lista_crear();

/**
* @brief Inserta el elemento 'elem' en la posición 'pos' de la lista (Asumo que la posicion 'pos' comienza desde 0).
* @param l Puntero a la lista de elementos.
* @param elem Elemento a insertar en la lista.
* @param pos Entero positivo que indica la posicion de inserción (0<pos<n, donde n es el tamaño de la lista).
* @return
*   ERROR_LISTA_MEMORIA si no se reserva memoria.
*   ERROR_LISTA_BUSQUEDA si se pasa una posicion fuera de rango.
*   TRUE si se inserta el elemento correctamente.
*/
extern int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos);

/**
* @brief Elimina el elemento de la posición 'pos' de la lista y lo retorna. 
* @param l Puntero a la lista de elementos.
* @param pos Entero positivo que indica la posicion a eliminar (0<pos<n, donde n es el tamaño de la lista).
* @return El elemento removido de la lista en caso de que la posición pertenezca a la lista (esto es, para una lista de n elemntos, pos<n), 
de lo contrario, retorna NULL.
*/
extern elemento_t *lista_eliminar(lista_t *l, unsigned int pos);

/**
* @brief Devuelve un puntero al elemento que ocupa la posición 'pos' de la lista.
* @param l Puntero a la lista de elemntos.
* @param pos Entero positivo que indica la posicion (0<pos<n, donde n es el tamaño de la lista).
* @return El elemento de la posición 'pos' (esto es, para una lista de n elemntos, pos<n), 
de lo contrario, retorna NULL.
*/
extern elemento_t *lista_elemento(lista_t *l, unsigned int pos);

/**
* @brief Dada la lista 'l' y la función 'comparar' ordena la lista de acuerdo al criterio de dicha función.
* @param l Puntero a la lista de elementos.
* @param comparar Función de comparación de elementos.
* @return TRUE si la lista fue ordenada con exito, de lo contrario, false.
*/
extern int lista_ordenar(lista_t *l, funcion_comparacion_t comparar);

/**
* @brief Devuelve la cantidad de elementos de la lista 'l'.
* @param l Puntero a la lista de elementos.
* @return Entero positivo mayor o igual a 0.
*/
extern unsigned int lista_cantidad(lista_t *l);

/**
* @brief Devuelve verdadero (distinto de 0) si la lista está vacía, y falso (igual a 0) si la lista contiene al menos un elemento.
* @param lista Lista de elementos.
* @return TRUE si la lista está vacia, de lo contrario, FALSE.
*/
extern int lista_vacia(lista_t lista);

#endif //LISTAORDENADA_H_INCLUDED
