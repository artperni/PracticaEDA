//
//  main.c
//  EDA_Arturo-Guillermo
//
//  Created by Guillermo Peñas Alonso on 21/3/17.
//  Copyright © 2017 Guillermo Peñas Alonso. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void error(const char *s);
long fileSize(char *fname);
unsigned char statFileType(char *fname);
unsigned char getFileType(char *ruta, struct dirent *ent);
char *getFullName(char *ruta, struct dirent *ent);
char *generaPosStr(int niv);
unsigned cuentaArchivos(char *ruta, int niv);

//DEfinición de los registros para los diferentes tipos de datos del programa
typedef struct {
    char *fname;
} dato;

struct nodo {
    dato dato;
    struct nodo *panterior;
    struct nodo *psiguiente;

};
typedef struct nodo nodo;

//Estructura de tipo nodo que permite la creación de un nuevo nodo
struct nodo *crearNodo(char *nombre1) {
    nodo* nuevoNodo;
    nuevoNodo = (nodo*) malloc(sizeof (nodo));
    nuevoNodo->dato.fname = strdup(nombre1);
    nuevoNodo->panterior = NULL;
    nuevoNodo->psiguiente = NULL;
    return nuevoNodo;

}
//Estructura de tipo nodo que permite insertar por el final de la lista un nuevo nodo
struct nodo *insertarFinal(nodo *nuevoNodo, nodo * final, nodo *cabeza) {
    if (cabeza == NULL) {
        final = nuevoNodo;
        nuevoNodo->psiguiente = nuevoNodo;
        nuevoNodo->panterior = nuevoNodo;
        
    } else {
        final->psiguiente = nuevoNodo;
        nuevoNodo->panterior = final;
        cabeza->panterior = nuevoNodo;
        nuevoNodo->psiguiente = cabeza;
        final = nuevoNodo;
       
    }
    return final;
}
//Procedimiento que permite visualizar los datos contenidos en el fichero
void visualiza(dato miDato) {
    printf("\n%s", miDato.fname);
}
// procedimiento que permite listar los elementos y llamar a visualizar para su visualizamiento
void listar(nodo *cabeza, nodo * final) {
    if (cabeza == NULL || final == NULL) {
        printf("\nLa lista esta vacia\n");
    } else {
        visualiza(cabeza->dato);
        nodo *pSiguiente = cabeza->psiguiente;
        while (pSiguiente != cabeza) {
            visualiza(pSiguiente->dato);
            pSiguiente = pSiguiente->psiguiente;
        }
    }
}
//Función para eliminar un elemento de la lista
int eliminar(nodo *sup_elemento) {
    if (sup_elemento == NULL) {
        printf("No hay ningún elemento en la lista");
    } else {
        sup_elemento->panterior->psiguiente = sup_elemento->psiguiente;
        sup_elemento->psiguiente->panterior = sup_elemento->panterior;
        //free (sup_elemento->dato);
        free(sup_elemento);
    }
    return 0;
}
//Procedimiento para eliminar todos los elementos de la lista
void limpiar(nodo *cabeza, nodo * final) {
    if (cabeza == NULL || final == NULL) {
        printf("\nLa lista esta vacia\n");
    } else {
        nodo *pSiguiente = cabeza->psiguiente;
        while (pSiguiente != cabeza) {
            eliminar(pSiguiente);
            pSiguiente = pSiguiente->psiguiente;
        }
        eliminar(cabeza);
    }
}
//Estructura de tipo nodo que permite cargar los elementos de un directorio
nodo *cargar(char * ruta, nodo * final, nodo *cabeza) {

    /* Con un puntero a DIR abriremos el directorio */
    DIR *dir;
    /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    struct dirent *ent;

    /* Empezaremos a leer en el directorio actual */
    dir = opendir(ruta);

    /* Miramos que no haya error */
    if (dir == NULL)
        error("No puedo abrir el directorio");

    /* Una vez nos aseguramos de que no hay error, ¡vamos a jugar! */
    /* Leyendo uno a uno todos los archivos que hay */
    while ((ent = readdir(dir)) != NULL) {
        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            // printf ("\n%s", ent->d_name);
            final = insertarFinal(crearNodo(ent->d_name), final, cabeza);
            if (cabeza == NULL) {
                cabeza = final;
            }

        }
    }

    return cabeza;
}

int main(int argc, char *argv[]) {//Lectura del directorio
                                  //Menú de opciones mediante un switch-case
    int opcion; //
    // inciación de los nodos
    nodo *cabeza = NULL;
    nodo * final = NULL;
    nodo *actual = NULL;

    char ruta[250] = ".";//permite introducir la ruta a las imaganes que queremos utilizar

    cabeza = cargar(ruta, final, cabeza);//inicialización del puntero cabeza "cargar"
    final = cabeza->panterior;

    actual = cabeza;
    do { //Mediante este bucle, se muestran todas las opciones posibles para el usuario
        if (actual != NULL)
            printf("\nFichero actual: %s", actual->dato.fname);
            //printf("test");
        else
            printf("\n La lista esta vacia");
        printf("\nElija una de estas opciones del menú:\n ");
        printf("1-Avanzar en la lista\n");
        printf("2-Retroceder en lista\n");
        printf("3-Eliminar un elemento de la lista\n");
        printf("4-Limpiar todas las imagenes de la lista\n");
        printf("5-Cargar todos los elementos de la LIsta\n");
        printf("6-|<--|Muestra por pantalla todos los elementos de la lista\n");
        printf("0-Finalizar la ejecucion del programa\n ");

        printf("\n\nIntroduzca su opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {//En función de la opción escogida se ejecuta una determinada opción case
            case 1://Permite avanzar en la lista, de este modo el puntero actual apuntará al puntero siguiente
                if (cabeza == NULL  || actual==actual->psiguiente)
                    printf("\nAvanzar no disponible");
                else
                    actual = actual->psiguiente;
                break;
            case 2://Permite retrocedes en la lista, de este modo el puntero actual apuntará al puntero anterior
                if (cabeza == NULL  || actual==actual->panterior)
                    printf("\nRetroceder no disponible");
                else
                    actual = actual->panterior;
                break;
            case 3://Permite eliminar un elemento de la lista, de este modo se elimina el puntero actual
                nodo* siguiente = actual->psiguiente;
                eliminar(actual);
                actual = siguiente;
                break;
            case 4://Elimina todas las imágenes de la lista, para ello apunta en la lista desde cabeza a final
                limpiar(cabeza, final);
                break;
            case 5://Permite la carga de las imágenes almacenadas en el directorio indicado, para ello necesita la direccion de la ruta, que luego alamacena apunta tanto al puntero cabeza desde el final, como al actual y a final
                printf("\nIntroduzca una ruta: ");
                scanf("%s", ruta);
                cabeza = cargar(ruta, final, cabeza);
                actual = cabeza;
                final = cabeza->panterior;
                break;
            case 6://Muestra por pantalla todos los elementos de la lista, devolviendonos al menú de opciones
                listar(cabeza, final);
                break;

            default://En caso de que el numero introducido no sea valido, se mostrara el siguiente mensaje de error
                printf("\nHa ingresado un numero no valido, introduzca una de las opciones.\n");
        }
    } while (opcion != 0);

    printf("\n Pulse una tecla para finalizar");
    scanf("%c");
    return EXIT_SUCCESS;
}

void error(const char *s) {//En caso de error se mostrará el puntero a error
    perror(s);
    exit(EXIT_FAILURE);
}
