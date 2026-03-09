#include "game.h"
#include "ui.h"
#include <stdlib.h>
#include <string.h>

Game* game_init(SDL_Window *window, SDL_Renderer *renderer) {
    Game *game = (Game*)malloc(sizeof(Game));
    game->window = window;
    game->renderer = renderer;
    game->state = GAME_STATE_MENU;
    game->current_choice = 1;
    game->mouse_x = 0;
    game->mouse_y = 0;
    
    // Initialize buttons
    game->buttons[0] = (Button){
        .rect = {400, 300, 224, 60},
        .text = "New Game",
        .is_hovered = 0
    };
    game->buttons[1] = (Button){
        .rect = {400, 400, 224, 60},
        .text = "Load Game",
        .is_hovered = 0
    };
    game->buttons[2] = (Button){
        .rect = {400, 500, 224, 60},
        .text = "Quit",
        .is_hovered = 0
    };
    
    return game;
}

void game_handle_input(Game *game, SDL_Event *event) {
    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        game->mouse_x = event->motion.x;
        game->mouse_y = event->motion.y;
        
        // Update hover state for all buttons
        for (int i = 0; i < 3; i++) {
            button_update_hover(&game->buttons[i], game->mouse_x, game->mouse_y);
        }
    }
    
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        // Check if any button was clicked
        for (int i = 0; i < 3; i++) {
            if (button_is_clicked(&game->buttons[i], game->mouse_x, game->mouse_y)) {
                game_process_choice(game, i + 1);
            }
        }
    }
    
    // Also keep keyboard support
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_UP:
                if (game->current_choice > 1) game->current_choice--;
                break;
            case SDLK_DOWN:
                if (game->current_choice < 3) game->current_choice++;
                break;
            case SDLK_RETURN:
                game_process_choice(game, game->current_choice);
                break;
            default:
                break;
        }
    }
}

void game_process_choice(Game *game, int choice) {
    if (game->state == GAME_STATE_MENU) {
        if (choice == 1) {
            game->state = GAME_STATE_PLAYING;
        } else if (choice == 3) {
            game->state = GAME_STATE_QUIT;
        }
    }
}

void game_update(Game *game) {
    // Update game logic here
}

void game_render(Game *game) {
    if (game->state == GAME_STATE_MENU) {
        render_menu(game);
    } else if (game->state == GAME_STATE_PLAYING) {
        render_game_scene(game);
    }
}

void render_menu(Game *game) {
    // Draw title
    draw_text(game->renderer, "PAPER LILY", 512, 100, 48);
    draw_text(game->renderer, "A Horror Story", 512, 150, 24);
    
    // Draw all buttons
    for (int i = 0; i < 3; i++) {
        draw_button(game->renderer, &game->buttons[i]);
    }
}

void render_game_scene(Game *game) {
    draw_text(game->renderer, "Welcome to Paper Lily...", 512, 300, 24);
}

void game_cleanup(Game *game) {
    free(game);
}
