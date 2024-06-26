/**
 * @file multiset.c
 * @brief Implementación del TDA Multiset, donde se implementan los prototipos de las operaciones dadas en el archivo encabezado multiset.h.
 * @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <stdio.h>
#include <stdlib.h>
#include "multiset.h"
#include "lista.h"
#include "define.h"

/**
 * @struct trie
 * @brief Modela un árbol trie, donde el rótulo será la cantidad de repeticiones de la palabra hasta el nodo actual y
 * puede tener hasta 26 nodos hijos, donde cada hijo representa un caracter entre 'a' y 'z' (excluyendo a la ñ).
*/
struct trie {
    int cantidad; //Cantidad de veces que aparece esa palabra en el multiset.
    struct trie *siguiente[26];
};

/**
 * @brief Operación Dado un char, devuelve la posicion del índice entre 0 y 25 del nodo trie que le corresponde al char.
 * @param ch Puntero al caracter.
 * @return Entero entre 0 y 25 si el char está entre 'a' y 'z' (excluyendo ñ). En caso contrario, devuelve -1.
*/
static int aux_recuperar_posicion_en_alfabeto(char*ch){
    int value_ch = *ch;

    if ((value_ch>=97) && (value_ch<=122)){
        value_ch = value_ch - 97;
    }
    else{
        value_ch = -1;
    }

    return value_ch;
}

/**
 * @brief Operación Dada una posición que corresponde a un nodo del árbol (entre 0 y 25), se recupera el caracter que simboliza dicha posición.
 * @param pos Entero entre 0 y 25.
 * @return Caracter entre 'a' y 'z' (excluyendo a ñ).
*/
static char aux_recuperar_caracter_en_posicion(int pos){
    char ch = (97 + pos);
    return ch;
}

/**
 * @brief Operación Construye un elemento para la cadena de caracteres y la cantidad de repeticiones dada.
 * Si no se reserva memoria, entonces el programa finaliza indicando ERROR_ELEMENTO_MEMORIA.
 * @param cant_repticiones Entero mayor o igual a 1.
 * @param s Puntero a una secuencia de caracteres.
 * @param length_s Entero mayor o igual a 1 que representa la longitud de la cadena s.
 * @throw ERROR_ELEMENTO_MEMORIA si no se pudo reservar memoria para el elemento o para su contenido.
 * @return Elemento construido y con memoria reservada.
*/
static elemento_t aux_construir_elemento(int cant_repeticiones, char* s, int length_s){
    //Reservacián de memoria
    elemento_t *elem = (elemento_t*) malloc(sizeof(elemento_t));
    //Si no se reserva memoria, entonces el programa finaliza indicando el error en cuestion.
    if (elem==NULL){
        printf("Error %d: No se pudo reservar memoria para el elemento.\n", ERROR_ELEMENTO_MEMORIA);
        exit(ERROR_ELEMENTO_MEMORIA);
    }
    //Establece el valor a, esto es, la cantidad de repeticiones.
    elem->a = cant_repeticiones;

    //Establece el valor b, esto es, la secuencia de caracteres.
    char *secuencia = (char*)malloc(sizeof(char)*(length_s+1));
    //Si no se reserva memoria, entonces el programa finaliza indicando el error en cuestion.
    if (secuencia==NULL){
        printf("Error %d: No se pudo reservar memoria para el dato b del elemento.\n", ERROR_ELEMENTO_MEMORIA);
        exit(ERROR_ELEMENTO_MEMORIA);
    }

    //Realiza la carga de los caracteres de la secuencia recibida.
    for (int i=0; i<length_s; i++){
        secuencia[i] = s[i];
    }
    //El ultimo caracter es será el caracter nulo o fin de cadena.
    secuencia[length_s] = '\0';
    elem->b = secuencia;

    return *elem;
}

/**
 * @brief Operación Dado el árbol T, realiza la carga de los elementos cuya cantidad de repeticiones es mayor o igual a 1 en la lista L.
 * @param L Puntero a la lista.
 * @param T Puntero a la estructura del árbol trie.
 * @param s Arreglo de caracteres de la iteración previa.
 * @param length_s Longitud del arreglo previo.
 * @throw ERROR_ELEMENTO_MEMORIA si no se pudo reservar memoria para el elemento o para su contenido.
*/
static void aux_cargar_elementos_en_lista(lista_t *L, struct trie *T, char s[], int length_s){
    ///Inicializa las variables a utilizar.
    struct trie *T_hijo;
    char s_nuevo[length_s+1];

    ///Copia lo obtenido en el arreglo s en uno nuevo con una capacidad+1.
    for (int j=0; j<length_s; j++){
        s_nuevo[j] = s[j];
    }

    ///Para cada posible hijo del nodo T.
    for (int i=0; i<26; i++){
        //Recupero al hijo i del nodo T
        T_hijo = T->siguiente[i];

        //Si el hijo i está definido, entonces se procede a recuperar el char de la posicion i.
        if (T_hijo!=NULL){
            s_nuevo[length_s] = aux_recuperar_caracter_en_posicion(i);

            //Si el nodo hijo tiene una cantidad>0, entonces es una palabra con repeticiones que se debe insertar en la lista.
            if (T_hijo->cantidad > 0){
                elemento_t elem = aux_construir_elemento(T_hijo->cantidad, s_nuevo, length_s+1);
                lista_insertar(L, elem, 0);
            }

            aux_cargar_elementos_en_lista(L, T_hijo, s_nuevo, length_s+1);
        }
    }
}


multiset_t *multiset_crear(){
    //Revervo memoria para el multiset.
    multiset_t *M = (struct trie*)malloc(sizeof(struct trie));
    //Si no se reservá memoria, entonces el programa finaliza indicando el error.
    if (M==NULL){
        printf("Error %d: No se pudo reservar memoria para el multiset.\n", ERROR_MULTISET_MEMORIA);
        exit(ERROR_MULTISET_MEMORIA);
    }
    M->cantidad = 0;
    //Inicializa como NULL las referencia a las 26 posibles letras del abecedario.
    for (int i=0; i<26; i++){
        M->siguiente[i] = NULL;
    }

    return M;
}

void multiset_insertar(multiset_t *m, char *s){
    int pos_en_alfabeto = -1;
    struct trie *T = (struct trie*) m;

    ///Mientras que no se llegue a fin de cadena, se procede a recorrer/crear la secuencia de chars.
    while (*s!='\0'){
        //Se recupera la posicion del char en cuestián.
        pos_en_alfabeto = aux_recuperar_posicion_en_alfabeto(s);
        //Si es un char válido, esto es, la posicion está entre 0 y 25 inclusive.
        if (pos_en_alfabeto!=-1){
            //Si el nodo siguiente en la posicion dada no existe, entonces se crea.
            if (T->siguiente[pos_en_alfabeto]==NULL){
                T->siguiente[pos_en_alfabeto] = (struct trie*) multiset_crear();
            }
            //Recupero el nodo trie en cuestián
            T = T->siguiente[pos_en_alfabeto];
        }
        //Siguiente char.
        s++;
    }

    //Al finalizar el recorrido, se está en el ultimo nodo, por lo que se debe incrementar el contador de palabra en 1.
    T->cantidad = T->cantidad + 1;
}

int multiset_cantidad(multiset_t *m, char s[]){
    ///Inicializar variables
    int cant_repeticiones = 0;
    int existe_palabra = TRUE;
    int pos_en_alfabeto = -1;
    struct trie *T = (struct trie*) m;

    ///Mientras exista la palabra en el trie y exista char que leer aun.
    while((existe_palabra==TRUE) && ((*s)!='\0')){
        //Se recupera la posicion del char en cuestion.
        pos_en_alfabeto = aux_recuperar_posicion_en_alfabeto(s);

        //Si la posicion en alfabeto es válida, esto es, está entre 0 y 25.
        if (pos_en_alfabeto>=0){
            //Se recupera el nodo que corresponde al char.
            T = T->siguiente[pos_en_alfabeto];
            //Si dicho nodo no está creado (esto es, no tiene memoria reservada), entonces no existe la cadena.
            if (T==NULL){
                existe_palabra = FALSE;
            }
        }

        //Si aún existe palabra en el trie, se procede a apuntar al siguiente char.
        if (existe_palabra==TRUE){
            s++;
        }
    }

    if (existe_palabra==TRUE){
        cant_repeticiones = T->cantidad;
    }

    return cant_repeticiones;
}

lista_t multiset_elementos(multiset_t *m, int (*f)(elemento_t, elemento_t)){
    //Se crea la lista de elementos y se almacena su puntero.
    lista_t *L = (lista_t*) lista_crear();
    //Parsing de multiset a trie para poder utilizarlo en la función a continuación.
    struct trie *T = (struct trie*) m;
    //Elemento del nodo raiz es una cadena vacía.
    char s[1] = {'\0'};
    //Se procede a cargar la lista de manera semi-recursiva.
    aux_cargar_elementos_en_lista(L, T, s, 0);

    return *L;
}

/**
 * @brief Realiza la liberación de la memoria, esto es, remueve todo dato perteneciente al nodo de la memoria.
 * @param nodo Puntero a un nodo del árbol.
*/
static void aux_liberar_memoria(struct trie *nodo){
    nodo->cantidad = 0;
    for (int i=0; i<26; i++){
        nodo->siguiente[i] = NULL;
        free(nodo->siguiente[i]);
    }
    free(nodo);
    nodo = NULL;
}

/**
 * @brief Elimina los nodos del árbol de manera recursiva.
 * @param nodo Puntero a un nodo del árbol.
*/
static void aux_multiset_eliminar(struct trie *nodo){
    //Si el puntero al nodo del arbol no es nulo, entonces.
    if (nodo!=NULL){
        //Un nodo puede llegar a tener, como mucho, 26 hijos (por cada letra del alfabeto, excluyendo la ñ).
        for (int i=0; i<26; i++){
            //Si el nodo i no es nulo, se sigue con el recorrido.
            if (nodo!=NULL){
                if (nodo->siguiente[i]!=NULL){
                //Sigo con el recorrido del árbol
                aux_multiset_eliminar(nodo->siguiente[i]);

                //Finalizado el recorrido, se procede a eliminar el nodo y su contenido.
                aux_liberar_memoria(nodo->siguiente[i]);
            }
            }
        }

    }
}

void multiset_eliminar(multiset_t **m){
    //Realiza la eliminación del multiset de manera recursiva, partiendo de la raiz del árbol trie.
    aux_multiset_eliminar(*m);
    //Libera el espacio reservado para el multiset y setea la referencia como NULL
    free(*m);
    *m = NULL;
}
