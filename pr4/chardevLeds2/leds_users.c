#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <ctype.h>

void contadorBinario(int fd);
void circular(int fd);

int main(){
	int opcion = -1;
	//permisos de escritura y lectura
	int fd = open("/dev/chardevLeds",O_RDWR);
	if(fd ==-1){
		printf("Fallo al leer el dispositivo\n");
		return 0;
	}
	while(opcion!=0){
		printf("============JUEGOS DE LEDS============\n");
		printf(" 1-->Secuencia circular\n");
		printf(" 2-->Contador binario\n");
		printf(" 0-->Salir del programa\n");
		//abrimos fichero, si no se abre terminamos ejecucion del programa
		
		do {
			printf(" Introduzca una de las secuencia anteriores\n");
			scanf("%d",&opcion);
		} while(opcion < 0 && opcion > 2);

		switch(opcion){
			case 1: circular(fd); break;
			case 2: contadorBinario(fd); break;
		}
	}
	//cerramos fichero
	close(fd);
	return 0;

}

void contadorBinario(int fd){
	int t = 0;
	char *contador;
	int bytesPintados;
	for(t = 0; t < 8; t++){
		switch(t){
			case 0: {contador = "";}break;	
			case 1: {contador = "3";}break;	
			case 2: {contador = "2";}break;	
			case 3: {contador = "23";}break;	
			case 4: {contador = "1";}break;	
			case 5: {contador = "13";}break;	
			case 6: {contador = "12";}break;	
			case 7: {contador = "123";}break;
		}
		//pintamos leds
		bytesPintados = write(fd,contador,strlen(contador));	
		if(bytesPintados < 0){
			printf("Error al colorear bytes\n");
			return;		
		}
		usleep(1000000);//1 seg de espera cada iteracion
	}	
}

void circular(int fd){
	int t = 0;
	int vueltas = -1;
	char *contador;
	int bytesPintados;
	printf("Seleccione el numero de vueltas (1-20):\n");

	//leemos numero de vueltas
	do {
	scanf("%d",&vueltas);
	} while(vueltas < 1 && vueltas > 20);


	for(t = 1; t <= 3*vueltas; t++){
		if(t%1 == 0) contador ="1"; // *..
		if(t%2 == 0) contador ="2"; // .*.
		if(t%3 == 0) contador ="4"; // ..*

		//pintamos leds
		bytesPintados = write(fd,contador,strlen(contador));	
		if(bytesPintados < 0){
			printf("Error al colorear bytes\n");
			return;		
		}
		usleep(1000000);//1 seg de espera cada iteracion
	}	
}

