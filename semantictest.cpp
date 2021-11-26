#include <stdio.h>
#include "semantic.h"

using namespace std;

extern "C" {

#include <SDL.h>

}

extern SDL_Window* sdl_window;
extern SDL_Renderer* renderer;

#define POINT_COUNT 5
#define T_POINT_COUNT 4
// points to draw
static SDL_Point points[POINT_COUNT] = {
    {50, 50},
    {60, 50},
    {60, 60},
    {50, 60},
    {50, 50} };

static SDL_Point triangle_points[T_POINT_COUNT] = {
    {150, 150},
    {200, 150},
    {175, 175},
    {150, 150} };

#undef main
int main() {
    int quit = 1;
    SDL_Event event;
    // 0 init sdl
    SDL_Init(SDL_INIT_VIDEO);
    // 1 create window
    sdl_window = SDL_CreateWindow("triangle",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600,
        600,
        SDL_WINDOW_SHOWN);
    // 2 create render
    renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_SOFTWARE);

    //3 set renderer color (set background color  Blue)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    //4 clear color
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 128, 0, SDL_ALPHA_OPAQUE);
    
    for (int i = 0; i < 100; i++) {
        DrawPoint(i+100, i+100);
    }

    // show window
    SDL_RenderPresent(renderer);


    do
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = 0;
            break;
        default:
            SDL_Log("event type is %d \n", event.type);
        }
    } while (quit);

__FAIL:
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (sdl_window)
    {
        SDL_DestroyWindow(sdl_window);
    }

    SDL_Quit();
	return 0;
}