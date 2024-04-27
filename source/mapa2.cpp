#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Constructor de un mapa. Necesita la ruta del fichero que contiene la informacion
del mismo, la camara2 y el mundo fisico
*/
Mapa2::Mapa2(const char* ruta, Camara2* camara2, b2World* world)
{
	// Leemos el fichero
	std::ifstream mapa;
	mapa.open(ruta, std::ios::in);
	
	// Vamos cargando todo la informacion del fichero
	mapa >> ancho >> alto;
	int pixeles = 16, act = 0, nCapas = 0, fondoAct, posFondo = 0;
	
	// Capa de suelos
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			mapa >> act;
		}
	}
	
	// Capa de tuberias
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			mapa >> act;
		}
	}
	
	// Capa de bloques
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			mapa >> act;
			if (act != 0)
			{
				switch (act)
				{
					
				}
			}
		}
	}
	
	// Capa de fondo
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			mapa >> act;
				
		}
	}
	
	// Capa de items
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			mapa >> act;
		}
	}
	
	// Capas de plataformas
	mapa >> nCapas;
	for (int k = 0; k < nCapas; k++)
	{
		for (int i = 0; i < alto; i++)
		{
			for (int j = 0; j < ancho; j++)
			{
				mapa >> act;
			}
		}
	}
	
	// Fondos
	mapa >> fondoR >> fondoG >> fondoB;
	mapa >> fondoAct;
	while (posFondo < ancho * pixeles)
	{
		fondos.push_back(new Fondo2(fondoAct, posFondo, 0.0f, camara2));
		posFondo += 512;
	}
	
	// Cerramos el fichero
	mapa.close();
}

/*
Metodo para renderizar el mapa
*/
void Mapa2::Renderizar()
{
	// Renderizamos todos los vectores de elementos
	for (std::size_t i = 0; i < fondos.size(); i++)
		fondos[i]->Renderizar();
	
}

/*
Metodo para liberar de memoria un mapa
*/
void Mapa2::Destruir()
{
	// Destruimos todos los vectores de elementos
	for (std::size_t i = 0; i < fondos.size(); i++)
		fondos[i]->Destruir();
	
}

