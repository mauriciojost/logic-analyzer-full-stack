#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main(){
  FILE *fp_destino;
  FILE *pf;
	char letra=0;





  if (!(pf=fopen("origen.txt","r"))){
		printf("Error al abrir el archivo de origen.\n");
    exit(0);
  }else{
    letra = fgetc(pf);
    printf("Leido: '%c'.\n",letra);

  }

  fclose(pf);

  if (!(fp_destino=fopen("destino.txt","w"))){
  	printf("Error al abrir el archivo de destino.\n");
	  exit(0);
  }else{
		fputc(letra,fp_destino);
	}

  fclose(fp_destino);

	printf("Fin del programa.\n");
	return 0;
}












