#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern int numGroundChecks;

/*
Constructor de la camara. Se le pasa la posicion inicial
*/
Camara::Camara(float x, float y)
{
	this->x = x;
	this->y = y;
}

/*
Metodo para tener una referencia al jugador
*/
void Camara::AsignarJugador(Jugador* jugador)
{
	this->jugador = jugador;
}

/*
Metodo para actualiza la posicion de la camara
*/
void Camara::Actualizar()
{
	/*
	float diff = jugador->posicion.x + jugador->posicion.w / 2 - x + SCREEN_W / 2;
	if (diff < 50.0f)
	{
		xReal -= velocidad * diff * 0.014;
		if (xReal + SCREEN_W / 2 < jugador->posicion.x + jugador->posicion.w / 2)
			xReal = jugador->posicion.x - SCREEN_W / 2 + jugador->posicion.w / 2;
	}
	else if (diff > -50.0f)
	{
		xReal += velocidad * diff * 0.014;
		if (xReal + SCREEN_W / 2 > jugador->posicion.x + jugador->posicion.w / 2)
			xReal = jugador->posicion.x- SCREEN_W / 2 + jugador->posicion.w / 2;
	}
	if (xReal < 0) xReal = 0;
	if (numGroundChecks > 0)
	{
		yReal = jugador->posicion.y - SCREEN_H / 2 - jugador->posicion.h / 2;
	}
	if (yReal > 512 - SCREEN_H) yReal = 512 - SCREEN_H;
	*/
	x = jugador->posicion.x - SCREEN_W / 2.0f + jugador->posicion.w / 2.0f;
	y = jugador->posicion.y - SCREEN_H / 2.0f - jugador->posicion.h / 2.0f;
	if (x < 0) x = 0;
	if (y > 512 - SCREEN_H) y = 512 - SCREEN_H;
}
