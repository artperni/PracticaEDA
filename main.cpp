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

struct nodo{
    
};

int main(int argc, char *argv[]){
    unsigned num;
    
    if (argc != 2){
        error("Uso: ./directorio_2 <ruta>\n");
    }
    printf("Entrando en: %s\n", argv[1]);
    num=cuentaArchivos(argv[1], 1);
    printf("%s . Total: %u archivos\n", argv[1], num);
    return EXIT_SUCCESS;
}

void error(const char *s)
{
    perror (s);
    exit(EXIT_FAILURE);
}

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
}