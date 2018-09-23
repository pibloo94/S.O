/*
 * ======================================
 * Autores: Manuel Monforte & Pablo Agudo
 * Dudas:
 * 	-->Como ejecutar con flag -x
 * 	-->Metemos nulo (linea 163)*
 * 	-->	linea 194 porque implicito?¿
 *
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
static int copynFile(FILE * origin, FILE * destination, unsigned int nBytes){

	//escribimos byte a byte de origen a destino
	//Aprovechamos recursos del ejecricio 1.3
	int i = 0;
	int x;
	x = fgetc(origin);
	while ( x!= EOF && i < nBytes  ) {
		fputc(x, destination);
		x = fgetc(origin);
		i++;
	}
	// Complete the function
	return i;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file){
	int nBytes = 0;
	char *contenido;
	// bucle, lee byte a byte hasta encontrar \0, contando el número de bytes leídos (tam)
	char c = getc(file);
	while(c!='\0'){
		nBytes++;
		c = getc(file);
	}

	contenido =malloc(sizeof(char)*(nBytes+1));//reservamos espacio

	// fseek para volver tam bytes hacia atrás
	fseek(file,-(nBytes+1),SEEK_CUR);

	fread(contenido,sizeof(char),nBytes+1,file);
	return contenido;
}



/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
static stHeaderEntry* readHeader(FILE * tarFile, unsigned int *nFiles){
	stHeaderEntry * array = NULL ;

	/* ... Read the number of files ( N ) from tarfile and
	store it in nr_files ... */
	fread(nFiles,sizeof(int),1,tarFile);

	array = (stHeaderEntry*)malloc ( sizeof ( stHeaderEntry ) * (*nFiles)) ;
	for(int i = 0; i <  *nFiles;i++){
			array[i].name = (loadstr(tarFile));//rellenamos contenido
			fread(&array[i].size,sizeof(int),1,tarFile);//rellenamos tamanos
	}
	return array ;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[]){

	FILE *mtarFile;

	//1)Creamos fichero mtar y damos permisos wb
	if((mtarFile = fopen(tarName,"wb"))==NULL){
		return EXIT_FAILURE;
	}else{
		//2)Reservamos memoria para el array de cabezeras (tituloFichero + tamano)
		stHeaderEntry *headers = malloc ((nFiles)*sizeof(stHeaderEntry));// creamos un array de structuras stHeaderEntry
		if(headers==NULL){
			return EXIT_FAILURE;
		}
		//3)Creamos variable para obtener tamano cabezera
		int totalCadenas =0;
		for(int i = 0; i < nFiles;i++){
			//diferencia entre sizeof y strlen(fileNames[i])+1
			headers[i].name=(char *)malloc(strlen(fileNames[i])+1);// para cada estructura reservamos espacio el nombre anaidendo el caracter nulo
			strcpy(headers[i].name, fileNames[i]);
			totalCadenas +=  strlen(fileNames[i])+1;
		}
		int offData = sizeof(int) + (nFiles) *(sizeof(unsigned int)) + totalCadenas;

		//4)Una vez obtenido la cabezera nos posicionamos debajo ella y creamos variable File para ir
		//rellenando mtarFile con contenidos de ficheros
		fseek(mtarFile,offData,SEEK_SET);

		FILE *actual;

		for(int i = 0;i < nFiles;i++){
			actual = fopen(headers[i].name,"r");//abrimos archivo

			if(actual == NULL){
				fclose(mtarFile);//cerramos tar ya que archivo fallido
				return EXIT_FAILURE;
			}
			//si no falla copiamos contenido y actualizamos tamano
			headers[i].size = copynFile(actual,mtarFile,INT_MAX);
			fclose(actual);
		}

		//5)volvemos arriba para rellenar tamanos,escribimos numero de ficheros arriba
		fseek(mtarFile, 0, SEEK_SET);
		fwrite(&nFiles,sizeof(int),1,mtarFile);

		//6)escribimos tamanos de ficheros
		for(int i = 0; i < nFiles;i++){
			fwrite(headers[i].name,sizeof(char),strlen(fileNames[i]),mtarFile);//escribimos nombre
			fputc('\0',mtarFile);
			fwrite(&headers[i].size,sizeof(int),1,mtarFile);//escribimos tamano

		}
	}
	fclose(mtarFile);
	//free(headers);
	printf("Fichero mitar creado con exito\n");
	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[]){
	// Complete the function

	FILE *ptarName;
	ptarName =fopen(tarName,"r");
	//si el fichero no es nulo, rellenamos pheaders y vamos extrayendo informacion y creando ficheros
	if(ptarName!=NULL){
		unsigned int numFiles;

		stHeaderEntry *pheaders;
		pheaders = readHeader(ptarName,&numFiles);
		if(pheaders!=NULL){
			FILE *nuevo;
			for(int i = 0; i < numFiles;i++){
				nuevo =fopen(pheaders[i].name,"w");
				copynFile(ptarName,nuevo,pheaders[i].size);
				fclose(nuevo);
				printf("Extracion de Tar en proceso %d/%d \n",i+1,numFiles);
			}
			printf("Extracion de Tar completada");
			return EXIT_SUCCESS;
		}else{
			fclose(ptarName);
			err(5,"Archivo no valido");
			return EXIT_FAILURE;
		}
	}else{// si es nulo cerramos fichero y salimos
	//escribimos mensaje error?¿
		fclose(ptarName);
		err(4,"Archivo no valido");
		return EXIT_FAILURE;
	}

}
