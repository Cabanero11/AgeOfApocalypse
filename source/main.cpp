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


//Your own raw RGB888 1280x720 image at "data/image.bin" is required.
//#include "image_bin.h"

// LAS CONSTANTES SE DEFINEN EN constantes.h

// Variables de movimiento
bool move_up = false;
bool move_down = false;
bool move_left = false;
bool move_right = false;

float velocidadMovimiento = 5.0f;

// Para el tiempo
#define CPU_TICKS_PER_SECOND 19200000
#define DURATION_SECONDS 600 // 10 minutos en segundos

int minutes = 10; // Variable para los minutos
int seconds = 0;  // Variable para los segundos
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
    enemigo.pos = { x + 15, y, 16, 16 };

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



// Estructura para representar un proyectil
struct Proyectil {
    SDL_Texture* texture;   // Textura del proyectil
    SDL_Rect pos;           // Posición del proyectil
    Coordenada coord;       // Coordenadas del proyectil
    double velocidad;       // Velocidad del proyectil
    double direccion_x;     // Componente x de la dirección del proyectil
    double direccion_y;     // Componente y de la dirección del proyectil
};


// Función para inicializar un proyectil
void InicializarProyectil(Proyectil& proyectil, SDL_Renderer* renderer, const char* filename, int x, int y) {
    proyectil.texture = IMG_LoadTexture(renderer, filename);
    if (!proyectil.texture) {
        printf("Error cargando la textura del proyectil: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // Posicionar el proyectil en la misma posición que el jugador
    proyectil.pos = { x, y, 16, 16 };

    proyectil.coord = { (double)x, (double)y };

    // Definir una velocidad para el proyectil
    proyectil.velocidad = 5.0;

    // Generar una dirección aleatoria para el proyectil
    double angulo = rand() % 360; // Ángulo en grados
    proyectil.direccion_x = cos(angulo * M_PI / 180); // Componente x de la dirección
    proyectil.direccion_y = sin(angulo * M_PI / 180); // Componente y de la dirección
}

// Función para mover un proyectil
void MoverProyectil(Proyectil& proyectil) {
    // Mover el proyectil en su dirección actual
    proyectil.coord.x += proyectil.velocidad * proyectil.direccion_x;
    proyectil.coord.y += proyectil.velocidad * proyectil.direccion_y;

    // Actualizar la posición del proyectil
    proyectil.pos.x = (int)proyectil.coord.x;
    proyectil.pos.y = (int)proyectil.coord.y;
}

// Función para dibujar un proyectil en el renderer
void DibujarProyectil(SDL_Renderer* renderer, const Proyectil& proyectil) {
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



// VARIABLES ANIMACION

// Declarar la variable animacionJugador antes de su uso
Animacion animacionJugador;

const char* animacionJugadorFrames[4] = {
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f0.png",
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f1.png",
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f2.png",
    "data/pumpkin_dude/pumpkin_dude_idle_anim_f3.png"
};




EstadoJugador estadoJugador = IDLE;

// Render the animation
void renderizarAnimacionJugador() {
    animacionJugador.Renderizar();
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

    // Obtiene la ventana predeterminada
    //NWindow* win = nwindowGetDefault();
    
    // Iniciamos para poder leer archivos
	romfsInit();
	chdir("romfs:/");
	
	// Iniciamos lo relacionado con el mundo fisico
	b2Vec2 gravity(0.0, 0.0f);
	b2World world(gravity);
	DetectorDeColisiones detector;
	world.SetContactListener(&detector);
	
	int exit_requested = 0;

	int joystick_deadzone = 8000;
	int joystick_speed = 1;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER);
	IMG_Init(IMG_INIT_PNG);
	
	// Creamos la ventana y el renderer
	window = SDL_CreateWindow("Test Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	
	SDL_Joystick* joystick = SDL_JoystickOpen(0);
    if (!joystick) 
	{
        printf("Error: no se pudo abrir el joystick.\n");
        return 1;
    }
	
	// Creamos los elementos del nivel
	interfaz = new Interfaz();
	Camara2 camara2(0.0f, 0.0f);
	Mapa2 mundo1(RUTA_MAPA_MUNDO_1, &camara2, &world);
	Jugador2 mario(&camara2, &world);
	camara2.AsignarJugador2(&mario);
	
    while (!exit_requested && appletMainLoop()) 
	{
        SDL_SetRenderDrawColor(renderer, mundo1.fondoR, mundo1.fondoG, mundo1.fondoB, 0xFF);
        SDL_RenderClear(renderer);
    
    	// Manejar la entrada del joystick
    	SDL_JoystickUpdate();
    	int x = SDL_JoystickGetAxis(joystick, 0);
    	int y = SDL_JoystickGetAxis(joystick, 1);

        // CONTROLES
        SDL_JoystickEventState(SDL_ENABLE);
        SDL_JoystickOpen(0);

        
   	 	// Actualizar la posición de la imagen en función de la entrada del joystick
    	if (x < -joystick_deadzone)
            mario.MoverIzquierda();
        else if (x > joystick_deadzone)
            mario.MoverDerecha();

        if (y < -joystick_deadzone)
            mario.MoverAbajo();
        else if (y > joystick_deadzone)
            mario.MoverArriba();


    		
    	//camara.x += 6 * x;
    	//camara.y += 6 * y;
	
		// Renderizamos todo lo necesario
		mundo1.Renderizar();
		interfaz->Renderizar();
		mario.Renderizar2(x, y);
        //mario.Renderizar2(y);

		// Actualizamos el mundo fisico
    	world.Step(1.0f / 60.0f, 6, 2);
    	
    	// Renderizamos en pantalla
    	SDL_RenderPresent(renderer);

        

    // MARIO

    // ###################################################################

    // CODIGO ANTIGUO

    /**
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
    SDL_Texture* helloworld_tex = render_text(renderer, "Bye bye!", font, white, &helloworld_rect);



    SDL_Surface* jugadorSurface = IMG_Load("data/pumpkin_dude/pumpkin_dude_idle_anim_f0.png");
    // Multiplica las dimensiones originales del sprite por un factor de escala (por ejemplo, 2 para duplicar el tamaño)
    SDL_Texture* jugadorTextura = SDL_CreateTextureFromSurface(renderer, jugadorSurface);               //jugadorSurface->w * 2 Para x2 tamaño
    SDL_Rect jugadorPosicion = { SCREEN_W / 2 - jugadorSurface->w, SCREEN_H / 2 - jugadorSurface->h, jugadorSurface->w,  jugadorSurface->h };
    SDL_FreeSurface(jugadorSurface);


    // Para el tiempo
    int minutes = 9; // Variable para los minutos
    int seconds = 59;  // Variable para los segundos
    SDL_Rect tiempo_rect = { SCREEN_W / 2, 36, 0, 0 }; // Posición del texto del tiempo

    // Inicializar las variables relacionadas con el tiempo
    SDL_Color red = { 255, 0, 0, 0 }; // Color del texto del tiempo
    char tiempo[6]; // Variable para almacenar el tiempo en formato de cadena

    // Generar el texto del tiempo en formato de cadena
    sprintf(tiempo, "%02d:%02d", minutes, seconds);

    // Renderizar el texto del tiempo en la pantalla
    SDL_Texture* tiempo_tex = render_text(renderer, tiempo, font, red, &tiempo_rect);




    // MUSICA
    Mix_Music* music = NULL;
    Mix_Chunk* sound[4] = { NULL }; 
    u64 snd;

    music = Mix_LoadMUS("data/mewing.ogg");
    sound[0] = Mix_LoadWAV("data/pop1.wav");
    sound[1] = Mix_LoadWAV("data/pop2.wav");
    sound[2] = Mix_LoadWAV("data/pop3.wav");
    sound[3] = Mix_LoadWAV("data/pop4.wav");

    if (music)
        Mix_PlayMusic(music, -1);

    

   
    


    // ENEMIGO GOBLIN
    Enemigo goblin;
    InicializarEnemigo(goblin, renderer, "data/goblin/goblin_idle_anim_f0.png", jugadorPosicion.x + 15, jugadorPosicion.y);


    // IMAGEN DE FONDO
    

    // Crea un framebuffer lineal de doble búfer
    Framebuffer fb;
    framebufferCreate(&fb, win, SCREEN_W, SCREEN_H, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);


    
    // BUCLE
    int exit_requested = 0;

    while (!exit_requested && appletMainLoop()) 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
                exit_requested = 1;

            // Manejar el movimiento continuo mientras se mantiene pulsado el botón
            if (event.type == SDL_JOYBUTTONDOWN) 
            {
                if (event.jbutton.button == JOY_UP)
                    move_up = true;
                else if (event.jbutton.button == JOY_DOWN)
                    move_down = true;
                else if (event.jbutton.button == JOY_LEFT)
                    move_left = true;
                else if (event.jbutton.button == JOY_RIGHT)
                    move_right = true;
            }

            // Manejar la interrupción del movimiento cuando se suelta el botón
            if (event.type == SDL_JOYBUTTONUP) 
            {
                if (event.jbutton.button == JOY_UP)
                    move_up = false;
                else if (event.jbutton.button == JOY_DOWN)
                    move_down = false;
                else if (event.jbutton.button == JOY_LEFT)
                    move_left = false;
                else if (event.jbutton.button == JOY_RIGHT)
                    move_right = false;
            }

            if(event.type == SDL_JOYBUTTONDOWN) 
            {
                if(event.jbutton.button == JOY_A)
                    Mix_PlayChannel(-1, sound[0], 0);
                else if(event.jbutton.button == JOY_B)
                    Mix_PlayChannel(-1, sound[3], 0);
            }
        } // FIN BUCLE EVENTOS


        // Actualizar la posición del personaje en función del estado de los botones
        switch (estadoJugador) {
            case IDLE:
                // Lógica para el estado idle
                break;
            case RUN:
                if (move_up)
                    jugadorPosicion.y -= velocidadMovimiento;
                if (move_down)
                    jugadorPosicion.y += velocidadMovimiento;
                if (move_left)
                    jugadorPosicion.x -= velocidadMovimiento;
                if (move_right)
                    jugadorPosicion.x += velocidadMovimiento;
                // Lógica para el estado run
                break;
            case MUERTE:
                // Lógica para el estado muerte
                break;
        }


        // Mover el enemigo hacia el jugador (pumpkin)
        MoverEnemigoHaciaElJugador(goblin, &jugadorPosicion, 5.0f);


        // LIMPIAR LA PANTALLA
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        // CARGAR IMAGEN FONDO
      

        // CREAR ANIMACIO
        animacionJugador.CrearAnimacion(4, 60, animacionJugadorFrames);



        // ########################
        // DIBUJAR A PARTIR DE AQUI
        // ########################

        estadoJugador = RUN;
        renderizarAnimacionJugador();

        



        // Dibujar el enemigo (goblin)
        DibujarEnemigo(renderer, goblin);

        // Dibujar el personaje
        SDL_RenderCopy(renderer, jugadorTextura, NULL, &jugadorPosicion);


        // Decrementar los segundos
        seconds--;

        // Si los segundos llegan a cero, decrementar los minutos y resetear los segundos a 59
        if (seconds < 0) 
        {
            seconds = 59;
            minutes--;
        }

        // Actualizar el texto del tiempo
        sprintf(tiempo, "%02d:%02d", minutes, seconds);

        // Renderizar el tiempo
        tiempo_tex = render_text(renderer, tiempo, font, red, &tiempo_rect);
        
        if (tiempo_tex)
            SDL_RenderCopy(renderer, tiempo_tex, NULL, &tiempo_rect);


        // Dibujar el texto en la parte inferior de la pantalla
        SDL_RenderCopy(renderer, helloworld_tex, NULL, &helloworld_rect);

        // Actualizamos el mundo fisico
    	//world.Step(1.0f / 60.0f, 6, 2);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        */
    } // FIN BUCLE MAIN

    // Liberamos todo lo necesario
    mundo1.Destruir();
	interfaz->Destruir();
    IMG_Quit();
    SDL_Quit();
    romfsExit();
    return 0;

    /**
     
    
    SDL_DestroyTexture(jugadorTextura);
    SDL_DestroyTexture(helloworld_tex);
    SDL_DestroyTexture(tiempo_tex);
    SDL_DestroyTexture(goblin.texture); // Destruir la textura del goblin para liberar la memoria

    //animacionJugador.Destruir();



    // Parar sonidos y liberar data 
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music);
    for (snd = 0; snd < 4; snd++)
        if (sound[snd])
            Mix_FreeChunk(sound[snd]);

    // CERRAR EL Buffer de Fondo
    framebufferClose(&fb);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    romfsExit();
    return 0;
    */
}

