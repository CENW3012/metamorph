#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "game.h"

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    // Debug: Print current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        SDL_Log("Current working directory: %s", cwd);
    }

    // Debug: Check if assets folder exists
    FILE *test = fopen("assets/locations.txt", "r");
    if (test) {
        SDL_Log("✓ Found assets/locations.txt");
        fclose(test);
    } else {
        SDL_Log("✗ ERROR: Cannot find assets/locations.txt in: %s", cwd);
    }

    // Debug: Check for PNG files
    test = fopen("assets/title_screen.png", "r");
    if (test) {
        SDL_Log("✓ Found assets/title_screen.png");
        fclose(test);
    } else {
        SDL_Log("✗ ERROR: Cannot find assets/title_screen.png");
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Project Yozora – A Horror Story",
        WINDOW_W, WINDOW_H, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Game *game = game_init(window, renderer);
    if (!game) {
        SDL_Log("game_init failed");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    while (game->running && game->state != GAME_STATE_QUIT) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                game->running = 0;
            game_handle_event(game, &event);
        }

        SDL_SetRenderDrawColor(renderer, 8, 6, 12, 255);
        SDL_RenderClear(renderer);

        game_update(game);
        game_render(game);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    game_cleanup(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
