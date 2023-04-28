#include <stdio.h>
#include <stdlib.h>
#include "multiset.h"
#include "listaordenada.h"

int main(int argc, char *argv[]){

    //Por defecto, siempre se pasa un parámetro que es el directorio en donde se ejecuta el programa.
    //Por lo tanto, argc es 1 o mayor a 1.
    if (argc==1){
        printf("Bienvenido al programa cuentapalabras. El programa admite los siguientes parametros opcionales:\n\n");
        printf("[-h] [directorio de entrada]: Dado el directorio de archivos de texto, se procesa cada archivo contabilizando las palabras de cada uno de los archivos.\n");
        printf("  -Genera un archivo 'cadauno.out' que contiene la cantidad de veces que aparece cada palabra en en cada uno de los archivos.\n");
        printf("  -Genera un archivo 'totales.out' que contiene la cantidad de veces que aparece cada palabra entre todos los archivos.\n");
    }
    else{
        //Si se ha definido algún parámetro
        printf("Cantidad de parametros: %i\n", argc);
        printf("%s", argv[1]);
    }

    return 0;
}
