#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    //Si se ha especificado par�metros.
    if (argc>0){

    }
    else{
        printf("El programa cuentapalabras admite los siguientes par�metros opcionales:\n");
        printf("[-h] [directorio de entrada]: Dado el directorio especificado, el programa lee todos los archivos .txt y son procesados de manera que para cada uno de los archivos se cuenta la cantidad de veces que aparecen las palabras.");
    }

    return 0;
}
