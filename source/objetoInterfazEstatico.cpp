#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar la imagen que se encuentra en ruta en la variable textura.
*/
void ObjetoInterfazEstatico::CargarTextura(const char* ruta)
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
Constructor de un objeto de interfaz a partir de la posicion y la ruta de la imagen
*/
ObjetoInterfazEstatico::ObjetoInterfazEstatico(float x, float y, const char* ruta)
{	
	// Ajustamos la posicion del SDL_Rect
	posicion.x = x;
	posicion.y = y;
	
	// Cargamas la textura
	CargarTextura(ruta);
}

/*
Metodo para dibujar en pantalla el bloque
*/
void ObjetoInterfazEstatico::Renderizar()
{
	// Si existe la textura, lo dibujamos
	if (textura)
		SDL_RenderCopy(renderer, textura, NULL, &posicion);
}

/*
Metodo para liberar el objeto de la memoria
*/
void ObjetoInterfazEstatico::Destruir()
{
	// Destruimos la textura
	SDL_DestroyTexture(textura);
}
