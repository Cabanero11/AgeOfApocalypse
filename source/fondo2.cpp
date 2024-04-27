#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar la imagen que se encuentra en ruta en la variable textura.
*/
void Fondo2::CargarTextura(const char* ruta)
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
Constructor de un fondo en una posicion y la camara2 (el tipo serviria para elegir entre
fondos)
*/
Fondo2::Fondo2(int tipo, float x, float y, Camara2* camara2)
{
	// Ajustamos la posicion del SDL_Rect
	posicion.x = x;
	posicion.y = y;
	
	// Guardamos la camara
	this->camara2 = camara2;
	
	// Cargamos la texutra (combiar por un switch para elegir)
	CargarTextura(RUTA_FONDO);
}

/*
Metodo para dibujar en pantalla el fondo
*/
void Fondo2::Renderizar()
{
	// Calculamos la posicion donde se debe dibujar teniendo en cuenta la camara, aplicado
	// un factor de 0.5 en x para crear un efecto de paralaje (funciona como un offset)
	int posicionDibujoX = posicion.x - camara2->x * 0.5f;
	int posicionDibujoY = posicion.y - camara2->y;
	
	// Dibujamos el fondo
	if (true)
	{
		SDL_Rect posicionDibujo;
		posicionDibujo = {posicionDibujoX, posicionDibujoY, posicion.w, posicion.h};
		SDL_RenderCopy(renderer, textura, NULL, &posicionDibujo);
	}
}

/*
Metodo para liberar el fondo de la memoria
*/
void Fondo2::Destruir()
{
	// Destruimos la textura
	SDL_DestroyTexture(textura);
}
