#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar una textura y devolverla como resultado
*/
SDL_Texture* Contenedor::CargarTextura(const char* ruta)
{
	// Cargamos la textura y declaramos la textura
	SDL_Surface* img = IMG_Load(ruta);
	SDL_Texture* tex = NULL;
	
	if (img)
	{
		// Creamos la textura, ajustamos la posicion y liberamos la imagen
		tex = SDL_CreateTextureFromSurface(renderer, img);
		posicion.w = img->w;
		posicion.h = img->h;
		SDL_FreeSurface(img);
	}
	
	
	// Devolvemos la textura
	return tex;
}

/*
Metodo para cargar las texturas del contendor
*/
void Contenedor::CargarTexturas()
{
	contenedores[0] = CargarTextura(RUTA_CONTENEDOR_VACIO);
	contenedores[1] = CargarTextura(RUTA_CONTENEDOR_SETA);
}

/*
Constructor del contenedor a partir de la posicion y si tiene seta o no
*/
Contenedor::Contenedor(float x, float y, bool conSeta)
{
	// Ajustamos la posicion del SDL_Rect
	posicion.x = x;
	posicion.y = y;
	
	// Cargamos las texturas
	CargarTexturas();
	
	// Guardamos si tiene seta y ajustamos el sprite
	this->conSeta = conSeta;
	if (conSeta)
		contenedor = contenedores[1];
	else
		contenedor = contenedores[0];
}

/*
Metodo para cambiar entre tener seta y no
*/
void Contenedor::CambiarSeta()
{
	if (conSeta)
	{
		conSeta = false;
		contenedor = contenedores[0];
	}
	else
	{
		conSeta = true;
		contenedor = contenedores[1];
	}
}

/*
Metodo para dibujar en pantalla el contenedor
*/
void Contenedor::Renderizar()
{
	if (contenedor)
		SDL_RenderCopy(renderer, contenedor, NULL, &posicion);
}

/*
Metodo para liberar el contenedor de la memoria
*/
void Contenedor::Destruir()
{
	// Destruimos ambas texturas
	SDL_DestroyTexture(contenedores[0]);
	SDL_DestroyTexture(contenedores[1]);
}
