#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <switch.h>
#include <cmath>
#include <unistd.h>


#define SCREEN_W 1280
#define SCREEN_H 720

#define JOY_A     0
#define JOY_B     1
#define JOY_X     2
#define JOY_Y     3
#define JOY_PLUS  10
#define JOY_MINUS 11
#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15

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
void inicializar_enemigo(Enemigo& enemigo, SDL_Renderer* renderer, const char* filename, int x, int y) {
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
void dibujar_enemigo(SDL_Renderer* renderer, const Enemigo& enemigo) {
    SDL_RenderCopy(renderer, enemigo.texture, NULL, &enemigo.pos);
}


void mover_enemigo_hacia_jugador(Enemigo& enemigo, const SDL_Rect* jugador, double distancia_maxima) {
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

























// MAIN
int main(int argc, char** argv) 
{

    romfsInit();
    chdir("romfs:/");

/**
	// Iniciamos lo relacionado con el mundo fisico
	b2Vec2 gravity(0.0, 0.0f);
	b2World world(gravity);

    // Codigo de Mario 3
    DetectorDeColisiones detector;
    Camara camara(0.0f, 0.0f);
    Mapa mundo1(RUTA_MAPA_MUNDO_1, &camara, &world);
    world.SetContactListener(&detector);



    Jugador jugador(&camara, &world);
    camara.AsignarJugador(&jugador);

    mundo1.Renderizar();

    */

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

    SDL_Window* window = SDL_CreateWindow("Age of Apocalypse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    TTF_Font* font = TTF_OpenFont("data/LeroyLetteringLightBeta01.ttf", 36);
    SDL_Rect helloworld_rect = { 0, SCREEN_H - 36, 0, 0 };
    //SDL_Rect helloworld_rect = { SCREEN_W / 2, 36, 0, 0 };
    SDL_Color white = { 255, 255, 255, 0 };
    SDL_Texture* helloworld_tex = render_text(renderer, "Bye bye!", font, white, &helloworld_rect);



    SDL_Surface* pumpkin_surface = IMG_Load("data/pumpkin_dude/pumpkin_dude_idle_anim_f0.png");
    // Multiplica las dimensiones originales del sprite por un factor de escala (por ejemplo, 2 para duplicar el tamaño)
    SDL_Texture* pumpkin_tex = SDL_CreateTextureFromSurface(renderer, pumpkin_surface);
    SDL_Rect pumpkin_pos = { SCREEN_W / 2 - pumpkin_surface->w, SCREEN_H / 2 - pumpkin_surface->h, pumpkin_surface->w * 2, pumpkin_surface->h * 2 };
    SDL_FreeSurface(pumpkin_surface);

    /**
    // TIEMPO
    SDL_Rect tiempo_rect = { SCREEN_W / 2, 36, 0, 0 };
    SDL_Color red = { 255, 0, 0, 0 };
    char tiempo[10]; // Variable para almacenar la cadena de tiempo formateada
    sprintf(tiempo, "%02d:%02d", minutes, seconds); // Formatear la cadena de tiempo inicial
    SDL_Texture* tiempo_tex = render_text(renderer, tiempo, font, red, &tiempo_rect);
    TTF_CloseFont(font);
    */

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

    music = Mix_LoadMUS("data/mewing.ogg");
    sound[0] = Mix_LoadWAV("data/pop1.wav");
    sound[1] = Mix_LoadWAV("data/pop2.wav");
    sound[2] = Mix_LoadWAV("data/pop3.wav");
    sound[3] = Mix_LoadWAV("data/pop4.wav");

    if (music)
        Mix_PlayMusic(music, -1);

    
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);
    

    Enemigo goblin;
    inicializar_enemigo(goblin, renderer, "data/goblin/goblin_idle_anim_f0.png", pumpkin_pos.x + 15, pumpkin_pos.y);




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
        if (move_up)
            pumpkin_pos.y -= velocidadMovimiento;
        if (move_down)
            pumpkin_pos.y += velocidadMovimiento;
        if (move_left)
            pumpkin_pos.x -= velocidadMovimiento;
        if (move_right)
            pumpkin_pos.x += velocidadMovimiento;

        // Mover el enemigo hacia el jugador (pumpkin)
        mover_enemigo_hacia_jugador(goblin, &pumpkin_pos, 5.0f);


        // LIMPIAR LA PANTALLA
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // DIBUJAR A PARTIR DE AQUI






        // Dibujar el enemigo (goblin)
        dibujar_enemigo(renderer, goblin);

        // Dibujar el personaje
        SDL_RenderCopy(renderer, pumpkin_tex, NULL, &pumpkin_pos);


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
    } // FIN BUCLE MAIN


    SDL_DestroyTexture(pumpkin_tex);
    SDL_DestroyTexture(helloworld_tex);
    SDL_DestroyTexture(tiempo_tex);
    SDL_DestroyTexture(goblin.texture); // Destruir la textura del goblin para liberar la memoria

    //mundo1.Destruir();

    Mix_FreeMusic(music);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    romfsExit();
    return 0;
}

