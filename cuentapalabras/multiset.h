/**
* @file multiset.h
* @brief Archivo encabezado del TDA Multiset.
* Un Multiset es una coleccion sin orden establecido que acepta elementos repetidos.
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#ifndef MULTISET_H_INCLUDED
#define MULTISET_H_INCLUDED

#include "lista.h"

#define ERROR_MULTISET_MEMORIA -4
#define ERROR_MULTISET_ELIMINAR -5
#define ERROR_MULTISET_INSERTAR -6
#define ERROR_ELEMENTO_MEMORIA -7


/**
* @struct trie
* @brief Representa un árbol Trie, donde cada nodo representa un caracter distinto.
*/
struct trie;
typedef struct trie multiset_t;


/**
 * @brief Crea un multiset vacio de palabras y lo devuelve.
 * Finaliza con ERROR_MULTISET_MEMORIA si el programa no logra reservar memoria para el multiset.
 * @return Puntero al multiset construido.
*/
extern multiset_t *multiset_crear();

/**
 * @brief Inserta la palabra 's' al multiset 'm'.
 * Si la reservación de memoria no se realiza correctamente, puede finalizar la ejecución del programa con ERROR_MULTISET_MEMORIA.
 * @param m Puntero al multiset.
 * @param s Puntero al inicio de la cadena de caracteres.
*/
extern void multiset_insertar(multiset_t *m, char *s);

/**
 * @brief Devuelve la cantidad de repeticiones de la palabra 's' en el multiset m.
 * @param m Puntero al multiset.
 * @param s Puntero al inicio de la cadena de caracteres.
 * @return Entero positivo mayor a 0 si se hay repticiones de s, en cambio, si no está definida o la misma no tiene repeticiones devuelve 0.
*/
extern int multiset_cantidad(multiset_t *m, char *s);

/**
 * @brief Devuelve una lista de tipo lista_t ordenada segun la funcion 'f' con todos los elementos del multiset 'm' y la cantidad de apariciones de cada uno.
 * @param m Puntero al multiset.
 * @param f Función de comparación de elementos.
 * @return Lista de elementos ordenados con las palabras y su respectiva cantidad de repeticiones.
*/
extern lista_t multiset_elementos(multiset_t *m, int (*f)(elemento_t, elemento_t));
 
/**
 * @brief Elimina el multiset 'm' liberando el espacio de memoria reservado. Luego de la invocacion 'm' debe NULL.
 * @param m Puntero al multiset.
*/
extern void multiset_eliminar(multiset_t **m);

#endif // MULTISET_H_INCLUDED
