#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "rutas.h"
#include "constantes.h"

#include <time.h>
//#include <chrono>
#include <unistd.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <switch.h>

#include <box2d/box2d.h>

using namespace std;

struct Jugador;
struct Jugador2;
struct Interfaz;
struct MonedaYoshi;
struct Moneda;
struct Hongo;
struct HongoVida;
struct BloqueInterrogante;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern Interfaz* interfaz;

struct FixtureData
{
	int tipo;	
	MonedaYoshi* monedaYoshi;
	Moneda* moneda;	
	Hongo* hongo;
	HongoVida* hongoVida;
	BloqueInterrogante* bloqueInterrogante;
};

class DetectorDeColisiones : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);	
};

struct Camara
{
	const float velocidad = 0.8f;
	int x;
	int y;
	float xReal;
	float yReal;
	Jugador* jugador;
	
	Camara(float x, float y);
	void AsignarJugador(Jugador* jugador);
	void Actualizar();
};

struct Camara2
{
	const float velocidad = 0.8f;
	int x;
	int y;
	float xReal;
	float yReal;
	Jugador2* jugador2;
	
	Camara2(float x, float y);
	void AsignarJugador2(Jugador2* jugador);
	void Actualizar2();
};

struct Animacion
{
	SDL_Texture* textura;
	SDL_Texture* frames[64];
	int nFrames;
	int nTicksPorFrame;
	int tickActual;
	
	Animacion();
	void CrearAnimacion(int nFrames, int nTicksPorFrame, const char* rutas[]);
	void Reiniciar();
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
};

struct Jugador
{	
	enum Estado
	{
		Agachado,
		CambioDir,
		Cayendo,
		Corriendo,
		Quieto,
		QuietoArriba,
		Saltando,
		SaltoCorriendo,
		Sprint,
		Muerte	
	};
	
	const float maxVelocidad = 2.0f;
	const float accel = 0.05f;
	float velocidad;
	int velocidad_movimiento;
	Camara* camara;
	b2Body* cuerpoFisico;
	SDL_Texture* agachado[4];
	SDL_Texture* cambioDir[4];
	SDL_Texture* cayendo[4];
	Animacion corriendo[4];
	SDL_Texture* quieto[4];
	SDL_Texture* quietoArriba[4];
	SDL_Texture* saltando[4];
	SDL_Texture* saltoCorriendo[4];
	Animacion sprint[4];
	Animacion growUp[4];
	Animacion muerte;
	SDL_Rect posicion;
	int tamanyo;
	int direccion;
	Estado estado;
	
	Jugador(Camara* camara, b2World* world);
	void Movimiento(int x);
	void Renderizar(int x, bool saltado);
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct Jugador2
{	
	enum Estado
	{
		Agachado,
		CambioDir,
		Cayendo,
		Corriendo,
		Quieto,
		QuietoArriba,
		Saltando,
		SaltoCorriendo,
		Sprint,
		Muerte	
	};
	
	const float maxVelocidad = 2.0f;
	const float accel = 0.05f;
	float velocidad;
	int velocidad_movimiento;
	Camara2* camara2;
	b2Body* cuerpoFisico;
	SDL_Texture* agachado[4];
	SDL_Texture* cambioDir[4];
	SDL_Texture* cayendo[4];
	Animacion corriendo[4];
	SDL_Texture* quieto[4];
	SDL_Texture* quietoArriba[4];
	SDL_Texture* saltando[4];
	SDL_Texture* saltoCorriendo[4];
	Animacion sprint[4];
	Animacion growUp[4];
	Animacion muerte;
	SDL_Rect posicion;
	int tamanyo;
	int direccion;
	Estado estado;
	
	Jugador2(Camara2* camara2, b2World* world);
	void Movimiento2(int x, int y);
	void Renderizar2(int x, int y);
	void MoverIzquierda();
	void MoverDerecha();
	void MoverArriba();
	void MoverAbajo();
private:
	SDL_Texture* CargarTextura2(const char* ruta);
	void IniciarCuerpoFisico2(b2World* world);
};

struct Suelo
{
	Camara* camara;
	SDL_Texture *textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	
	Suelo(int tipo, float x, float y, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct Tuberia
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	
	Tuberia(int tipo, float x, float y, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct BloqueGiratorio
{
	Animacion animacion;
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	
	BloqueGiratorio(float x, float y, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct BloqueInterrogante
{
	Animacion animacion;
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	bool golpeado;
	
	BloqueInterrogante(float x, float y, Camara* camara, b2World* world);
	void Golpear();
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct BloqueNube
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	
	BloqueNube(float x, float y, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct BloquePiedra
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	
	BloquePiedra(float x, float y, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct Decoracion
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	
	Decoracion(int tipo, float x, float y, Camara* camara);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);	
};

struct MonedaYoshi
{
	Animacion animacion;
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	bool recogida;
	
	MonedaYoshi(float x, float y, Camara* camara, b2World* world);
	void Recoger();
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct Moneda
{
	Animacion animacion;
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	bool recogida;
	
	Moneda(float x, float y, Camara* camara, b2World* world);
	void Recoger();
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);		
};

struct Hongo
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	bool recogido;
	
	Hongo(float x, float y, Camara* camara, b2World* world);
	void Recoger();
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct HongoVida
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	bool recogido;
	
	HongoVida(float x, float y, Camara* camara, b2World* world);
	void Recoger();
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct Plataforma
{
	Camara* camara;
	SDL_Texture *textura;
	SDL_Rect posicion;
	b2Body* cuerpoFisico;
	
	Plataforma(int tipo, float x, float y, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
	void IniciarCuerpoFisico(b2World* world);
};

struct Fondo
{
	Camara* camara;
	SDL_Texture* textura;
	SDL_Rect posicion;
	
	Fondo(int tipo, float x, float y, Camara* camara);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
};

// fondo2.cpp modificacion de fondo.cpp sin Camara creo

struct Fondo2
{
	Camara2* camara2;
	SDL_Texture* textura;
	SDL_Rect posicion;
	
	Fondo2(int tipo, float x, float y, Camara2* camara2);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
};


struct Mapa
{
	int fondoR;
	int fondoG;
	int fondoB;
	int ancho;
	int alto;
	vector<Suelo*> suelos;
	vector<Tuberia*> tuberias;
	vector<BloqueGiratorio*> bGiratorios;
	vector<BloqueInterrogante*> bInterrogantes;
	vector<BloqueNube*> bNubes;
	vector<BloquePiedra*> bPiedras;
	vector<Decoracion*> decoraciones;
	vector<MonedaYoshi*> monedasYoshi;
	vector<Plataforma*> plataformas;
	vector<Fondo*> fondos;
	vector<Moneda*> monedas;
	vector<Hongo*> hongos;
	vector<HongoVida*> hongosVida;
	
	Mapa(const char* ruta, Camara* camara, b2World* world);
	void Renderizar();
	void Destruir();
};

struct Mapa2
{
	int fondoR;
	int fondoG;
	int fondoB;
	int ancho;
	int alto;

	vector<Fondo2*> fondos;

	
	Mapa2(const char* ruta, Camara2* camara2, b2World* world);
	void Renderizar();
	void Destruir();
};

struct ObjetoInterfazEstatico
{
	SDL_Texture* textura;
	SDL_Rect posicion;
	
	ObjetoInterfazEstatico(float x, float y, const char* ruta);
	void Renderizar();
	void Destruir();
private:
	void CargarTextura(const char* ruta);
};

struct NumeroVidas
{
	SDL_Texture* numeros[10];
	SDL_Texture* vidas[2];
	SDL_Rect posicion[2];
	int numVidas;
	
	NumeroVidas(float x, float y, int numVidasInicial);
	void CambiarVida(int incremento);
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void CargarTexturas();
};

struct Contenedor
{
	SDL_Texture* contenedores[2];
	SDL_Texture* contenedor;
	SDL_Rect posicion;
	bool conSeta;
	
	Contenedor(float x, float y, bool conSeta);
	void CambiarSeta();
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void CargarTexturas();
};

struct TiempoInterfaz
{
	SDL_Texture* numeros[10];
	SDL_Texture* tiempo[3];
	SDL_Rect posicion[3];
	int tiempoAct;
	float tiempoReal;
	
	TiempoInterfaz(float x, float y, int tiempoInicial);
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void CargarTexturas();
};

struct MonedasInterfaz
{
	SDL_Texture* numeros[10];
	SDL_Texture* monedas[2];
	SDL_Rect posicion[2];
	int monedasAct;
	
	MonedasInterfaz(float x, float y, int monedasInicial);
	void CambiarMonedas(int incremento);
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void CargarTexturas();
};

struct EstrellasInterfaz
{
	SDL_Texture* numeros[10];
	SDL_Texture* estrellas[2];
	SDL_Rect posicion[2];
	int estrellasAct;
	
	EstrellasInterfaz(float x, float y, int estrellasInicial);
	void CambiarEstrellas(int incremento);
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void CargarTexturas();
};

struct PuntuacionInterfaz
{
	SDL_Texture* numeros[10];
	SDL_Texture* puntuacion[7];
	SDL_Rect posicion[7];
	int puntuacionAct;
	
	PuntuacionInterfaz(float x, float y, int puntuacionInicial);
	void CambiarPuntuacion(int incremento);
	void Renderizar();
	void Destruir();
private:
	SDL_Texture* CargarTextura(const char* ruta);
	void CargarTexturas();
};

struct Interfaz
{
	vector<ObjetoInterfazEstatico*> objetosEstaticos;
	NumeroVidas* vidas;
	EstrellasInterfaz* estrellas;
	Contenedor* contenedor;
	TiempoInterfaz* tiempo;
	MonedasInterfaz* monedas;
	PuntuacionInterfaz* puntuacion;
	
	Interfaz();
	void CambiarPuntuacion(int numPuntos);
	void CambiarNumeroMonedas(int numMonedas);
	void CambiarNumeroVidas(int numVidas);
	void CambiarNumeroEstrellas(int numEstrellas);
	void CambiarContenedor();
	void Renderizar();
	void Destruir();
};

#endif
