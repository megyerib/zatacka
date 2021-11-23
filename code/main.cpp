#include <SDL.h>
#include "zatacka.h"

// Ezt hívja meg az SDL időzítő.
// Berak egy user eventet az eseménysorba.
Uint32 idozit(Uint32 ms, void *param);

// Létrehoz egy ablakot azonn a kijelzőn, amelyiken a kurzor van.
SDL_Window* CreateMainWindow(char* title, bool full_screen);

int main()
{
    // SDL init
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    // Window init
    SDL_Window *window = CreateMainWindow((char*)"Zatacka", true);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }

    // Renderer init
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    // Timer
    SDL_TimerID id = SDL_AddTimer(JATEK_PERIODUS, idozit, NULL);

    // Rajzoló panelek helyének kiszámítása
    int renderer_sz, renderer_m;
    SDL_GetRendererOutputSize(renderer, &renderer_sz, &renderer_m);

    SDL_Rect jatekter_hely = {
        .x = 0,
        .y = 0,
        .w = renderer_sz - EREDMJ_SZ,
        .h = renderer_m
    };
    Jatekter jatekter(renderer, &jatekter_hely);

    SDL_Rect menu_hely = {
        .x = 16,
        .y = 16,
        .w = 457,
        .h = 321,
    };
    Menu menu(renderer, &menu_hely);

    SDL_Rect eredmenyjelzo_hely = {
        .x = renderer_sz - EREDMJ_SZ,
        .y = 0,
        .w = EREDMJ_SZ,
        .h = renderer_m
    };
    Eredmenyjelzo eredmenyjelzo(renderer, &eredmenyjelzo_hely);

    // Játék osztály indítása
    Zatacka jatek(renderer, menu, jatekter, eredmenyjelzo);

    // Event loop
    bool quit = false;

    while (!quit && !jatek.kilepes) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_KEYUP:
            jatek.FormKeyUp(event.key.keysym.sym);
            break;
            
        case SDL_KEYDOWN:
            jatek.FormKeyDown(event.key.keysym.sym);
            break;
        
        case SDL_MOUSEBUTTONDOWN:
            jatek.FormMouseDown(event.button.button, event.button.x, event.button.y);
            break;

        case SDL_MOUSEBUTTONUP:
            jatek.FormMouseUp(event.button.button, event.button.x, event.button.y);
            break;
        
        case SDL_USEREVENT:
            jatek.Timer1Timer();
            break;

        case SDL_QUIT:
            quit = true;
            break;
        
        default:
            break;
        }
    }
 
    // Takarítás, kilépés
    SDL_Quit();
 
    return 0;
}

Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

SDL_Window* CreateMainWindow(char* title, bool full_screen)
{
    int display_num = SDL_GetNumVideoDisplays();
    SDL_Rect db; // Display bounds
    int mouse_x, mouse_y;
    uint32_t window_flags = SDL_WINDOW_FULLSCREEN;

    SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
    
    for(int i = 0; i < display_num; i++) {
        SDL_GetDisplayBounds(i, &db);
        if(
            mouse_x >= db.x &&
            mouse_x < db.x + db.w &&
            mouse_y >= db.y &&
            mouse_y < db.y + db.h
        ) {
            break;
        }
    }

    if(!full_screen) {
        db.w = 1024;
        db.h = 768;
        window_flags &= ~(SDL_WINDOW_FULLSCREEN);
    }

    return SDL_CreateWindow(
        title,
        db.x, db.y, db.w, db.h,
        window_flags
    );
}
