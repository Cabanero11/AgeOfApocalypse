#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <switch.h>

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

bool move_up = false;
bool move_down = false;
bool move_left = false;
bool move_right = false;

float velocidadMovimiento = 5.0f;

SDL_Texture * render_text(SDL_Renderer *renderer, const char* text, TTF_Font *font, SDL_Color color, SDL_Rect *rect) 
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);

    return texture;
}

int main(int argc, char** argv) {
    romfsInit();
    chdir("romfs:/");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

    SDL_Window* window = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    TTF_Font* font = TTF_OpenFont("data/LeroyLetteringLightBeta01.ttf", 36);
    SDL_Rect helloworld_rect = { 0, SCREEN_H - 36, 0, 0 };
    SDL_Color white = { 255, 255, 255, 0 };
    SDL_Texture* helloworld_tex = render_text(renderer, "Bye bye!", font, white, &helloworld_rect);
    TTF_CloseFont(font);

    SDL_Surface* pumpkin_surface = IMG_Load("data/pumpkin_dude/pumpkin_dude_idle_anim_f0.png");
    SDL_Texture* pumpkin_tex = SDL_CreateTextureFromSurface(renderer, pumpkin_surface);
    SDL_Rect pumpkin_pos = { SCREEN_W / 2 - pumpkin_surface->w / 2, SCREEN_H / 2 - pumpkin_surface->h / 2, pumpkin_surface->w, pumpkin_surface->h };
    SDL_FreeSurface(pumpkin_surface);

    SDL_Rect sdl_pos = { 0, 0, 0, 0 };


    // MUSICA
    Mix_Music *music = NULL;
    Mix_Chunk *sound[4] = { NULL }; 

    music = Mix_LoadMUS("data/mewing.ogg");
    sound[0] = Mix_LoadWAV("data/pop1.wav");
    sound[1] = Mix_LoadWAV("data/pop2.wav");
    sound[2] = Mix_LoadWAV("data/pop3.wav");
    sound[3] = Mix_LoadWAV("data/pop4.wav");
    if (music)
        Mix_PlayMusic(music, -1);



    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);



    int exit_requested = 0;
    while (!exit_requested && appletMainLoop()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit_requested = 1;

            // mario

            // Manejar el movimiento continuo mientras se mantiene pulsado el botón
            if (event.type == SDL_JOYBUTTONDOWN) {
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
            if (event.type == SDL_JOYBUTTONUP) {
                if (event.jbutton.button == JOY_UP)
                    move_up = false;
                else if (event.jbutton.button == JOY_DOWN)
                    move_down = false;
                else if (event.jbutton.button == JOY_LEFT)
                    move_left = false;
                else if (event.jbutton.button == JOY_RIGHT)
                    move_right = false;
            }
        }

        // Actualizar la posición del personaje en función del estado de los botones
        if (move_up)
            pumpkin_pos.y -= velocidadMovimiento;
        if (move_down)
            pumpkin_pos.y += velocidadMovimiento;
        if (move_left)
            pumpkin_pos.x -= velocidadMovimiento;
        if (move_right)
            pumpkin_pos.x += velocidadMovimiento;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dibujar el personaje
        SDL_RenderCopy(renderer, pumpkin_tex, NULL, &pumpkin_pos);

        // Dibujar el texto en la parte inferior de la pantalla
        SDL_RenderCopy(renderer, helloworld_tex, NULL, &helloworld_rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(pumpkin_tex);
    SDL_DestroyTexture(helloworld_tex);

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
