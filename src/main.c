#include <SDL3/SDL.h>
#include "game.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow(
        "Project 夜空(Yozora)",
        1024,
        768,
        0
    );
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);  // Dark background
    
    Game *game = game_init(window, renderer);
    
    // Game loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
            game_handle_input(game, &event);
        }
        
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);  // Dark background
        SDL_RenderClear(renderer);  // CLEAR FIRST
        
        game_update(game);
        game_render(game);
        
        SDL_RenderPresent(renderer);  // PRESENT AFTER RENDERING
        
        SDL_Delay(16);
    }
    
    game_cleanup(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
