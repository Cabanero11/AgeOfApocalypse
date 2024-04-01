#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern int numGroundChecks;

/*
Metodo ejecutado al ocurrir una colision
*/
void DetectorDeColisiones::BeginContact(b2Contact* contact)
{
	// Informacion de la colision
	FixtureData* data;
	
	// Si existe en el primer objeto
	if (contact->GetFixtureA()->GetUserData().pointer != 0)
	{
		// Lo interpretamos y realizamos las acciones necesarias
		data = reinterpret_cast<FixtureData*>(contact->GetFixtureA()->GetUserData().pointer);
		switch (data->tipo)
		{
			case TIPO_GROUND_CHECK:
				numGroundChecks++;
				break;
			case TIPO_MONEDA_YOSHI:
				data->monedaYoshi->Recoger();
				break;
			case TIPO_MONEDA:
				data->moneda->Recoger();
				break;
			case TIPO_HONGO:
				data->hongo->Recoger();
				break;
			case TIPO_HONGO_VIDA:
				data->hongoVida->Recoger();
				break;
			case TIPO_BLOQUE_INTERROGANTE:
				data->bloqueInterrogante->Golpear();
				break;
		}
	}
	
	// Si existe en el segundo objeto
	if (contact->GetFixtureB()->GetUserData().pointer != 0)
	{
		// Lo interpretamos y realizamos las acciones necesarias
		data = reinterpret_cast<FixtureData*>(contact->GetFixtureB()->GetUserData().pointer);
		switch (data->tipo)
		{
			case TIPO_GROUND_CHECK:
				numGroundChecks++;
				break;
			case TIPO_MONEDA_YOSHI:
				data->monedaYoshi->Recoger();
				break;
			case TIPO_MONEDA:
				data->moneda->Recoger();
				break;
			case TIPO_HONGO:
				data->hongo->Recoger();
				break;
			case TIPO_HONGO_VIDA:
				data->hongoVida->Recoger();
				break;
			case TIPO_BLOQUE_INTERROGANTE:
				data->bloqueInterrogante->Golpear();
				break;
		}
	}
}

/*
Metodo que se ejecuta al finalizar una colision
*/
void DetectorDeColisiones::EndContact(b2Contact* contact)
{
	// Informacion de la colision
	FixtureData* data;
	
	// Si existe en el primer objeto
	if (contact->GetFixtureA()->GetUserData().pointer != 0)
	{
		// Lo interpretamos y realizamos las acciones necesarias
		data = reinterpret_cast<FixtureData*>(contact->GetFixtureA()->GetUserData().pointer);
		switch (data->tipo)
		{
			case TIPO_GROUND_CHECK:
				numGroundChecks--;
				break;
		}
	}
	
	// Si existe en el segundo objeto
	if (contact->GetFixtureB()->GetUserData().pointer != 0)
	{
		// Lo interpretamos y realizamos las acciones necesarias
		data = reinterpret_cast<FixtureData*>(contact->GetFixtureB()->GetUserData().pointer);
		switch (data->tipo)
		{
			case TIPO_GROUND_CHECK:
				numGroundChecks--;
				break;
		}
	}
}
