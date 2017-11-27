#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/p.h"
#include "src/TratamentoPPM.h"



int main (int argc, char *argv[]){

//----
	char *arg2 = argv[2];
	char *imagem = "images/";
	char Nome[20];
	Nome[0] = '\0';

	strcat(Nome,imagem);//concatenar strings
	strcat(Nome,arg2);


	FILE *file = readFile(Nome); /* TROCAR ENTRADA PADRÃO APÓS OS TESTES */
	Image *image = getImage(file);

	FILE *file1 = readFile(Nome); /* TROCAR ENTRADA PADRÃO APÓS OS TESTES */
	Image *image1 = getImage(file1);

//Retorno com dimensões do arquivo
	printf ("<==================Dimensões da imagem==================>\n");
	printf("Largura: %d\n", (*image).width);
	printf("Altura: %d\n", (*image).height);

	Image * GS = grayScale(image);
	saveImage ("diagnostico/GrayScale.ppm", GS ,255);

	Image * FG = FiltroGaussiano (GS);
	saveImage ("diagnostico/Gauss",FG,255);

	Image * FS = FiltroSobel (FG);
	saveImage ("diagnostico/Sobel",FS,255);

	Image * Bin = binario(FS);
	saveImage ("diagnostico/Bin",Bin,1);

	Image * Hough = filtroHough (Bin,image1);
	saveImage ("diagnostico/Hough",Hough,255);

	return 0;
}
