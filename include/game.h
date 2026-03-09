#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include "ui.h"

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_DIALOGUE,
    GAME_STATE_QUIT
} GameState;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    GameState state;
    int current_choice;
    Button buttons[3];  // Add buttons array
    int mouse_x;
    int mouse_y;
} Game;

Game* game_init(SDL_Window *window, SDL_Renderer *renderer);
void game_handle_input(Game *game, SDL_Event *event);
void game_update(Game *game);
void game_render(Game *game);
void game_cleanup(Game *game);
void game_process_choice(Game *game, int choice);
void render_menu(Game *game);
void render_game_scene(Game *game);

#endif
