// function_printer.cpp: 定义应用程序的入口点。
//
#include <cstdio>
#include <cstring>
#include "function_printer.h"

extern "C" {

#include <SDL.h>

}

#define __BASE_FILE__ "function_printer.cpp"

using namespace std;

extern SDL_Window* sdl_window;
extern SDL_Renderer* renderer;

char* filename;
unsigned int WINDOW_WIDTH = DEFAULT_WINDOW_W;
unsigned int WINDOW_HIGH = DEFAULT_WINDOW_H;

void help() {
    puts("Usage: function_printer [-h] [-W Width] [-H High] <FILE>");
}

#undef main
int main(int argc, char *argv[])
{
    int quit = 1;
    SDL_Event event;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h")) {
            help();
            exit(0);
        }
        if (!strcmp(argv[i], "-W") && i + 1 < argc) {
            WINDOW_WIDTH = atoi(argv[i + 1]) > 64u ? atoi(argv[i + 1]) : DEFAULT_WINDOW_W;
            i++;
            continue;
        }
        if (!strcmp(argv[i], "-H") && i + 1 < argc) {
            WINDOW_HIGH = atoi(argv[i + 1]) > 64u ? atoi(argv[i + 1]) : DEFAULT_WINDOW_H;
            i++;
            continue;
        }
        filename = filename? filename:argv[i];
    }

    if (!filename) {
        fprintf(stderr, "[%s, %d, %s()] Please specify a source file!\n", __BASE_FILE__, __LINE__, __func__);
        exit(-1);
    }

    SDL_setenv(SDL_HINT_RENDER_SCALE_QUALITY, "linear", 0);

    // init sdl
    SDL_Init(SDL_INIT_VIDEO);
    // create window
    sdl_window = SDL_CreateWindow("Funtion Printer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HIGH,
        SDL_WINDOW_SHOWN);
    // create render
    renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_SOFTWARE);

    // set renderer color (set background color  Blue)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // clear color
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    Parser(filename);

    do
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = 0;
            break;
        default:
            //SDL_Log("event type is %d \n", event.type);
            break;
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
