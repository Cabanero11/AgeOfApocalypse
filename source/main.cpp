#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <switch.h>
#include <cmath>
#include <unistd.h>

// Incluye los encabezados más comunes de la biblioteca estándar de C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

// EN TEORIA AÑADIR COSAS AQUI SI NUEVAS CLASES
#include "gameEngine.h"

using namespace std;


// LAS CONSTANTES SE DEFINEN EN constantes.h

// Sistema de experiencia

int nivelExperiencia = 0;
int contadorDeExperiencia = 0;
double porcentajeDeNivel = 0;

bool partidaAcabada = false;

double danoProyectil = 10;
int COOLDOWNProyectil_MAX = 60; // Tiempo de cooldownProyectil en frames (ajusta según necesites)

// Variables de movimiento
bool move_up = false;
bool move_down = false;
bool move_left = false;
bool move_right = false;

SDL_Rect jugadorPosicion;

float velocidadMovimiento = 5.0f;

// Para el tiempo
#define CPU_TICKS_PER_SECOND 19200000

// En lugar de definir los minutos como 60, los definiremos como 10 para 10 minutos.
int minutes = 5;
int seconds = 0;

// Cambiar la duración total a 10 minutos en segundos
#define DURATION_SECONDS (10 * 60) // 10 minutos en segundos




SDL_Rect tiempo_rect = { SCREEN_W / 2, 36, 0, 0 }; // Posición del texto del tiempo


SDL_Color colors[] = {
    { 255, 0, 0, 0 }, 
    { 0, 0, 255, 0 },
};

struct Coordenada {
    double x;
    double y;
};

double distancia(const Coordenada& p1, const Coordenada& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}


// Estructura para representar un enemigo
struct Enemigo {
    SDL_Texture* texture;   // Textura del enemigo
    SDL_Rect pos;           // Posición del enemigo
    Coordenada coord;       // Coordenadas del enemigo
    double life;            // Puntos de vida del enemigo
    bool isAlive;           // Indica si el enemigo está con vida
    int tiempoUltimoImpacto;
};

// VIDA ENEMIGOS
double vidaGoblin = 20;
double vidaDemon = 30;

// Función para inicializar un enemigo
void InicializarEnemigo(Enemigo& enemigo, SDL_Renderer* renderer, const char* filename, int x, int y, double vida) {
    enemigo.texture = IMG_LoadTexture(renderer, filename);
    if (!enemigo.texture) {
        printf("Error cargando la textura del enemigo: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // Posicionar el enemigo a la derecha del jugador
    // 32 es el tamaño del pixel art del enemigo
    enemigo.pos = { x + 15, y, 32, 32 };

    enemigo.coord = { (double)x, (double)y };

    
    enemigo.life = vida;

    enemigo.isAlive = true;

    enemigo.tiempoUltimoImpacto = 302;
}

//#####################################################################################################
// DEFINIR OLEADAS OLEADAS OLEADAS OLEADAS
//#####################################################################################################

Enemigo oleadas[20][8]; // 18 oleadas, una cada 15 segundos a partir del 4:30. Cada oleada tendrá 8 enemigos
int contadorOleada = 0;
int oleadasVivas[20];
int cantidadOleadasVivas = 0;
bool generandoOleada = false;
bool oleada1generada = false;
bool oleada2generada = false;
bool oleada3generada = false;
bool oleada4generada = false;
bool oleada5generada = false;
bool oleada6generada = false;
bool oleada7generada = false;
bool oleada8generada = false;
bool oleada9generada = false;
bool oleada10generada = false;
bool oleada11generada = false;
bool oleada12generada = false;
bool oleada13generada = false;
bool oleada14generada = false;
bool oleada15generada = false;
bool oleada16generada = false;
bool oleada17generada = false;
bool oleada18generada = false;
bool oleada19generada = false;
bool oleada20generada = false;

Enemigo enemigosGoblins[8];
Enemigo enemigosBigDemon[16];

#define NUM_ENEMIGOS_DEMON 16
#define NUM_OLEADAS_DEMON 2

void InicializarOleada(int numeroOleada){
    if (!generandoOleada) 
    {
        generandoOleada = true;

        if (numeroOleada == 1 && !oleada1generada) {
            oleada1generada = true;
            InicializarEnemigo(oleadas[1][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[1][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[1][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[1][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[1][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[1][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[1][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[1][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 1;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 2 && !oleada2generada) {
            oleada2generada = true;
            InicializarEnemigo(oleadas[2][0], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaDemon);
            InicializarEnemigo(oleadas[2][1], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaDemon);
            InicializarEnemigo(oleadas[2][2], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaDemon);
            InicializarEnemigo(oleadas[2][3], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaDemon);
            InicializarEnemigo(oleadas[2][4], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaDemon);
            InicializarEnemigo(oleadas[2][5], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaDemon);
            InicializarEnemigo(oleadas[2][6], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaDemon);
            InicializarEnemigo(oleadas[2][7], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaDemon);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 2;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 3 && !oleada3generada) {
            oleada3generada = true;
            InicializarEnemigo(oleadas[3][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[3][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[3][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[3][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[3][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[3][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[3][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[3][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 3;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 4 && !oleada4generada) {
            oleada4generada = true;
            InicializarEnemigo(oleadas[4][0], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaDemon);
            InicializarEnemigo(oleadas[4][1], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaDemon);
            InicializarEnemigo(oleadas[4][2], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaDemon);
            InicializarEnemigo(oleadas[4][3], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaDemon);
            InicializarEnemigo(oleadas[4][4], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaDemon);
            InicializarEnemigo(oleadas[4][5], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaDemon);
            InicializarEnemigo(oleadas[4][6], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaDemon);
            InicializarEnemigo(oleadas[4][7], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaDemon);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 4;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 5 && !oleada5generada) {
            oleada5generada = true;
            InicializarEnemigo(oleadas[5][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[5][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[5][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[5][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[5][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[5][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[5][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[5][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 5;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 6 && !oleada6generada) {
            oleada6generada = true;
            InicializarEnemigo(oleadas[6][0], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaDemon);
            InicializarEnemigo(oleadas[6][1], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaDemon);
            InicializarEnemigo(oleadas[6][2], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaDemon);
            InicializarEnemigo(oleadas[6][3], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaDemon);
            InicializarEnemigo(oleadas[6][4], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaDemon);
            InicializarEnemigo(oleadas[6][5], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaDemon);
            InicializarEnemigo(oleadas[6][6], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaDemon);
            InicializarEnemigo(oleadas[6][7], renderer, "data/big_demon/big_demon_idle_anim_f3.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaDemon);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 6;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 7 && !oleada7generada) {
            oleada7generada = true;
            InicializarEnemigo(oleadas[7][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[7][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[7][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[7][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[7][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[7][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[7][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[7][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 7;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 8 && !oleada8generada) {
            oleada8generada = true;
            InicializarEnemigo(oleadas[8][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[8][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[8][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[8][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[8][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[8][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[8][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[8][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 8;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 9 && !oleada9generada) {
            oleada9generada = true;
            InicializarEnemigo(oleadas[9][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[9][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[9][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[9][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[9][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[9][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[9][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[9][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 9;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 10 && !oleada10generada) {
            oleada10generada = true;
            InicializarEnemigo(oleadas[10][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[10][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[10][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[10][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[10][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[10][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[10][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[10][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 10;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 11 && !oleada11generada) {
            oleada11generada = true;
            InicializarEnemigo(oleadas[11][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[11][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[11][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[11][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[11][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[11][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[11][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[11][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 11;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 12 && !oleada12generada) {
            oleada12generada = true;
            InicializarEnemigo(oleadas[12][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[12][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[12][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[12][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[12][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[12][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[12][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[12][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 12;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 13 && !oleada13generada) {
            oleada13generada = true;
            InicializarEnemigo(oleadas[13][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[13][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[13][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[13][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[13][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[13][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[13][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[13][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 13;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 14 && !oleada14generada) {
            oleada14generada = true;
            InicializarEnemigo(oleadas[14][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[14][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[14][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[14][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[14][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[14][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[14][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[14][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 14;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 15 && !oleada15generada) {
            oleada15generada = true;
            InicializarEnemigo(oleadas[15][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[15][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[15][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[15][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[15][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[15][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[15][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[15][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 15;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 16 && !oleada16generada) {
            oleada16generada = true;
            InicializarEnemigo(oleadas[16][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[16][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[16][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[16][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[16][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[16][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[16][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[16][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 16;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 17 && !oleada17generada) {
            oleada17generada = true;
            InicializarEnemigo(oleadas[17][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[17][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[17][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[17][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[17][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[17][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[17][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[17][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 17;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 18 && !oleada18generada) {
            oleada18generada = true;
            InicializarEnemigo(oleadas[18][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[18][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[18][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[18][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[18][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[18][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[18][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[18][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 18;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 19 && !oleada19generada) {
            oleada19generada = true;
            InicializarEnemigo(oleadas[19][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[19][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[19][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[19][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[19][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[19][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[19][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[19][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 19;
            cantidadOleadasVivas++;
        } else if (numeroOleada == 20 && !oleada18generada) {
            oleada20generada = true;
            InicializarEnemigo(oleadas[20][0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 0 , vidaGoblin);
            InicializarEnemigo(oleadas[20][1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 850, jugadorPosicion.y + 500, vidaGoblin);
            InicializarEnemigo(oleadas[20][2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y + 620, vidaGoblin);
            InicializarEnemigo(oleadas[20][3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 850, jugadorPosicion.y - 550, vidaGoblin);
            InicializarEnemigo(oleadas[20][4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 840, vidaGoblin);
            InicializarEnemigo(oleadas[20][5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 500, jugadorPosicion.y - 280, vidaGoblin);
            InicializarEnemigo(oleadas[20][6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 210, vidaGoblin);
            InicializarEnemigo(oleadas[20][7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 500, jugadorPosicion.y - 300, vidaGoblin);
            contadorOleada++;
            oleadasVivas[cantidadOleadasVivas] = 20;
            cantidadOleadasVivas++;
        }

        generandoOleada = false;
    }
}

struct OrbeExperiencia {
    SDL_Texture* texture;   // Textura del proyectil
    SDL_Rect pos;           // Posición del proyectil
    bool isAlive;
};

OrbeExperiencia orbesExperiencia[20][8];
int oleadasOrbesVivos[20];
int cantidadDeOrbesVivos = 0;

// Función auxiliar para intercambiar dos elementos en un array
void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Función para dividir el array y devolver el índice del pivote
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Seleccionamos el último elemento como pivote
    int i = low - 1; // Índice del elemento más pequeño

    for (int j = low; j < high; j++) {
        // Si el elemento actual es menor o igual al pivote
        if (arr[j] <= pivot) {
            i++; // Incrementa el índice del elemento más pequeño
            swap(arr, i, j); // Intercambia arr[i] y arr[j]
        }
    }
    swap(arr, i + 1, high); // Intercambia el pivote con el elemento en arr[i+1]
    return i + 1; // Retorna el índice del pivote
}

// Función principal de Quicksort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Encuentra el índice del pivote
        int pi = partition(arr, low, high);

        // Ordena los elementos antes y después del pivote
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


// Función para dibujar un enemigo en elrenderer
void DibujarEnemigo(SDL_Renderer* renderer, const Enemigo& enemigo) {
    SDL_RenderCopy(renderer, enemigo.texture, NULL, &enemigo.pos);
}


void MoverEnemigoHaciaElJugador(Enemigo& enemigo, const SDL_Rect* jugador, double distancia_maxima) {
    // Calculamos el vector dirección
    double direccion_x = jugador->x - enemigo.pos.x;
    double direccion_y = jugador->y - enemigo.pos.y;

    // Normalizamos el vector dirección
    double longitud = sqrt(pow(direccion_x, 2) + pow(direccion_y, 2));
    direccion_x /= longitud;
    direccion_y /= longitud;

    // Si el enemigo está lo suficientemente cerca del jugador, no se mueve
    if (longitud < distancia_maxima) {
        return;
    }

    // Escalamos el vector dirección por la cantidad de unidades
    double cantidad_unidades = 1.0;
    enemigo.coord.x += direccion_x * cantidad_unidades;
    enemigo.coord.y += direccion_y * cantidad_unidades;

    // Actualizamos la posición del enemigo
    enemigo.pos.x = (int)enemigo.coord.x;
    enemigo.pos.y = (int)enemigo.coord.y;
}

// Función para detectar colisión entre el jugador y un enemigo
bool detectarColisionJugadorEnemigo(SDL_Rect* jugador, Enemigo& enemigo) {
    // Verificar si las coordenadas del jugador y del enemigo se superponen
    if (jugador->x < enemigo.pos.x + enemigo.pos.w &&
        jugador->x + jugador->w > enemigo.pos.x &&
        jugador->y < enemigo.pos.y + enemigo.pos.h &&
        jugador->y + jugador->h > enemigo.pos.y) {
        // Colisión detectada, aquí puedes agregar la lógica correspondiente
        // por ejemplo, decrementar la salud del jugador
        printf("¡Colisión detectada entre el jugador y un enemigo!\n");
        return true;
    }

    return false;
}


// NO PASAR &jugadorPosicion sino no persigue al jugador
void OleadasGoblins(const SDL_Rect* jugadorPosicion) {
    for(int i = 0; i < 1; i++) {
        //InicializarEnemigo(enemigosGoblins[0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion->x + 650, jugadorPosicion->y + 0);
    }
}


void LiberarMemoriaGoblins() {
    for(int i = 0; i < 8; i++) {
        SDL_DestroyTexture(enemigosGoblins[i].texture);
    }
}



void DibujarOleadasGoblins() {
    for(int i = 0; i < 8; i++) {
        DibujarEnemigo(renderer, enemigosGoblins[i]);
    }
}

void DibujarOleadasDemons() {
    for(int i = 0; i < 8; i++) {
        DibujarEnemigo(renderer, enemigosBigDemon[i]);
    }
}

void LiberarMemoriaDemons() {
    for(int i = 0; i < 8; i++) {
        SDL_DestroyTexture(enemigosBigDemon[i].texture);
    }
}

void LiberarMemoriaOleada(int numeroOleada) {
    for(int i = 0; i < 8; i++) {
        SDL_DestroyTexture(oleadas[numeroOleada][i].texture);
    }
}

void LiberarMemoriaOrbe(int numeroOleada) {
    for(int i = 0; i < 8; i++) {
        SDL_DestroyTexture(orbesExperiencia[numeroOleada][i].texture);
    }
}

// Para crear 8 puntos en un circulo fuera de la pantalla
void calcularPosicionesSpawn(int posicionesSpawn[], int anchoPantalla, int altoPantalla) {
    const int cantidadPuntos = 8; // Cantidad de puntos de spawn
    const double radio = sqrt(pow(anchoPantalla / 2.0, 2) + pow(altoPantalla / 2.0, 2)); // Radio del círculo circunscrito

    for (int i = 0; i < cantidadPuntos; ++i) {
        double angulo = 2 * 3.14159265358979323846 * i / cantidadPuntos; // Ángulo en radianes
        posicionesSpawn[i * 2] = static_cast<int>(anchoPantalla / 2 + radio * cos(angulo)); // Calcula la coordenada x
        posicionesSpawn[i * 2 + 1] = static_cast<int>(altoPantalla / 2 + radio * sin(angulo)); // Calcula la coordenada y
    }
}









SDL_Texture* render_text(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color color, SDL_Rect* rect) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}


SDL_Texture* render_time(SDL_Renderer* renderer, TTF_Font* font, const char* time_string, SDL_Color color, SDL_Rect* rect) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, time_string, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}


// #######################################################################################################
// PROYECTILES                              PROYECTILES                    PROYECTILES                  //
// #######################################################################################################


struct Proyectil {
    SDL_Texture* texture;   // Textura del proyectil
    SDL_Rect pos;           // Posición del proyectil
    Coordenada direccion;   // Dirección del proyectil
    double velocidad;       // Velocidad del proyectil
    bool activo;            // Indica si el proyectil está activo o no
    double dañoProyectil;
    bool haImpactado;
};

// Función para inicializar un proyectil desde el jugador hacia un enemigo
void InicializarProyectil(Proyectil& proyectil,const SDL_Rect* jugadorPosicion, const Enemigo& enemigo) {
    // Inicializa la posición del proyectil en la posición actual del jugador
    proyectil.pos.x = jugadorPosicion->x;
    proyectil.pos.y = jugadorPosicion->y;

    // Calcula la dirección del proyectil hacia el enemigo
    double direccionX = enemigo.pos.x - jugadorPosicion->x;
    double direccionY = enemigo.pos.y - jugadorPosicion->y;

    // Normaliza la dirección del proyectil
    double magnitud = sqrt(direccionX * direccionX + direccionY * direccionY);
    proyectil.direccion.x = direccionX / magnitud;
    proyectil.direccion.y = direccionY / magnitud;

    // Establece la velocidad del proyectil
    proyectil.velocidad = 8.0; // Puedes ajustar la velocidad según lo necesites

    // Carga la textura del proyectil (asegúrate de tener la textura adecuada)
    proyectil.texture = IMG_LoadTexture(renderer, "data/bomb_f0.png");

    // Activa el proyectil
    proyectil.activo = true;

    proyectil.haImpactado = false;
}


// Función para inicializar un enemigo
void InicializarProyectil2(Proyectil& proyectil, SDL_Renderer* renderer, const char* filename, int x, int y, const SDL_Rect* jugadorPosicion) {
    proyectil.texture = IMG_LoadTexture(renderer, filename);
    if (!proyectil.texture) {
        printf("Error cargando la textura del enemigo: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // 32 es el tamaño del proyectil art del enemigo
    proyectil.pos = { x + 10, y + 10, 26, 26 };

    // Establece la velocidad del proyectil
    proyectil.velocidad = 8.0; // Puedes ajustar la velocidad según lo necesites

    proyectil.activo = true;

    proyectil.haImpactado = false;
}

// Función para dibujar un proyectil en el renderer
void DibujarProyectil(SDL_Renderer* renderer, const Proyectil& proyectil) {
    if (proyectil.activo) {
        SDL_RenderCopy(renderer, proyectil.texture, NULL, &proyectil.pos);
    }
}


// Función para mover un proyectil hacia el jugador
void MoverProyectilAlJugador(Proyectil& proyectil, const SDL_Rect* jugador, double velocidad) {
    // Calculamos el vector dirección hacia el jugador
    double direccion_x = jugador->x - proyectil.pos.x;
    double direccion_y = jugador->y - proyectil.pos.y;

    // Normalizamos el vector dirección
    double longitud = sqrt(direccion_x * direccion_x + direccion_y * direccion_y);
    direccion_x /= longitud;
    direccion_y /= longitud;

    // Movemos el proyectil en la dirección hacia el jugador con la velocidad especificada
    proyectil.pos.x += (int)(direccion_x * velocidad);
    proyectil.pos.y += (int)(direccion_y * velocidad);
}


// Función para mover un proyectil hacia el enemigo
void MoverProyectilAlEnemigo(Proyectil& proyectil, Enemigo enemigo, double velocidad) {
    // Calculamos el vector dirección hacia el enemigo
    double direccion_x = enemigo.pos.x - proyectil.pos.x;
    double direccion_y = enemigo.pos.y - proyectil.pos.y;

    // Normalizamos el vector dirección
    double longitud = sqrt(direccion_x * direccion_x + direccion_y * direccion_y);
    direccion_x /= longitud;
    direccion_y /= longitud;

    // Movemos el proyectil en la dirección hacia el enemigo con la velocidad especificada
    proyectil.pos.x += (int)(direccion_x * velocidad);
    proyectil.pos.y += (int)(direccion_y * velocidad);
}

// Función para detectar colisión entre un proyectil y un enemigo
bool detectarColisionProyectilEnemigo(const Proyectil& proyectil, const Enemigo& enemigo) {
    if (proyectil.activo) {
        // Verificar si las coordenadas del proyectil y del enemigo se superponen
        if (proyectil.pos.x < enemigo.pos.x + enemigo.pos.w &&
            proyectil.pos.x + proyectil.pos.w > enemigo.pos.x &&
            proyectil.pos.y < enemigo.pos.y + enemigo.pos.h &&
            proyectil.pos.y + proyectil.pos.h > enemigo.pos.y) {
            // Colisión detectada
            return true;
        }
    }
    return false;
}

bool detectarColisionProyectilEnemigoOleadas(const Proyectil& proyectil, const Enemigo oleadas[][8], int numOleadas) {
    if (proyectil.activo) {
        // Iterar sobre todas las oleadas y enemigos dentro de cada oleada
        for (int i = 0; i < numOleadas; i++) {
            for (int j = 0; j < 8; j++) {
                const Enemigo& enemigo = oleadas[i][j];
                // Verificar si las coordenadas del proyectil y del enemigo se superponen
                if (proyectil.pos.x < enemigo.pos.x + enemigo.pos.w &&
                    proyectil.pos.x + proyectil.pos.w > enemigo.pos.x &&
                    proyectil.pos.y < enemigo.pos.y + enemigo.pos.h &&
                    proyectil.pos.y + proyectil.pos.h > enemigo.pos.y) {
                    // Colisión detectada con un enemigo
                    return true;
                }
            }
        }
    }
    return false;
}

// #########################################################
// ORBE EXP     7u7
// #########################################################


// Función para inicializar un orbe
void InicializarOrbeExperiencia(SDL_Renderer* renderer, const char* filename, const Enemigo& enemigo, int i, int j) {
    orbesExperiencia[i][j].texture = IMG_LoadTexture(renderer, filename);
    if (!orbesExperiencia[i][j].texture) {
        printf("Error cargando la textura del enemigo: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // 32 es el tamaño del proyectil art del enemigo
    orbesExperiencia[i][j].pos = { enemigo.pos.x , enemigo.pos.y + 20, 20, 20 };

    orbesExperiencia[i][j].isAlive = true;
}


// Función para dibujar un proyectil en el renderer
void DibujarOrbeExperiencia(SDL_Renderer* renderer, const OrbeExperiencia& orbeExperiencia) {
    SDL_RenderCopy(renderer, orbeExperiencia.texture, NULL, &orbeExperiencia.pos);
}


// Función para detectar colisión entre el jugador y un enemigo
bool detectarColisionJugadorOrbeExperiencia(SDL_Rect* jugador, OrbeExperiencia& orbeExperiencia) {
    // Verificar si las coordenadas del jugador y del enemigo se superponen
    if (jugador->x < orbeExperiencia.pos.x + orbeExperiencia.pos.w &&
        jugador->x + jugador->w > orbeExperiencia.pos.x &&
        jugador->y < orbeExperiencia.pos.y + orbeExperiencia.pos.h &&
        jugador->y + jugador->h > orbeExperiencia.pos.y) {
        // Colisión detectada, aquí puedes agregar la lógica correspondiente
        // por ejemplo, decrementar la salud del jugador
        printf("¡Colisión detectada entre el jugador y un enemigo!\n");
        return true;
    }

    return false;
}







// MAQUINA ESTADOS JUGADOR

enum EstadoJugador {
    IDLE,
    RUN,
    MUERTE
};


EstadoJugador estadoJugador = IDLE;

// Render the animation
void renderizarAnimacionJugador() {
    //animacionJugador.Renderizar();
    SDL_RenderPresent(renderer);
}





// SINO EL COMPILADOR NO ENCUENTRA ESTAS VARIABLES
// QUE SE USAN CON "extern SDL_Renderer* renderer;""
SDL_Renderer* renderer;
SDL_Window* window;
Interfaz* interfaz;



//##################################################################################################################
//##################################################################################################################
//##################################################################################################################
//##################################################################################################################
//##################################################################################################################
//#########################################          ###############################################################
//#########################################   MAIN   ###############################################################
//#########################################          ###############################################################
//##################################################################################################################
//##################################################################################################################
//##################################################################################################################
//##################################################################################################################




// MAIN
int main(int argc, char** argv) 
{

    romfsInit();
    chdir("romfs:/");

    // Obtiene la ventana predeterminada
    //NWindow* win = nwindowGetDefault();

    // Ajustar el tiempo en función de los fotogramas por segundo
    int frames_per_second = 60; // Asumiendo que el juego se ejecuta a 60 fotogramas por segundo
    int frames_per_minute = frames_per_second * 60; // Fotogramas por minuto
    int total_frames = minutes * frames_per_minute + seconds * frames_per_second;
    
    int cooldownProyectil = 0;



    // JUGADOR BARRA DE VIDA
    // 100 la barra es muy larga
    int vidaJugador = 60;

    
    // CAlcular circulo fuera de la pantalla
    //const int anchoPantalla = 1280;
    //const int altoPantalla = 720;
    //int posicionesSpawn[16]; // Definimos un array de 16 posiciones

    //calcularPosicionesSpawn(posicionesSpawn, anchoPantalla, altoPantalla);


    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

    window = SDL_CreateWindow("Age of Apocalypse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    TTF_Font* font = TTF_OpenFont("data/LeroyLetteringLightBeta01.ttf", 36);
    SDL_Rect helloworld_rect = { 0, SCREEN_H - 36, 0, 0 };
    //SDL_Rect helloworld_rect = { SCREEN_W / 2, 36, 0, 0 };
    SDL_Color white = { 255, 255, 255, 0 };
    SDL_Color green = { 0, 255, 0, 0 };
    SDL_Texture* helloworld_tex = render_text(renderer, ":(", font, white, &helloworld_rect);


    // BACKGROUND IMAGEN

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "data/background/Background2.png");
    if (!backgroundTexture) {
        printf("Error cargando la textura de fondo: %s\n", IMG_GetError());
    }

    // GAME OVER RECTA

    SDL_Rect gameOverRect = { (SCREEN_W / 2) - 80, (SCREEN_H  / 2) + 80, 0, 0 };
    SDL_Texture* gameOverTexture;

    // Experiencia por pantalla

    SDL_Rect nivelExpRect = { 20, 700, 0, 0 };
    SDL_Texture* nivelExpTexture;
    SDL_Rect expRect = { 20, 650, 0, 0 };
    SDL_Texture* expTexture;

    // EL PEPE
    SDL_Rect proyectilPuntoImpact = { (SCREEN_W / 2) - 80, (SCREEN_H  / 2) + 80, 0, 0 };
    SDL_Texture* proyectilImpacto;

    // JOYSTICK

    SDL_Joystick* joystick = SDL_JoystickOpen(0);
    if (!joystick) 
	{
        printf("Error: no se pudo abrir el joystick.\n");
        return 1;
    }

    int joystick_deadzone = 8000;
	//int joystick_speed = 1;
    

    SDL_Surface* jugadorSurface = IMG_Load("data/pumpkin_dude/pumpkin_dude_idle_anim_f0.png");
    // Multiplica las dimensiones originales del sprite por un factor de escala (por ejemplo, 2 para duplicar el tamaño)
    SDL_Texture* jugadorTextura = SDL_CreateTextureFromSurface(renderer, jugadorSurface);               //jugadorSurface->w * 2 Para x2 tamaño
    jugadorPosicion = { SCREEN_W / 2 - jugadorSurface->w, SCREEN_H / 2 - jugadorSurface->h, jugadorSurface->w * 1.5,  jugadorSurface->h * 1.5};
    SDL_FreeSurface(jugadorSurface);


    // Para el tiempo
    int minutes = 0; // Variable para los minutos
    int seconds = 59;  // Variable para los segundos
    SDL_Rect tiempo_rect = { SCREEN_W / 2, 36, 0, 0 }; // Posición del texto del tiempo
    


    // Inicializar las variables relacionadas con el tiempo
    SDL_Color red = { 255, 0, 0, 0 }; // Color del texto del tiempo
    char tiempo[6]; // Variable para almacenar el tiempo en formato de cadena

    // Generar el texto del tiempo en formato de cadena
    sprintf(tiempo, "%02d:%02d", minutes, seconds);

    // Renderizar el texto del tiempo en la pantalla
    SDL_Texture* tiempo_tex = render_text(renderer, tiempo, font, red, &tiempo_rect);

    // BARRA DE VIDA
    

    SDL_Surface* vidaJugadorSurface = IMG_Load("data/barraVida.png");
    SDL_Texture* vidaJugadorTextura = SDL_CreateTextureFromSurface(renderer, vidaJugadorSurface);

    SDL_FreeSurface(vidaJugadorSurface);




    // MUSICA
    Mix_Music* music = NULL;
    Mix_Chunk* sound[4] = { NULL }; 
    u64 snd;

    music = Mix_LoadMUS("data/backgroundMusic.ogg");
    sound[0] = Mix_LoadWAV("data/pop1.wav");
    sound[1] = Mix_LoadWAV("data/pop2.wav");
    sound[2] = Mix_LoadWAV("data/pop3.wav");
    sound[3] = Mix_LoadWAV("data/pop4.wav");

    if (music)
        Mix_PlayMusic(music, -1);

    

    // CONTROLES
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);
    
    // #######################################
    // CREAR ENEMIGOS
    // #######################################





    // #######################################
    // PROYECTILES
    // #######################################
    
    // INICIALIZAR UN PROYECTIL DESDE EL JUGADOR
    Proyectil proyectilJugador;

    // INICIALIZAR UN ORBE DESDE EL ENEMIGO
    OrbeExperiencia orbeExperiencia;
    
    
    
    // BUCLE
    int exit_requested = 0;

    while (!exit_requested && appletMainLoop()) 
    {

        int current_minutes;
        int current_seconds;
        
        // Dentro del bucle principal, calcular los minutos y segundos en función del número total de fotogramas
        if (!partidaAcabada) 
        {
            current_minutes = total_frames / frames_per_minute;
            current_seconds = (total_frames % frames_per_minute) / frames_per_second;
        }

       // Manejar la entrada del joystick
        SDL_JoystickUpdate();
        int x = SDL_JoystickGetAxis(joystick, 0);
        int y = SDL_JoystickGetAxis(joystick, 1);

        // Determinar los movimientos en función de la entrada del joystick
        if (x < -joystick_deadzone && jugadorPosicion.x >= 80 && !partidaAcabada) {
            jugadorPosicion.x -= velocidadMovimiento;
        } else if (x > joystick_deadzone && jugadorPosicion.x <= 1180 && !partidaAcabada) {
            jugadorPosicion.x += velocidadMovimiento;
        }

        if (y < -joystick_deadzone && jugadorPosicion.y >= 46 && !partidaAcabada) {
            jugadorPosicion.y -= velocidadMovimiento;
        } else if (y > joystick_deadzone && jugadorPosicion.y <= 650 && !partidaAcabada) {
            jugadorPosicion.y += velocidadMovimiento;
        }


        // Actualizar la posición del personaje en función del estado de los botones
        switch (estadoJugador) {
            case IDLE:
                break;
            case RUN:

                    
                // Lógica para el estado run
                break;
            case MUERTE:
                // Lógica para el estado muerte
                break;
        }

        //############################################################################
        //  MOVER ENEMIGOS          MOVER ENEMIGOS              MOVER ENEMIGOS      //
        //############################################################################

        //generar oleadas
        if (!partidaAcabada) 
        {
            if (current_minutes == 5 && current_seconds == 0) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 1", font, white, &helloworld_rect);
                InicializarOleada(1);
            } else if (current_minutes == 4 && current_seconds == 45) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 2", font, white, &helloworld_rect);
                InicializarOleada(2);
            } else if (current_minutes == 4 && current_seconds == 30) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 3", font, white, &helloworld_rect);
                InicializarOleada(3);
            } else if (current_minutes == 4 && current_seconds == 15) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 4", font, white, &helloworld_rect);
                InicializarOleada(4);
            } else if (current_minutes == 4 && current_seconds == 0) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 5", font, white, &helloworld_rect);
                InicializarOleada(5);
            } else if (current_minutes == 3 && current_seconds == 45) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 6", font, white, &helloworld_rect);
                InicializarOleada(6);
            } else if (current_minutes == 3 && current_seconds == 30) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 7", font, white, &helloworld_rect);
                InicializarOleada(7);
            } else if (current_minutes == 3 && current_seconds == 15) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 8", font, white, &helloworld_rect);
                InicializarOleada(8);
            } else if (current_minutes == 3 && current_seconds == 0) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 9", font, white, &helloworld_rect);
                InicializarOleada(9);
            } else if (current_minutes == 2 && current_seconds == 45) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 10", font, white, &helloworld_rect);
                InicializarOleada(10);
            } else if (current_minutes == 2 && current_seconds == 30) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 11", font, white, &helloworld_rect);
                InicializarOleada(11);
            } else if (current_minutes == 2 && current_seconds == 15) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 12", font, white, &helloworld_rect);
                InicializarOleada(12);
            } else if (current_minutes == 2 && current_seconds == 0) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 13", font, white, &helloworld_rect);
                InicializarOleada(13);
            } else if (current_minutes == 1 && current_seconds == 45) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 14", font, white, &helloworld_rect);
                InicializarOleada(14);
            } else if (current_minutes == 1 && current_seconds == 30) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 15", font, white, &helloworld_rect);
                InicializarOleada(15);
            } else if (current_minutes == 1 && current_seconds == 15) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 16", font, white, &helloworld_rect);
                InicializarOleada(16);
            } else if (current_minutes == 1 && current_seconds == 0) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 17", font, white, &helloworld_rect);
                InicializarOleada(17);
            } else if (current_minutes == 0 && current_seconds == 45) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 18", font, white, &helloworld_rect);
                InicializarOleada(18);
            } else if (current_minutes == 0 && current_seconds == 30) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 19", font, white, &helloworld_rect);
                InicializarOleada(19);
            } else if (current_minutes == 0 && current_seconds == 15) {
                helloworld_tex = render_text(renderer, "Spawneando oleada 20", font, white, &helloworld_rect);
                InicializarOleada(20);
            }
        }
        

        // Sistema de experiencia

        nivelExperiencia = contadorDeExperiencia / 16;
        porcentajeDeNivel = (contadorDeExperiencia % 16) / 16;

        switch (nivelExperiencia)
        {
        case 1:
            danoProyectil = 15;
            COOLDOWNProyectil_MAX = 60;
            velocidadMovimiento = 5.0f;
            break;
        case 2:
            danoProyectil = 15;
            COOLDOWNProyectil_MAX = 60;
            velocidadMovimiento = 6.0f;
            break;
        case 3:
            danoProyectil = 15;
            COOLDOWNProyectil_MAX = 50;
            velocidadMovimiento = 6.0f;
            break;
        case 4:
            danoProyectil = 20;
            COOLDOWNProyectil_MAX = 50;
            velocidadMovimiento = 6.0f;
            break;
        case 5:
            danoProyectil = 20;
            COOLDOWNProyectil_MAX = 50;
            velocidadMovimiento = 8.0f;
            break;
        case 6:
            danoProyectil = 20;
            COOLDOWNProyectil_MAX = 40;
            velocidadMovimiento = 8.0f;
            break;
        case 7:
            danoProyectil = 25;
            COOLDOWNProyectil_MAX = 40;
            velocidadMovimiento = 8.0f;
            break;
        case 8:
            danoProyectil = 30;
            COOLDOWNProyectil_MAX = 40;
            velocidadMovimiento = 8.0f;
            break;
        case 9:
            danoProyectil = 30;
            COOLDOWNProyectil_MAX = 40;
            velocidadMovimiento = 10.0f;
            break;
        case 10:
            danoProyectil = 30;
            COOLDOWNProyectil_MAX = 30;
            velocidadMovimiento = 10.0f;
            break;
        default:
            break;
        }

        nivelExpTexture = render_text(renderer, ("Nivel: " + std::to_string(nivelExperiencia)).c_str(), font, red, &gameOverRect);
        expTexture = render_text(renderer, ("Porcentaje de nivel: " + std::to_string(porcentajeDeNivel)).c_str(), font, red, &gameOverRect);

        // ##################
        // COOLDOWN PROYECTIL
        // ##################

        Enemigo enemigoADisparar;
        bool enemigoEncontrado = false;

        for(int i = 0; i < cantidadOleadasVivas && !enemigoEncontrado; i++) {
            for (int j = 0; j < 8 && !enemigoEncontrado; j++) {
                if (oleadas[oleadasVivas[i]][j].isAlive) {
                    enemigoADisparar = oleadas[oleadasVivas[i]][j];
                    enemigoEncontrado = true;
                }
            }
        }


        if (cooldownProyectil > 0) {
            cooldownProyectil--;      

            MoverProyectilAlEnemigo(proyectilJugador, enemigoADisparar, 8.0); // 5.0 es la velocidad del proyectil, ajusta según lo necesites
        }

       

        if (cooldownProyectil == 0) {
            InicializarProyectil2(proyectilJugador, renderer, "data/bomb_f0.png", jugadorPosicion.x , jugadorPosicion.y, &jugadorPosicion);
            //MoverProyectilAlEnemigo(proyectilJugador, goblin, 8.0); // 5.0 es la velocidad del proyectil, ajusta según lo necesites
            
            Mix_PlayChannel(-1, sound[0], 0); // SONIDITO DE DISPARO OH
            helloworld_tex = render_text(renderer, "MISIL DISPARADO RAHHH", font, white, &helloworld_rect);
            cooldownProyectil = COOLDOWNProyectil_MAX; // Establece el cooldownProyectil original
        }


        // ########################
        // GAME OVER / FIN PARTIDA
        // ########################

        if(vidaJugador <= 0) {
            gameOverTexture = render_text(renderer, "GAME OVER", font, red, &gameOverRect);
            partidaAcabada = true;
        } else if(current_minutes <= 0 && current_seconds <= 0) {
            gameOverTexture = render_text(renderer, "YOU HAVE SURVIDED", font, red, &gameOverRect);
            partidaAcabada = true;
        }


        // LIMPIAR LA PANTALLA
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);



        // ########################
        // DIBUJAR A PARTIR DE AQUI
        // ########################

        estadoJugador = RUN;
        // renderizarAnimacionJugador();

        // Renderizar imagen de fondo
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        if (!proyectilJugador.haImpactado) {
            DibujarProyectil(renderer, proyectilJugador);
        }

        // Movimiento, colisiones y render de los enemigos

        int contadorEnemigosMuertos = 0;
        bool auxOrbe = false;

        if (!partidaAcabada) 
        {
            for(int i = 0; i < cantidadOleadasVivas; i++) {
                for (int j = 0; j < 8; j++) {
                    if (oleadas[oleadasVivas[i]][j].isAlive) {
                        MoverEnemigoHaciaElJugador(oleadas[oleadasVivas[i]][j], &jugadorPosicion, 5.0f);
                        if(detectarColisionJugadorEnemigo(&jugadorPosicion, oleadas[oleadasVivas[i]][j])) {
                            helloworld_tex = render_text(renderer, "COLISION", font, red, &helloworld_rect);
                            vidaJugador -= 0.5;
                        }
                        if(detectarColisionProyectilEnemigo(proyectilJugador, oleadas[oleadasVivas[i]][j]) 
                        && oleadas[oleadasVivas[i]][j].tiempoUltimoImpacto >= (current_minutes * 60 + current_seconds + 1)) 
                        {
                            proyectilJugador.haImpactado = true;
                            oleadas[oleadasVivas[i]][j].tiempoUltimoImpacto = (current_minutes * 60 + current_seconds);
                            oleadas[oleadasVivas[i]][j].life -= danoProyectil;
                            if (oleadas[oleadasVivas[i]][j].life <= 0) {
                                // TRAS IMPACTAR A ENEMIGO CREAR ORBE EXP
                                InicializarOrbeExperiencia(renderer, "data/orbe_experiencia.png", oleadas[oleadasVivas[i]][j], i, j);

                                auxOrbe = false;

                                for (int l = 0; l < cantidadDeOrbesVivos; l++) {
                                    if (oleadasOrbesVivos[cantidadDeOrbesVivos] == i) {
                                        auxOrbe = true;
                                    }
                                }

                                if (!auxOrbe) {
                                    oleadasOrbesVivos[cantidadDeOrbesVivos] = i;
                                    quickSort(oleadasOrbesVivos, 0, cantidadDeOrbesVivos - 1);
                                    cantidadDeOrbesVivos++;
                                }


                                oleadas[oleadasVivas[i]][j].isAlive = false;
                            }
                        }
                        DibujarEnemigo(renderer,oleadas[oleadasVivas[i]][j]);
                    } else {
                        contadorEnemigosMuertos++;
                        if (contadorEnemigosMuertos == 8) {
                            for(int k = i; k < cantidadOleadasVivas -1; k++) {
                                oleadasVivas[k] = oleadasVivas[k + 1];
                            }
                            oleadasVivas[cantidadOleadasVivas] = 0;
                            cantidadOleadasVivas--;
                            LiberarMemoriaOleada(i);
                        }
                    }
                }
                contadorEnemigosMuertos = 0;
            }
        }

        int contadorOrberRecogidos = 0;
        
        if (!partidaAcabada) 
        {
            for(int i = 0; i < cantidadDeOrbesVivos; i++) {
                for (int j = 0; j < 8; j++) {
                    if (orbesExperiencia[oleadasOrbesVivos[i]][j].isAlive) {
                        if (detectarColisionJugadorOrbeExperiencia(&jugadorPosicion, orbesExperiencia[oleadasOrbesVivos[i]][j])) {
                            orbesExperiencia[oleadasOrbesVivos[i]][j].isAlive = false;
                            contadorDeExperiencia++;
                        }
                        DibujarOrbeExperiencia(renderer, orbesExperiencia[oleadasOrbesVivos[i]][j]);
                    } else {
                        contadorOrberRecogidos++;
                        if (contadorOrberRecogidos == 8) {
                            for(int k = i; k < cantidadDeOrbesVivos -1; k++) {
                                oleadasOrbesVivos[k] = oleadasOrbesVivos[k + 1];
                            }
                            oleadasOrbesVivos[cantidadDeOrbesVivos] = 0;
                            cantidadDeOrbesVivos--;
                            LiberarMemoriaOrbe(i);
                        }
                    }
                }
                contadorOrberRecogidos = 0;
            }
        }

        // Dibujar el personaje
        SDL_RenderCopy(renderer, jugadorTextura, NULL, &jugadorPosicion);
        
        // Actualizar barra de Vida y Dibujarla
        // Barra centrada
        int vidaBarraX = jugadorPosicion.x + (jugadorPosicion.w - vidaJugador) / 2;
        SDL_Rect vidaJugadorPosicion = {vidaBarraX, jugadorPosicion.y - 10, vidaJugador, 20};
        SDL_RenderCopy(renderer, vidaJugadorTextura, NULL, &vidaJugadorPosicion);

       


        // Actualización del tiempo
        total_frames--; // Incrementar el número total de fotogramas

        // Calcular los minutos y segundos actuales en función del número total de fotogramas
        if (!partidaAcabada) 
        {
            current_minutes = total_frames / frames_per_minute;
            current_seconds = (total_frames % frames_per_minute) / frames_per_second;
        }


        // Si los segundos llegan a cero, decrementar los minutos y resetear los segundos a 59
        if (current_seconds < 0) 
        {
            current_seconds = 59;
            current_minutes--;
        }

        // Si los minutos llegan a cero y los segundos llegan a cero, detener el juego
        if (current_minutes <= 0 && current_seconds <= 0) 
        {
            exit_requested = 1; // Salir del bucle
            // Aquí puedes agregar cualquier lógica adicional que necesites al finalizar el tiempo
        }
       
        // Actualizar el texto del tiempo
        sprintf(tiempo, "%02d:%02d", current_minutes, current_seconds);

        // Renderizar el tiempo
        tiempo_tex = render_text(renderer, tiempo, font, red, &tiempo_rect);
        
        if (tiempo_tex)
            SDL_RenderCopy(renderer, tiempo_tex, NULL, &tiempo_rect);
        
        
        // Dibujar el texto en la parte inferior de la pantalla
        SDL_RenderCopy(renderer, helloworld_tex, NULL, &helloworld_rect);
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        SDL_RenderCopy(renderer, nivelExpTexture, NULL, &nivelExpRect);
        SDL_RenderCopy(renderer, expTexture, NULL, &expRect);

        

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / frames_per_second);
    } // FIN BUCLE MAIN


    SDL_DestroyTexture(jugadorTextura);
    SDL_DestroyTexture(helloworld_tex);
    SDL_DestroyTexture(tiempo_tex);
    LiberarMemoriaGoblins();
    LiberarMemoriaDemons();

    // animacionJugador.Destruir();



    // Parar sonidos y liberar data 
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music);
    for (snd = 0; snd < 4; snd++)
        if (sound[snd])
            Mix_FreeChunk(sound[snd]);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    SDL_JoystickClose(joystick);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    romfsExit();
    return 0;
    }


