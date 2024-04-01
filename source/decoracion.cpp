#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar la imagen que se encuentra en ruta en la variable textura.
*/
void Decoracion::CargarTextura(const char* ruta)
{
	// Cargamos la imagen
	SDL_Surface* img = IMG_Load(ruta);
	
	if (img)
	{
		// Ajustamos el tamanyo del SDL_Rect al de la imagen
		posicion.w = img->w;
		posicion.h = img->h;
		
		// Creamos la textura a partir de la imagen
		textura = SDL_CreateTextureFromSurface(renderer, img);
		
		// Liberamos la imagen
		SDL_FreeSurface(img);
	}
}

/*
Constructor de un elemento de decoracion en una posicion dada. Necesita la camara y el mundo fisico actual
El tipo se utiliza para indicar que decoracion usar
*/
Decoracion::Decoracion(int tipo, float x, float y, Camara* camara)
{
	// Ajustamos la posicion del SDL_Rect
	posicion.x = x;
	posicion.y = y;
	
	// Guardamos la camara
	this->camara = camara;
	
	// Comprobamos que decoracion usar
	switch (tipo)
	{
		case 1:
			CargarTextura(RUTA_CESPED_1);
			break;
		case 2:
			CargarTextura(RUTA_CESPED_2);
			break;
		case 3:
			CargarTextura(RUTA_CESPED_3);
			break;
		case 4:
			CargarTextura(RUTA_CESPED_4);
			break;
		case 5:
			CargarTextura(RUTA_CESPED_5);
			break;
		case 6:
			CargarTextura(RUTA_CESPED_6);
			break;
		case 7:
			CargarTextura(RUTA_CESPED_7);
			break;
		case 8:
			CargarTextura(RUTA_CARTEL);
			break;
		case 9:
			CargarTextura(RUTA_FINAL_NIVEL);
			break;
	}
}

/*
Metodo para dibujar en pantalla la decoracion
*/
void Decoracion::Renderizar()
{
	// Calculamos la posicion donde se debe dibujar teniendo en cuenta la camara (funciona como un offset)
	int posicionDibujoX = posicion.x - camara->x;
	int posicionDibujoY = posicion.y - camara->y;
	
	// Si tenemos la textura y se encuentra dentro de los limites de la pantalla, lo dibujamos
	if (textura && 
		posicionDibujoX + posicion.w > -50 && posicionDibujoX < SCREEN_W + 50 && 
		posicionDibujoY > -50 && posicionDibujoY + posicion.h < SCREEN_H + 50)
	{
		SDL_Rect posicionDibujo;
		posicionDibujo = {posicionDibujoX, posicionDibujoY, posicion.w, posicion.h};
		SDL_RenderCopy(renderer, textura, NULL, &posicionDibujo);
	}
}

/*
Metodo para liberar el bloque de la memoria
*/
void Decoracion::Destruir()
{
	// Destruimos la textura
	SDL_DestroyTexture(textura);
}
