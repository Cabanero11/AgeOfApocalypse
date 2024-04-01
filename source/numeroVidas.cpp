#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar una textura y devolverla como resultado
*/
SDL_Texture* NumeroVidas::CargarTextura(const char* ruta)
{
	// Cargamos la textura y declaramos la textura
	SDL_Surface* img = IMG_Load(ruta);
	SDL_Texture* tex = NULL;
	
	if (img)
	{
		// Creamos la textura, ajustamos las posiciones y liberamos la imagen
		tex = SDL_CreateTextureFromSurface(renderer, img);
		for (int i = 0; i < 2; i++)
		{
			posicion[i].w = img->w;
			posicion[i].h = img->h;
		}
		SDL_FreeSurface(img);
	}
	return tex;
}

/*
Metodo para cargar las texturas de los numeros
*/
void NumeroVidas::CargarTexturas()
{
	numeros[0] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_0);
	numeros[1] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_1);
	numeros[2] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_2);
	numeros[3] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_3);
	numeros[4] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_4);
	numeros[5] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_5);
	numeros[6] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_6);
	numeros[7] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_7);
	numeros[8] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_8);
	numeros[9] = CargarTextura(RUTA_NUMERO_BLANCO_PEQUENYO_9);

}

/*
Constructor del numero de vidas de la interfaz a partir de la posicion del primer 
numero y el numero de vidas inicial
*/
NumeroVidas::NumeroVidas(float x, float y, int numVidasInicial)
{
	// Actualizamos las monedas actuales
	numVidas = numVidasInicial;
	
	// Cargamos las texturas de los numeros
	CargarTexturas();
	
	// Ajustamos las posiciones de los numeros
	for (int i = 0; i < 2; i++)
	{
		posicion[i].x = x + posicion[i].w * i;
		posicion[i].y = y;
	}
	
	// Actualizamos los sprites
	switch(numVidas / 10)
	{
		case 0:
			vidas[0] = numeros[0];
			break;
		case 1:
			vidas[0] = numeros[1];
			break;
		case 2:
			vidas[0] = numeros[2];
			break;
		case 3:
			vidas[0] = numeros[3];
			break;
		case 4:
			vidas[0] = numeros[4];
			break;
		case 5:
			vidas[0] = numeros[5];
			break;
		case 6:
			vidas[0] = numeros[6];
			break;
		case 7:
			vidas[0] = numeros[7];
			break;
		case 8:
			vidas[0] = numeros[8];
			break;
		case 9:
			vidas[0] = numeros[9];
			break;
	}
	
	switch(numVidas % 10)
	{
		case 0:
			vidas[1] = numeros[0];
			break;
		case 1:
			vidas[1] = numeros[1];
			break;
		case 2:
			vidas[1] = numeros[2];
			break;
		case 3:
			vidas[1] = numeros[3];
			break;
		case 4:
			vidas[1] = numeros[4];
			break;
		case 5:
			vidas[1] = numeros[5];
			break;
		case 6:
			vidas[1] = numeros[6];
			break;
		case 7:
			vidas[1] = numeros[7];
			break;
		case 8:
			vidas[1] = numeros[8];
			break;
		case 9:
			vidas[1] = numeros[9];
			break;
	}
}

/*
Metodo para anyadir vidas y actualizar la interfaz
*/
void NumeroVidas::CambiarVida(int incremento)
{
	// Actualizamos las vidas actuales
	numVidas = numVidas + incremento;
	
	// Actualizamos los sprites
	switch(numVidas / 10)
	{
		case 0:
			vidas[0] = numeros[0];
			break;
		case 1:
			vidas[0] = numeros[1];
			break;
		case 2:
			vidas[0] = numeros[2];
			break;
		case 3:
			vidas[0] = numeros[3];
			break;
		case 4:
			vidas[0] = numeros[4];
			break;
		case 5:
			vidas[0] = numeros[5];
			break;
		case 6:
			vidas[0] = numeros[6];
			break;
		case 7:
			vidas[0] = numeros[7];
			break;
		case 8:
			vidas[0] = numeros[8];
			break;
		case 9:
			vidas[0] = numeros[9];
			break;
	}
	
	switch(numVidas % 10)
	{
		case 0:
			vidas[1] = numeros[0];
			break;
		case 1:
			vidas[1] = numeros[1];
			break;
		case 2:
			vidas[1] = numeros[2];
			break;
		case 3:
			vidas[1] = numeros[3];
			break;
		case 4:
			vidas[1] = numeros[4];
			break;
		case 5:
			vidas[1] = numeros[5];
			break;
		case 6:
			vidas[1] = numeros[6];
			break;
		case 7:
			vidas[1] = numeros[7];
			break;
		case 8:
			vidas[1] = numeros[8];
			break;
		case 9:
			vidas[1] = numeros[9];
			break;
	}
}

/*
Metodo para dibujar en pantalla el contador
*/
void NumeroVidas::Renderizar()
{
	// Si existen las texturas, los dibujamos en su posicion
	if (vidas[0])
		SDL_RenderCopy(renderer, vidas[0], NULL, &posicion[0]);
	if (vidas[1])
		SDL_RenderCopy(renderer, vidas[1], NULL, &posicion[1]);
}

/*
Metodo para liberar el contador de la memoria
*/
void NumeroVidas::Destruir()
{	
	// Destruimos todas las texturas
	for (int i = 0; i < 10; i++)
	{
		SDL_DestroyTexture(numeros[i]);
	}
}
