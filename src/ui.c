#include "ui.h"
#include <SDL3/SDL.h>

void draw_text(SDL_Renderer *renderer, const char *text, int x, int y, int size) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Placeholder - you'll add actual text rendering later
}

void draw_button(SDL_Renderer *renderer, Button *button) {
    // Draw button background
    if (button->is_hovered) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);  // Red when hovered
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 150, 255);  // Blue normally
    }
    SDL_RenderFillRect(renderer, &button->rect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &button->rect);
    
    // Draw button text (centered)
    int text_x = button->rect.x + button->rect.w / 2;
    int text_y = button->rect.y + button->rect.h / 2;
    draw_text(renderer, button->text, text_x, text_y, 20);
}

int button_is_clicked(Button *button, int mouse_x, int mouse_y) {
    return (mouse_x >= button->rect.x && mouse_x <= button->rect.x + button->rect.w &&
            mouse_y >= button->rect.y && mouse_y <= button->rect.y + button->rect.h);
}

void button_update_hover(Button *button, int mouse_x, int mouse_y) {
    button->is_hovered = button_is_clicked(button, mouse_x, mouse_y);
}
