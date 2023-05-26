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
#include <string.h>
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
        exit(ERROR_LISTA_MEMORIA);
    }
    //Actualiza los atributos de la lista para modelar una lista vacía.
    L->primera = NULL;
    L->cantidad = 0;

    return L;
}

static celda_t * aux_construir_celda(elemento_t elem, celda_t * celda_siguiente){
    ///Reservo memoria para la celda nueva.
    celda_t * celda_nueva = (struct celda*) malloc(sizeof(struct celda));
    ///Si no se ha reservado memoria para la nueva celda, salir del programa.
    if (celda_nueva==NULL){
        exit(ERROR_LISTA_MEMORIA);
    }
    ///Asignar los valores correspondientes.
    celda_nueva->elem = &elem;
    celda_nueva->siguiente = celda_siguiente;

    return celda_nueva;
}


int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos){
    int valor_retornar = TRUE;
    int cantidad_elementos = l->cantidad;
    celda_t * celda_nueva;
    celda_t * celda_siguiente;
    celda_t * celda_actual;

    ///Si la posicion es menor o igual a la cantidad de elementos se puede insertar elemento.
    if (pos<=(cantidad_elementos)){
        celda_actual = l->primera;

        ///Si hay elementos y se inserta al principio de la lista.
        if (pos==0){
            //Caso 1: Cantidad y pos en 0.
            if (cantidad_elementos==0){
                celda_nueva = aux_construir_celda(elem, NULL);
                l->primera = celda_nueva;
                printf("Primer caso. Elemento (%d, %s)\n", celda_nueva->elem->a, celda_nueva->elem->b);
            }
            else{
                //Caso 2: pos==0 y cantidad>0
                celda_nueva = aux_construir_celda(elem, celda_actual);
                l->primera = celda_nueva;
                printf("Segundo caso. Elemento (%d, %s)\n", celda_nueva->elem->a, celda_nueva->elem->b);
            }
        }
        else{
            //Caso 3: Si la posicion es la cantidad de elementos, es inserción al final de la lista.
            if (pos==cantidad_elementos){
                while(celda_actual!=NULL){
                    celda_actual = celda_actual->siguiente;
                }
                celda_nueva = aux_construir_celda(elem, NULL);
                celda_actual = celda_nueva;
                printf("Tercer caso. Elemento (%d, %s)\n", celda_nueva->elem->a, celda_nueva->elem->b);
            }
            else{
                //Caso 4: Si la posicion está es 0<pos<cantidad
                for (int i=1; i<pos; i++){
                    celda_actual = celda_actual->siguiente;
                }
                celda_siguiente = celda_actual->siguiente;
                celda_nueva = aux_construir_celda(elem, celda_siguiente);
                celda_actual->siguiente = celda_nueva;
                printf("Cuarto caso. Elemento (%d, %s)\n", celda_nueva->elem->a, celda_nueva->elem->b);
            }
        }
        l->cantidad = cantidad_elementos+1;
    }

    else{
        valor_retornar = ERROR_LISTA_BUSQUEDA;
    }

    return valor_retornar;
}

elemento_t *lista_eliminar(lista_t *l, unsigned int pos){
    elemento_t *elemento_eliminado = NULL;
    ///Buscar solo si la posicion no excede el tamaño de la lista.
    if (pos<l->cantidad && l->cantidad>0){
        celda_t * celda_eliminar = l->primera;
        celda_t * celda_siguiente;
        celda_t * celda_anterior;

        if (pos==0){
            l->primera = NULL;
            l->primera = celda_eliminar->siguiente;
        }
        else{
            for (int i=1; i<pos; i++){
                celda_eliminar = celda_eliminar->siguiente;
            }

            ///Almacena la celda anterior a la que se debe eliminar (posicion indirecta).
            celda_anterior = celda_eliminar;
            celda_siguiente = (celda_eliminar->siguiente)->siguiente;
            celda_eliminar = celda_eliminar->siguiente;
        }

        ///Remueve toda referencia de la celda a eliminar.
        elemento_eliminado = celda_eliminar->elem;
        celda_eliminar->siguiente = NULL;
        celda_eliminar->elem = NULL;

        ///Asigno las nuevas referencias a la celda anterior y siguiente.
        celda_anterior->siguiente = celda_siguiente;

        ///Disminuye en 1 el contador de elementos de la lista.
        l->cantidad = l->cantidad - 1;

        ///Finalmente se libera la memoria reservada para la celda.
        free(celda_eliminar);
    }

    printf("lista_eliminar::elemento=%d\n", elemento_eliminado->a);

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
            for(int posicion_actual=1; posicion_actual<pos; posicion_actual++){
                celda_actual = celda_actual->siguiente;
            }
            //Almacena la celda del elemento.
            celda_elemento = celda_actual->siguiente;
        }

        ///Recupera el elemento a retornar.
        elemento_a_retornar = celda_elemento->elem;
    }

    return elemento_a_retornar;
}

elemento_t *ordenar_auxiliar(celda_t *celda_actual, int pos_inicial, int pos_final, funcion_comparacion_t comparar){
    //Si se llegó al final
    if (pos_inicial==pos_final){
        return celda_actual->elem;
    }
    else{
        celda_t *celda_sgte = celda_actual->siguiente;
        elemento_t * elem_actual = celda_actual->elem;
        elemento_t * elem_ordenar = ordenar_auxiliar(celda_actual->siguiente, pos_inicial+1, pos_final, comparar);

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
