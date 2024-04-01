#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar una textura y devolverla como resultado
*/
SDL_Texture* Animacion::CargarTextura(const char* ruta)
{
	// Cargamos la textura y declaramos la textura
	SDL_Surface* img = IMG_Load(ruta);
	SDL_Texture* tex = NULL;
	
	if (img)
	{
		// Creamos la textura y liberamos la imagen
		tex = SDL_CreateTextureFromSurface(renderer, img);
		SDL_FreeSurface(img);
	}
	
	// Devolvemos la textura
	return tex;
}

/*
Constructor de una animacion, en este caso se trata de un constructor que inicializa las variables
como si no existiera una animacion
*/
Animacion::Animacion()
{
	nFrames = 0; nTicksPorFrame = 0;
	for (int i = 0; i < 64; i++)
		frames[i] = NULL;
	textura = NULL;
	tickActual = 0;
}

/*
Metodo para crear una animacion a partir del numero de frames, del numero de ticks que dura cada frame 
(velocidad) y con las rutas donde se encuentran las imagenes de los frames
*/
void Animacion::CrearAnimacion(int nFrames, int nTicksPorFrame, const char* rutas[])
{
	// Almacenamos lo necesario
	this->nFrames = nFrames;
	this->nTicksPorFrame = nTicksPorFrame;

	// Cargamos los frames	
	for (int i = 0; i < nFrames; i++)
		frames[i] = CargarTextura(rutas[i]);
	
	// Colocamos la animación en el primer frame
	textura = frames[0];
	tickActual = 0;
}

/*
Metodo para colocar la animacion en el primer frame
*/
void Animacion::Reiniciar()
{
	textura = frames[0];
	tickActual = 0;
}

/*
Metodo para avanzar la animacion en un tick
*/
void Animacion::Renderizar()
{
	// Avanzamos el tick
	tickActual++;
	
	// En caso de que hayamos acabado la animacion volvemos al comienzo
	if (tickActual >= nFrames * nTicksPorFrame + nTicksPorFrame)
	{
		tickActual = -1;
		textura = frames[0];
		return;
	}
	
	// Si no, buscamos cual es el frame actual
	for (int i = 0; i < nFrames; i++)
	{
		if (tickActual < i * nTicksPorFrame + nTicksPorFrame)
		{
			textura = frames[i];
			break;
		}
	}
	
}

/*
Metodo para liberar la animacion de memoria
*/
void Animacion::Destruir()
{
	// Destruimos las texturas de los frames
	for (int i = 0; i < nFrames; i++)
		SDL_DestroyTexture(frames[i]);
}

