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
unsigned char getFileType(char *ruta, struct dirent *ent);char *getFullName(char *ruta, struct dirent *ent);
char *generaPosStr(int niv);
unsigned cuentaArchivos(char *ruta, int niv);


typedef struct {
    char *fname;
} dato;

struct nodo{
    dato dato;
    struct nodo *panterior;
    struct nodo *psiguiente;
    
};
typedef struct nodo nodo;


struct nodo *crearNodo(char *nombre1){
    nodo* nuevoNodo;
    nuevoNodo = (nodo*)malloc(sizeof(nodo));
    nuevoNodo->dato.fname = nombre1;
    nuevoNodo->panterior = NULL;
    nuevoNodo->psiguiente = NULL;
    return nuevoNodo;
   
}

struct nodo *insertarFinal(nodo *nuevoNodo, nodo *final, nodo *cabeza){
    if (cabeza == NULL){
        final=nuevoNodo;
        nuevoNodo->psiguiente=nuevoNodo;
        nuevoNodo->panterior=nuevoNodo;
        printf ("\nprueba if%s", nuevoNodo->dato.fname);
    } else{
        final->psiguiente=nuevoNodo;
        nuevoNodo->panterior=final;
        cabeza->panterior=nuevoNodo;
        nuevoNodo->psiguiente=cabeza;
        final=nuevoNodo;
        printf ("\nprueba else%s", nuevoNodo->dato.fname);
        return final;
    }
    
    
}

void visualiza(dato miDato){
    printf("\n%s",miDato.fname);
}

void listar(nodo *cabeza, nodo *final){
    if (cabeza == NULL) {
        printf ("lista Vacia");
    } else{
         nodo *siguiente= NULL;
         siguiente = cabeza->psiguiente;
         visualiza(cabeza->dato);
         while (siguiente != cabeza){
             visualiza(siguiente->dato);
             
         }
    }
}

int eliminar (nodo *sup_elemento){
  if (sup_elemento == NULL){
      printf ("No hay ningún elemento en la lista");
  } else{
      sup_elemento->panterior->psiguiente = sup_elemento->psiguiente;
      sup_elemento->psiguiente->panterior = sup_elemento->panterior;
       //free (sup_elemento->dato);
       free (sup_elemento);
  }
  return 0;
}

void limpiar(nodo *cabeza, nodo *final){
    if (cabeza == NULL) {
        printf ("lista Vacia");
    } else{
         nodo *siguiente= NULL;
         siguiente = cabeza->psiguiente;
         eliminar(cabeza);
         while (siguiente != cabeza){
             eliminar(siguiente);
             
         }
    }
}

nodo *cargar(char * ruta){
     nodo *cabeza= NULL;
    nodo *final=NULL;
 /* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;

  /* Empezaremos a leer en el directorio actual */
  dir = opendir (ruta);

  /* Miramos que no haya error */
  if (dir == NULL) 
    error("No puedo abrir el directorio");
  
  /* Una vez nos aseguramos de que no hay error, ¡vamos a jugar! */
  /* Leyendo uno a uno todos los archivos que hay */
  while ((ent = readdir (dir)) != NULL) 
    {
      /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
          /*.=ruta  // ..=ruta/..*/
    {
      /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
          printf ("\n%s", ent->d_name);
          final=insertarFinal(crearNodo(ent->d_name),final,cabeza);
          if (cabeza == NULL){
              cabeza = final;
          }
          
    }
    }
  return cabeza;
}


int main(int argc, char *argv[]){
    int opcion;
    
    nodo *cabeza= NULL;
    nodo *final=NULL;
    nodo *actual=NULL;
    
   /* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;

  /* Empezaremos a leer en el directorio actual */
  dir = opendir (".");

  /* Miramos que no haya error */
  if (dir == NULL) 
    error("No puedo abrir el directorio");
  
  /* Una vez nos aseguramos de que no hay error, ¡vamos a jugar! */
  /* Leyendo uno a uno todos los archivos que hay */
  while ((ent = readdir (dir)) != NULL) 
    {
      /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    {
      /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
         // printf ("\n%s", ent->d_name);
          final=insertarFinal(crearNodo(ent->d_name),final,cabeza);
          if (cabeza == NULL){
              cabeza = final;
          }
          actual=cabeza;
    }
    }
      do  { 
    //printf("El valor actual es %s", actual->dato.fname);
    printf("\n\nElija una de estas opciones del menú:\n ");
    printf("1-Avanzar en la lista\n");
    printf("2-Retroceder en lista\n");    
    printf("3-Eliminar un elemento de la lista\n");
    printf("4-Limpiar todas las imagenes de la lista\n");
    printf("5-Cargar todos los elementos de la LIsta\n");
    printf("0-Salir \n ");
    
    printf("\n\nIntroduzca su opcion: ");
    scanf("%d",&opcion);
    
        switch(opcion){
            case 1:
                actual=actual->psiguiente;
                break;
            case 2:
                actual=actual->panterior;
                break;
            case 3:
                eliminar(actual);
                break;
            case 4:
                limpiar(cabeza,final);
                break;
            case 5:
                char * ruta;
                printf ("introduzca una ruta");
                scanf ("%s", ruta);
                cabeza=cargar(ruta);
                actual=cabeza;
                final=cabeza->panterior;
           
                break;
                
            case 0:
                printf ("Valor incorrecto\n");
                break;
        default:
                printf("\nHa ingresado un numero no valido, introduzca una de las opciones.\n");
        }
    } while (opcion!=0);
    
    printf("\n Pulse una tecla para finalizar");
    scanf("%c");
  closedir (dir);
  listar(cabeza,final);
    return EXIT_SUCCESS;
}

void error(const char *s)
{
    perror (s);
    exit(EXIT_FAILURE);
}







/*
char *getFullName(char *ruta, struct dirent *ent){
    char *nombrecompleto;
    int tmp;
    
    tmp=strlen(ruta);
    nombrecompleto=malloc(tmp+strlen(ent->d_name)+2);
    if (ruta[tmp-1]=='/')
        sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
    else
        sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);
    
    return nombrecompleto;
}

char *generaPosStr(int niv)
{
    int i;
    char *tmp=malloc(niv*2+1);
    for (i=0; i<niv*2; ++i)
        tmp[i]=' ';
    tmp[niv*2]='\0';
    return tmp;
}


unsigned cuentaArchivos(char *ruta, int niv){
    DIR *dir;
    struct dirent *ent;
    unsigned numfiles=0;
    unsigned char tipo;
    char *nombrecompleto;
    char *posstr;
    dir = opendir (ruta);
    
    
    if (dir == NULL)
        error("No puedo abrir el directorio");
    
    while ((ent = readdir (dir)) != NULL)
    {
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
        {
            nombrecompleto=getFullName(ruta, ent);
            tipo=getFileType(nombrecompleto, ent);
            if (tipo==DT_REG)
            {
                ++numfiles;
            }
            else if (tipo==DT_DIR)
            {
                posstr=generaPosStr(niv);
                printf("%sEntrando en: %s\n", posstr, nombrecompleto);
                printf("%s%s . Total: %u archivos ", posstr, nombrecompleto, cuentaArchivos(nombrecompleto, niv+1));
                printf("\n");
                free(posstr);
            }
            free(nombrecompleto);
        }
    }
    closedir (dir);
    
    return numfiles;
}

unsigned char getFileType(char *nombre, struct dirent *ent)
{
    unsigned char tipo;
    
    tipo=ent->d_type;
    if (tipo==DT_UNKNOWN){
        tipo=statFileType(nombre);
    }
    return tipo;
}

unsigned char statFileType(char *fname)
{
    struct stat sdata;
    
    if (stat(fname, &sdata)==-1){
        return DT_UNKNOWN;
    }
    
    switch (sdata.st_mode & S_IFMT)
    {
        case S_IFBLK:  return DT_BLK;
        case S_IFCHR:  return DT_CHR;
        case S_IFDIR:  return DT_DIR;
        case S_IFIFO:  return DT_FIFO;
        case S_IFLNK:  return DT_LNK;
        case S_IFREG:  return DT_REG;
        case S_IFSOCK: return DT_SOCK;
        default:       return DT_UNKNOWN;
    }
}*/