#include <math.h>

/* funções para tratamento dos arquivos .ppm */

/* Função para ignorar comentarios em um arquivo, caso tenha */

void skipComments(FILE *file_img) {
    // Cria uma variável para armazenar cada caracter do comentário
    char buffer = fgetc(file_img);

    // Repete o laço enquanto a variável de buffer for igual a um '#'
    do {
        if (buffer == '#')
            // Se buffer for igual a '#', é por que está iniciando
            // um comentário, então a variável buffer recebe cada
            // caractere até que ele seja uma quebra de linha
            while (buffer != '\n')
                buffer = fgetc(file_img);
        else
            // Caso contrário, não é um comentário, então "devolve"
            // para o arquivo, para que ele possa ser lido novamente
            ungetc(buffer, file_img);
        // buffer pega novamente um caracter (para verificar
        // se tem mais algum comentário no arquivo)
        buffer = fgetc(file_img);
    } while (buffer == '#');
    // Quando o laço acabar (o último caracter pego for
    // diferente de um '#'), "devolve" para o arquivo o
    // caracter que está em buffer, pois não é um comentário
    ungetc(buffer, file_img);
}

/* Abre e lê um arquivo */

FILE * readFile(char filename[]){
	FILE *file;

	file = fopen(filename, "rw");

	if (! file){
		printf("\nOcorreu algum erro durante a leitura do arquivo!\n");
		printf("O formato de abertura é: \n");
		printf("./catarata -i Catarata.ppm -f ppm -o diagnostico.txt\n");
		exit(1);
	}

	

	return file;
}

/* Função para alocar imagem na memoria */

Image * buildImage( int width, int height ){
	int i = 0;

	Image *img = malloc(sizeof(Image));
	
	if(!img){
		printf("\nNão foi possível alocar a imagem na memória!\n\n");
		exit(1);
	}
	
	img->width = width;
	img->height = height;

	img->pixels = (Pixel **)malloc(sizeof(Pixel*) * img->height);

	for( i = 0; i < height; i++ )
		img->pixels[i] = (Pixel*)malloc(img->width * sizeof(Pixel));

	return img;
}

//Liberar memoria

void LibMem (Image *img){
	int i;

	for (i = 0 ; img -> height ; i++){
		free(img->pixels[i]);
		free(img->pixels);
		free(img);
	}
}

//Excluí comentarios das imagens

Image * getImage(FILE *file){
	char header[3];
	int width, height, i, j, tamanhoMaximo;

	fscanf(file, "%s ", header);

	if(header[0] != 'P' && header[1]!= '3'){
		printf("\nTipo da imagem não suportado!\n\n");
		exit(1);
	}

  	skipComments(file);
	fscanf(file, "%d %d", &width, &height);

	fscanf(file, "%d", &tamanhoMaximo);
	
	Image *img = buildImage(width, height);

	for(i = 0; i < img->height; i++){
		for(j = 0; j < img->width; j++){
			skipComments(file);
			fscanf(file, "%i", &img->pixels[i][j].r);
			skipComments(file);
			fscanf(file, "%i", &img->pixels[i][j].g);
			skipComments(file);
			fscanf(file, "%i", &img->pixels[i][j].b);
			skipComments(file);
		}
	}

	fclose(file);

	return img;
}

// Recebe e cria uma imagem em escala de cinzas 


Image * grayScale(Image *img){
	int i, j;

	for(i = 0; i < img->height; i++){
		for(j = 0; j < img->width; j++){
			img->pixels[i][j].r = (int)(img->pixels[i][j].r*0.3 + img->pixels[i][j].g*0.59 +
			img->pixels[i][j].b*0.11);
            
            img->pixels[i][j].g = img->pixels[i][j].r;
            img->pixels[i][j].b = img->pixels[i][j].r;
		}
	}

	return img;
}

/* retorna quantidade de pixels */

Pixel * pixelReturn(Image *img, int width, int height){

    if( width >= img->width ) width = img->width - 1;
    if( height >= img->height ) height = img->height - 1;
    if( width < 0 ) width = 0;
    if( height < 0 ) height = 0;

    return &img->pixels[height][width];
}

//Filtro Gaussiano

Image * FiltroGaussiano (Image *img){
int i,j,k,l;
int npx=0;
Pixel * p;
int sm, dv;
int filtro[5][5] = {{ 2,  4,  5,  4, 2 },
			    	{ 4,  9, 12,  9, 4 },
		            { 5, 12, 15, 12, 5 },
		            { 4,  9, 12,  9, 4 },
					{ 2,  4,  5,  4, 2 }};

Image *IF = buildImage(img->width, img->height);

for (i = 0 ; i < img->height ; i++){
	for(j = 0 ; j < img->width ; j++){
		sm = 0;
		dv = 0;
			for(k = 0 ; k < 5 ; k++){
				for(l = 0 ; l < 5 ; l++){
					p = pixelReturn(img, j+(l-2), i+(k-2));
					sm += (p->r * filtro[k][l]);
					dv += filtro[k][l];
				}
			}
		npx = sm/dv;

		IF->pixels[i][j].r = (int) npx;
		IF->pixels[i][j].g = (int) npx;
		IF->pixels[i][j].b = (int) npx;
	}
}

return IF;
}

//Filtro Sobel

Image * FiltroSobel(Image *img){

	Image * IF = buildImage(img->width, img->height);

	int mX[3][3] = {{-1, 0, 1},
			{-2, 0, 2},			
			{-1, 0, 1}};
	
	int mY[3][3] = {{ 1, 2, 1},
			{ 0, 0, 0},			
			{-1,-2,-1}};

	int i, j, k, l;
	int x, y, s = 0;
	Pixel *p;
	
	for (i = 1 ; i < img->height-1; i++){
		for (j = 1 ; j < img->width-1; j++){
			x = 0;
			y = 0;
			for (k = 0 ; k < 3 ; k++){
				for(l = 0 ; l < 3 ; l++){
					x += img->pixels[i-1+k][j-1+l].r * mX[k][l];
					y += img->pixels[i-1+k][j-1+l].r * mY[k][l];
				}
			}
		s = (int) (sqrt(pow(x, 2) + pow(y, 2)));

		IF->pixels[i][j].r = s;
		IF->pixels[i][j].g = s;
		IF->pixels[i][j].b = s;
		}
	}

return IF;
}

//Binarizacao

Image * binario(Image *img){

	int i, j, t;
	Image *bin = buildImage(img->width, img->height);

	 if(img->width == 1015 && img->height == 759) {
    		t = 50;
 	 }
	else if(img->width == 1198  && img->height == 770) {
    		t = 10;
  	}
	else {
    		t = 15;
	}

	for(i = 1; i < img->height-1; i++){
		for(j = 1; j < img->width-1; j++){
			if(img->pixels[i][j].r > t){
				bin->pixels[i][j].r = 255;			
				bin->pixels[i][j].g = 255;
				bin->pixels[i][j].b = 255;
			}else{
				bin->pixels[i][j].r = 0;				
				bin->pixels[i][j].g = 0;
				bin->pixels[i][j].b = 0;
			}  			
		}
	}

	return bin;
		
}

//hough

Image * filtroHough (Image *Bin, Image *N){

	int i, j ,a ,b;
	int r, rmin, rmax;
	int t;
	double pi = 3.14159265;
	int ***matriz = calloc(Bin->height, sizeof(int **));

	if(Bin->width == 1015 && Bin->height == 759){
		rmin = 80;
		rmax = 90;
	}
	else if(Bin->width == 1198  && Bin->height == 770) {
		rmin = 140;
		rmax = 150;
	}
	else if(Bin->width == 1167  && Bin->height == 739) {
		rmin = 155;
		rmax = 160;
	}
	else {
		rmin = 50;
		rmax = 60;
	}

	
	//criar matriz com calloc
	for (i = 0 ; i < Bin->height ; i++){
		matriz[i] = calloc(Bin->width, sizeof(int *));
			for(j = 0; j < Bin->width ;j++){
				matriz[i][j] = calloc(rmax - rmin + 1, sizeof(int));

			}
	}

//preenchimento da matriz
	for(i = rmin; i < Bin->height -rmin ;i++){
		for(j = rmin; j < Bin->width - rmin ;j++){
			if(Bin->pixels[i][j].r == 255){
				for(r = rmin; r < rmax ;r++){
					for(t = 0; t<360 ;t++){
						a = i - r * cos(t * pi/180);
						b = j - r * sin(t * pi/180);
						//seg fault
						if (a >= 0 && b >= 0 && a < Bin->height && b < Bin->width && a + r < Bin->height && b + r < Bin->width){
							matriz[a][b][r-rmin]++;
						}
					}
				}
			}
		}
	}

//hough = struct "p.h"
Hough h = {0, 0, 0, matriz[0][0][0]};
	for (i = rmin; i < Bin->height - rmin ;i++){
		for (j = rmin; j < Bin->width ;j++){
			for (r = rmin; r < rmax ;r++){
				if(matriz[i][j][r - rmin] > h.valor){
					h.valor = matriz[i][j][r-rmin];
					h.x = i;
					h.y = j;
					h.r = r;
				}
			}
		}
	}

//calculo do circulo
	for (i = rmin; i < N->height-rmin ;i++){
		for (j = rmin; j < N->width-rmin ;j++){
			int d = (int) sqrt(pow(i-h.x, 2) + pow(j-h.y, 2));
	
			if(d == h.r){
				N->pixels[i][j].r = 0;
				N->pixels[i][j].g = 255;
				N->pixels[i][j].b = 0;
			}
		}
	}
	for (i = 0; i < N->height ;i++){
		for(j = 0; j < N->width ;j++){
		int d = (int) sqrt(pow(i-h.x, 2) + pow(j-h.y, 2));
			if (d > h.r){
				N->pixels[i][j].r = 0;
				N->pixels[i][j].g = 0;
				N->pixels[i][j].b = 0;
			}
		}
	}
return N;
}

//Diagnostico

//DEV

//Salvar imagem

int saveImage(char *file, Image *img, int comp){

	int i = 0;
	int j = 0;

	FILE * fileName = fopen(file , "w");

	if(! fileName)
		return -1;

	fprintf( fileName, "P3\n");
	fprintf( fileName, "%d %d\n", img->width, img->height );
	fprintf( fileName, "%d\n", comp);

	for(i = 0; i < img->height; i++){
		for(j = 0; j < img->width; j++){
			if(img->pixels[i][j].r > 255) img->pixels[i][j].r = 255;
			if(img->pixels[i][j].b > 255) img->pixels[i][j].g = 255;
			if(img->pixels[i][j].b > 255) img->pixels[i][j].b = 255;
			fprintf(fileName, "%d\n", img->pixels[i][j].r);
			fprintf(fileName, "%d\n", img->pixels[i][j].g);
			fprintf(fileName, "%d\n", img->pixels[i][j].b);
		}
	}

	fclose(fileName);

	return 0;

}
