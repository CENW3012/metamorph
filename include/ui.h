#ifndef UI_H
#define UI_H

#include <SDL3/SDL.h>

typedef struct {
    SDL_FRect rect;
    const char *text;
    int is_hovered;
} Button;

void draw_text(SDL_Renderer *renderer, const char *text, int x, int y, int size);
void draw_text_highlighted(SDL_Renderer *renderer, const char *text, int x, int y, int size);
void draw_dialogue_box(SDL_Renderer *renderer, const char *character, const char *dialogue);
void draw_button(SDL_Renderer *renderer, Button *button);
int button_is_clicked(Button *button, int mouse_x, int mouse_y);
void button_update_hover(Button *button, int mouse_x, int mouse_y);

#endif
