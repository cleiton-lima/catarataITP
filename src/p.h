typedef struct Pixels{
	int r, g, b;
} Pixel;

typedef struct Images{
	int width, height;
	int maxLength;
	Pixel **pixels;
} Image;

enum bool{
	true = 1,
	false = 0
} bool;

typedef struct hough { 
	int x, y, r, valor;
} Hough;
