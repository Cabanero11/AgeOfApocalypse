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

// EN TEORIA AÑADIR COSAS AQUI SI NUEVAS CLASES
#include "gameEngine.h"


// LAS CONSTANTES SE DEFINEN EN constantes.h

// Variables de movimiento
bool move_up = false;
bool move_down = false;
bool move_left = false;
bool move_right = false;

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
};

// Función para inicializar un enemigo
void InicializarEnemigo(Enemigo& enemigo, SDL_Renderer* renderer, const char* filename, int x, int y) {
    enemigo.texture = IMG_LoadTexture(renderer, filename);
    if (!enemigo.texture) {
        printf("Error cargando la textura del enemigo: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // Posicionar el enemigo a la derecha del jugador
    // 32 es el tamaño del pixel art del enemigo
    enemigo.pos = { x + 15, y, 26, 26 };

    enemigo.coord = { (double)x, (double)y };
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



// INTENDO DE OLEADAS


Enemigo enemigosGoblins[8];
Enemigo enemigosBigDemon[16];

#define NUM_ENEMIGOS_DEMON 16
#define NUM_OLEADAS_DEMON 2


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



/// PROYECTILES


struct ProyectilMagico {
    SDL_Texture* texture;   // Textura del proyectil mágico
    SDL_Rect pos;           // Posición del proyectil mágico
    Coordenada direccion;   // Dirección del proyectil mágico
    double velocidad;       // Velocidad del proyectil mágico
};

ProyectilMagico crearProyectilMagico(SDL_Renderer* renderer, const SDL_Rect* jugadorPosicion, Enemigo* enemigos) {
    ProyectilMagico proyectil;

    // Inicializa la posición del proyectil en la posición actual del jugador
    proyectil.pos.x = jugadorPosicion->x;
    proyectil.pos.y = jugadorPosicion->y;

    // Establece la velocidad del proyectil
    proyectil.velocidad = 4.0; // Puedes ajustar la velocidad según lo necesites

    // Carga la textura del proyectil mágico (asegúrate de tener la textura adecuada)
    proyectil.texture = IMG_LoadTexture(renderer, "data/bomb_f0.png");

    return proyectil;
}

void dispararProyectilMagico(SDL_Renderer* renderer, const SDL_Rect* jugadorPosicion, Enemigo* enemigoMasCercano) {
    ProyectilMagico proyectil = crearProyectilMagico(renderer, jugadorPosicion, enemigoMasCercano);

    // Calcula la dirección del proyectil basándose en el enemigo más cercano
    double direccionX = enemigoMasCercano->pos.x - proyectil.pos.x;
    double direccionY = enemigoMasCercano->pos.y - proyectil.pos.y;

    // Normaliza la dirección del proyectil
    double magnitud = sqrt(direccionX * direccionX + direccionY * direccionY);
    proyectil.direccion.x = direccionX / magnitud;
    proyectil.direccion.y = direccionY / magnitud;

    // Por ahora, simplemente dibujaremos el proyectil en la pantalla
    SDL_RenderCopy(renderer, proyectil.texture, NULL, &proyectil.pos);
}






// VARIABLES DE ANIMACION

const char* idleJugador[] = {
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f0.png",
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f1.png",
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f2.png",
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f3.png"
};


const char* runJugador[] = {
    "data/pumpkin_dude/pumpkin_dude_run_anim_f0.png",
    "data/pumpkin_dude/pumpkin_dude_run_anim_f1.png",
    "data/pumpkin_dude/pumpkin_dude_run_anim_f2.png",
    "data/pumpkin_dude/pumpkin_dude_run_anim_f3.png"
};




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
    const int COOLDOWNProyectil_MAX = 60; // Tiempo de cooldownProyectil en frames (ajusta según necesites)



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
    SDL_Rect jugadorPosicion = { SCREEN_W / 2 - jugadorSurface->w, SCREEN_H / 2 - jugadorSurface->h, jugadorSurface->w * 1.5,  jugadorSurface->h * 1.5};
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

    // ENEMIGO GOBLIN
    Enemigo goblin;
    InicializarEnemigo(goblin, renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 300, jugadorPosicion.y);


    //Enemigo enemigosGoblins[8];

    InicializarEnemigo(enemigosGoblins[0], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 650, jugadorPosicion.y + 0);
    InicializarEnemigo(enemigosGoblins[1], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 650, jugadorPosicion.y + 100);
    InicializarEnemigo(enemigosGoblins[2], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 650, jugadorPosicion.y + 150);
    InicializarEnemigo(enemigosGoblins[3], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 650, jugadorPosicion.y - 450);
    InicializarEnemigo(enemigosGoblins[4], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y + 740);
    InicializarEnemigo(enemigosGoblins[5], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 200, jugadorPosicion.y + -80);
    InicializarEnemigo(enemigosGoblins[6], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 0, jugadorPosicion.y - 200);
    InicializarEnemigo(enemigosGoblins[7], renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x - 200, jugadorPosicion.y + -200);

    // ENEMIGO DEMONS

    int posicionesDemons[][2] = {
        { jugadorPosicion.x + 850, jugadorPosicion.y + 0 },
        { jugadorPosicion.x + 850, jugadorPosicion.y + 500 },
        { jugadorPosicion.x - 850, jugadorPosicion.y + 620 },
        { jugadorPosicion.x - 850, jugadorPosicion.y - 550 },
        { jugadorPosicion.x + 0, jugadorPosicion.y + 840 },
        { jugadorPosicion.x + 500, jugadorPosicion.y - 280 },
        { jugadorPosicion.x + 0, jugadorPosicion.y - 210 },
        { jugadorPosicion.x - 500, jugadorPosicion.y - 300 }
    };
    

    // INICIALIZAR 8 DEMONS

    for (int i = 0; i < 8; i++) {
        InicializarEnemigo(enemigosBigDemon[i], renderer, "data/big_demon/big_demon_idle_anim_f3.png", posicionesDemons[i][0], posicionesDemons[i][1]);
    }
    

    // #######################################
    // PROYECTILES
    // #######################################
    
    // INICIALIZAR UN PROYECTIL DESDE EL JUGADOR

    
    
    
    // BUCLE
    int exit_requested = 0;

    while (!exit_requested && appletMainLoop()) 
    {
        
        // Dentro del bucle principal, calcular los minutos y segundos en función del número total de fotogramas
        int current_minutes = total_frames / frames_per_minute;
        int current_seconds = (total_frames % frames_per_minute) / frames_per_second;

       // Manejar la entrada del joystick
        SDL_JoystickUpdate();
        int x = SDL_JoystickGetAxis(joystick, 0);
        int y = SDL_JoystickGetAxis(joystick, 1);

        // Determinar los movimientos en función de la entrada del joystick
        if (x < -joystick_deadzone && jugadorPosicion.x >= 80) {
            jugadorPosicion.x -= velocidadMovimiento;
        } else if (x > joystick_deadzone && jugadorPosicion.x <= 1180) {
            jugadorPosicion.x += velocidadMovimiento;
        }

        if (y < -joystick_deadzone && jugadorPosicion.y >= 46) {
            jugadorPosicion.y -= velocidadMovimiento;
        } else if (y > joystick_deadzone && jugadorPosicion.y <= 650) {
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

        // Mover el enemigo hacia el jugador (pumpkin)
        MoverEnemigoHaciaElJugador(goblin, &jugadorPosicion, 5.0f);

        // Mover 8 goblins hacia el jugador
        if(current_minutes <= 4 && current_seconds <= 55) {
            for(int i = 0; i < 8; i++) {
                MoverEnemigoHaciaElJugador(enemigosGoblins[i], &jugadorPosicion, 5.0f);
            }
        }

        
        
        // Mover 8 demons hacia el jugador tras 30 segundos
        if(current_minutes <= 4 && current_seconds <= 30) {
            for(int i = 0; i < 8; i++) {
                MoverEnemigoHaciaElJugador(enemigosBigDemon[i], &jugadorPosicion, 5.0f);
            }
        }
        
        // MOVIMIENTO DEL PROYECTIL

        // DETECTAR COLISIONES CON EL JUGADOR

        // goblin solito :(
        if(detectarColisionJugadorEnemigo(&jugadorPosicion, goblin)) {
            helloworld_tex = render_text(renderer, "COLISION GOBLIN", font, white, &helloworld_rect);
            vidaJugador -= 0.5;
        }

        // demons colisiones
        for (int i = 0; i < 8; i++) {
            if(detectarColisionJugadorEnemigo(&jugadorPosicion, enemigosGoblins[i])) {
                helloworld_tex = render_text(renderer, "COLISION GOBLIN", font, white, &helloworld_rect);
                vidaJugador -= 0.5;
            }
        }

        // demons colisiones
        for (int i = 0; i < 8; i++) {
            if(detectarColisionJugadorEnemigo(&jugadorPosicion, enemigosBigDemon[i])) {
                helloworld_tex = render_text(renderer, "COLISION DEMON", font, white, &helloworld_rect);
                vidaJugador -= 1;
            }
        }



        if (cooldownProyectil > 0) {
            cooldownProyectil--;
            //helloworld_tex = render_text(renderer, "MISIL COOLDOWN JOO", font, white, &helloworld_rect);
        }

        double distanciaMinima = 1.0f;
        Enemigo* enemigoMasCercano;
        // Itera sobre todos los enemigos
        for (int i = 0; i < 8; ++i) {
            // Calcula la distancia entre el jugador y el enemigo actual
            double distanciaActual = distancia(Coordenada{jugadorPosicion.x, jugadorPosicion.y}, {enemigosGoblins[i].pos.x, enemigosGoblins[i].pos.y});

            // Si la distancia actual es menor que la distancia mínima encontrada hasta ahora
            if (distanciaActual < distanciaMinima) {
                // Actualiza la distancia mínima
                distanciaMinima = distanciaActual;
                // Actualiza el puntero al enemigo más cercano
                enemigoMasCercano = &enemigosGoblins[i];
            }
        }

        if (cooldownProyectil == 0) {
            dispararProyectilMagico(renderer, &jugadorPosicion, &goblin);
            

            Mix_PlayChannel(-1, sound[0], 0); // SONIDITO DE DISPARO OH
            helloworld_tex = render_text(renderer, "MISIL DISPARADO RAHHH", font, white, &helloworld_rect);
            cooldownProyectil = COOLDOWNProyectil_MAX; // Establece el cooldownProyectil original
        }


        // ########################
        // GAME OVER / FIN PARTIDA
        // ########################

        if(vidaJugador <= 0) {
            gameOverTexture = render_text(renderer, "GAME OVER", font, red, &gameOverRect);
            // LINEA ABAJO CIERRA EL JUEGO DIRECTAMENTE
            //exit_requested = 1;
        } else if(current_minutes <= 0 && current_seconds <= 0) {
            gameOverTexture = render_text(renderer, "YOU HAVE SURVIDED", font, red, &gameOverRect);
        }


        // LIMPIAR LA PANTALLA
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);



        // ########################
        // DIBUJAR A PARTIR DE AQUI
        // ########################

        estadoJugador = RUN;
        //renderizarAnimacionJugador();

        // Renderizar imagen de fondo
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);



        // Dibujar el enemigo (goblin)
        DibujarEnemigo(renderer, goblin);
        DibujarOleadasGoblins();
        DibujarOleadasDemons();
        //SDL_RenderCopy(renderer, proyectilJugador.texture, NULL, &proyectilJugador.pos);

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
        current_minutes = total_frames / frames_per_minute;
        current_seconds = (total_frames % frames_per_minute) / frames_per_second;


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

        

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / frames_per_second);
    } // FIN BUCLE MAIN


    SDL_DestroyTexture(jugadorTextura);
    SDL_DestroyTexture(helloworld_tex);
    SDL_DestroyTexture(tiempo_tex);
    SDL_DestroyTexture(goblin.texture); // Destruir la textura del goblin para liberar la memoria
    LiberarMemoriaGoblins();
    LiberarMemoriaDemons();

    //animacionJugador.Destruir();



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


