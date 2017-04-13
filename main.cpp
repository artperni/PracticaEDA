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

typedef struct {
    char *fname;
} dato;

struct nodo {
    dato dato;
    struct nodo *panterior;
    struct nodo *psiguiente;

};
typedef struct nodo nodo;

struct nodo *crearNodo(char *nombre1) {
    nodo* nuevoNodo;
    nuevoNodo = (nodo*) malloc(sizeof (nodo));
    nuevoNodo->dato.fname = strdup(nombre1);
    nuevoNodo->panterior = NULL;
    nuevoNodo->psiguiente = NULL;
    return nuevoNodo;

}

struct nodo *insertarFinal(nodo *nuevoNodo, nodo * final, nodo *cabeza) {
    if (cabeza == NULL) {
        final = nuevoNodo;
        nuevoNodo->psiguiente = nuevoNodo;
        nuevoNodo->panterior = nuevoNodo;
        //printf ("\nprueba if%s", nuevoNodo->dato.fname);
    } else {
        final->psiguiente = nuevoNodo;
        nuevoNodo->panterior = final;
        cabeza->panterior = nuevoNodo;
        nuevoNodo->psiguiente = cabeza;
        final = nuevoNodo;
        //printf ("\nprueba else%s", nuevoNodo->dato.fname);
    }
    return final;
}

void visualiza(dato miDato) {
    printf("\n%s", miDato.fname);
}

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

int main(int argc, char *argv[]) {
    int opcion;

    nodo *cabeza = NULL;
    nodo * final = NULL;
    nodo *actual = NULL;

    char ruta[250] = ".";

    cabeza = cargar(ruta, final, cabeza);
    final = cabeza->panterior;

    actual = cabeza;
    do {
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
        printf("0-Salir \n ");

        printf("\n\nIntroduzca su opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                actual = actual->psiguiente;
                break;
            case 2:
                actual = actual->panterior;
                break;
            case 3:
                eliminar(actual);
                break;
            case 4:
                limpiar(cabeza, final);
                break;
            case 5:
                printf("\nIntroduzca una ruta: ");
                scanf("%s", ruta);
                cabeza = cargar(ruta, final, cabeza);
                actual = cabeza;
                final = cabeza->panterior;
                break;
            case 6:
                listar(cabeza, final);
                break;

            default:
                printf("\nHa ingresado un numero no valido, introduzca una de las opciones.\n");
        }
    } while (opcion != 0);

    printf("\n Pulse una tecla para finalizar");
    scanf("%c");
    return EXIT_SUCCESS;
}

void error(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}
