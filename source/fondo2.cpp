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
Constructor de un fondo en una posicion y la camara (el tipo serviria para elegir entre
fondos)
*/
Fondo2::Fondo2(float x, float y, const char* ruta)
{
    // Ajustamos la posicion del SDL_Rect
    posicion.x = x;
    posicion.y = y;

    // Cargamos la texutra (combiar por un switch para elegir)
    CargarTextura(ruta);
}

/*
Metodo para dibujar en pantalla el fondo
*/
void Fondo2::Renderizar()
{
    // Dibujamos el fondo
    if (textura != NULL)
    {
        SDL_RenderCopy(renderer, textura, NULL, &posicion);
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
