/**
* @file cuentapalabras.h
* @brief Encabezado del TDA Cuenta Palabras.
* Modela las operaciones necesarias para poder leer un directorio de archivos .txt y
poder contabilizar la cantidad de palabras que hay, y posteriormente plasmar dicha
información en archivos de salida.
*
* @author Comisión N°17 (David Emanuel Latouquette - Otto Krause)
*/

#include <dirent.h> //Utilizada para obtener los archivos de los directorios.

#define CUENTAPALABRAS_ERROR_CONTADOR           -5
#define CUENTAPALABRAS_ARCHIVOS_GENERADOS       -6
#define CUENTAPALABRAS_ERROR_APERTURA_ARCHIVO   -7
#define MAXCHAR 5000

/**
 * @brief Abre el directorio indicado en path y devuelve un puntero al manejador de archivos de dicho directorio.
 * @param path Puntero a una cadena de caracteres que indica una ruta a un directorio.
 * @return Puntero al manejador de archivos del directorio en caso de que la ruta sea válida, en caso contrario, retorna NULL (y por lo tanto, la ruta dada será errónea).
*/
extern DIR* cuentapalabras_abrir_directorio(char *path);

/**
 * @brief Recupera un arreglo de todos los nombres de archivos de texto que hay en el directorio apuntado por d.
 * @param d Puntero a DIR.
 * @param cant_filas Puntero a entero que representará la cantidad de filas.
 * @return Puntero a puntero de cadenas de caracteres (relativo a un arreglo bidimensional de chars).
*/
extern char** cuentapalabras_recopilar_nombres_archivos_txt(DIR *d, int*cant_filas);

/**
 * @brief Libera la memoria reservada para un puntero de punteros a cadenas de caracteres.
 * @param C Puntero a punteros de cadenas de caracteres.
 * @param cant_filas Cantidad de filas (o punteros a punteros) en C.
*/
extern void cuentapalabras_liberar_memoria_nombres_archivos(char** C, int cant_filas);

/**
 * @brief Realiza la lectura de todos los archivos de texto, contabiliza todas sus palabras y genera dos archivos de salida.
 * @param nombre_archivo Puntero a punteros de cadena de caracteres.
 * @param cant_filas Cantidad de filas que hay en nombre_archivo.
*/
extern void cuentapalabras_construir_archivos_salida(char* directorio, char** nombre_archivo, int cant_filas);
