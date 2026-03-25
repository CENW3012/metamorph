#include "ui.h"
#include "player.h"
#include "render.h"
#include <string.h>
#include <stdio.h>

/* ── Button ───────────────────────────────────────────────────────────── */

void button_update_hover(Button *button, float mouse_x, float mouse_y)
{
    if (!button) return;
    button->is_hovered = button_is_clicked(button, mouse_x, mouse_y);
}

int button_is_clicked(Button *button, float mouse_x, float mouse_y)
{
    if (!button) return 0;
    return (mouse_x >= button->rect.x &&
            mouse_x <= button->rect.x + button->rect.w &&
            mouse_y >= button->rect.y &&
            mouse_y <= button->rect.y + button->rect.h);
}

void draw_button(SDL_Renderer *renderer, const Button *button)
{
    if (!button || !renderer) return;

    int x = (int)button->rect.x;
    int y = (int)button->rect.y;
    int w = (int)button->rect.w;
    int h = (int)button->rect.h;

    /* Shadow */
    render_filled_rect(renderer, x+4, y+4, w, h, 0,0,0,120);

    /* Body */
    if (button->is_hovered)
        render_filled_rect(renderer, x, y, w, h, 80, 50, 90, 240);
    else
        render_filled_rect(renderer, x, y, w, h, 35, 25, 45, 230);

    /* Border */
    render_rect_outline(renderer, x,   y,   w,   h,   120,80,140,255);
    render_rect_outline(renderer, x+2, y+2, w-4, h-4,  60,40,80, 180);

    /* Text */
    if (button->text) {
        int tw = render_text_width(button->text, 2);
        int tx = x + (w - tw) / 2;
        int ty = y + (h - 16) / 2;
        if (button->is_hovered)
            render_text(renderer, button->text, tx, ty, 2, 255, 220, 255);
        else
            render_text(renderer, button->text, tx, ty, 2, 180, 150, 200);
    }
}
void draw_button_menu(SDL_Renderer *renderer, const Button *button)
{
    if (!button || !renderer) return;

    int x = (int)button->rect.x;
    int y = (int)button->rect.y;
    int w = (int)button->rect.w;
    int h = (int)button->rect.h;

    /* Dark overlay when hovered */
    if (button->is_hovered) {
        render_filled_rect(renderer, x, y, w, h, 0, 0, 0, 110);           /* dark transparent */
        
        /* Shadow effect - offset dark rectangle below */
        render_filled_rect(renderer, x + 4, y + h + 2, w, 8, 0, 0, 0, 80);  /* shadow */
    }

    /* Text (black on hover, light purple when not) */
    if (button->text) {
        int tw = render_text_width(button->text, 2);
        int tx = x + (w - tw) / 2;
        int ty = y + (h - 16) / 2;

        if (button->is_hovered)
            render_text(renderer, button->text, tx, ty, 2, 255, 0, 0);      /* black text on hover */
        else
            render_text(renderer, button->text, tx, ty, 2, 200, 190, 210); /* light purple */
    }
}
/* ── Slider ────────────────────────────────────────────────────────────── */

void slider_init(Slider *s, float x, float y, float w, float h,
                 float min_val, float max_val, float value)
{
    if (!s) return;
    s->x = x; s->y = y;
    s->w = w; s->h = h;
    s->min = min_val; s->max = max_val;
    s->focused = 0;
    slider_set_value(s, value);
}

void slider_set_value(Slider *s, float value)
{
    if (!s) return;
    if (value < s->min) value = s->min;
    if (value > s->max) value = s->max;
    s->value = value;
}

int slider_handle_click(Slider *s, float mx, float my)
{
    if (!s) return 0;
    if (mx >= s->x && mx <= s->x + s->w &&
        my >= s->y - 10.0f && my <= s->y + s->h + 10.0f) {
        float t = (mx - s->x) / s->w;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        s->value = s->min + t * (s->max - s->min);
        return 1;
    }
    return 0;
}

void slider_render(SDL_Renderer *r, const Slider *s, const char *label)
{
    if (!r || !s) return;

    int x = (int)s->x, y = (int)s->y;
    int w = (int)s->w, h = (int)s->h;

    /* Label (right-aligned, left of the track) */
    if (label) {
        int lw = render_text_width(label, 2);
        int lx = x - lw - 20;
        int ly = y + (h - 16) / 2;
        if (s->focused)
            render_text(r, label, lx, ly, 2, 230, 200, 255);
        else
            render_text(r, label, lx, ly, 2, 160, 140, 180);
    }

    /* Track background */
    render_filled_rect(r, x, y, w, h, 30, 22, 45, 200);
    render_rect_outline(r, x, y, w, h,
        s->focused ? 140 : 80,
        s->focused ? 100 : 55,
        s->focused ? 170 : 105,
        200);

    /* Filled portion */
    float t = (s->max > s->min) ? (s->value - s->min) / (s->max - s->min) : 0.0f;
    int filled_w = (int)(w * t);
    if (filled_w > 0) {
        render_filled_rect(r, x, y, filled_w, h,
            s->focused ? 120 : 80,
            s->focused ?  85 : 55,
            s->focused ? 155 : 110,
            220);
    }

    /* Handle */
    int handle_x = x + filled_w - 6;
    if (handle_x < x) handle_x = x;
    if (handle_x > x + w - 12) handle_x = x + w - 12;
    render_filled_rect(r, handle_x, y - 4, 12, h + 8,
        s->focused ? 220 : 160,
        s->focused ? 190 : 130,
        s->focused ? 255 : 185,
        255);

    /* Value text (right of the track) */
    char val_buf[16];
    snprintf(val_buf, sizeof(val_buf), "%d", (int)s->value);
    render_text(r, val_buf, x + w + 16, y + (h - 16) / 2, 2,
        s->focused ? 230 : 170,
        s->focused ? 200 : 150,
        s->focused ? 255 : 195);
}

/* ── HUD ───────────────────────────────────────────────────────────────── */

void ui_draw_hud(SDL_Renderer *renderer, const Player *player)
{
    if (!renderer || !player) return;

    /* Inventory count */
    char inv_text[32];
    snprintf(inv_text, sizeof(inv_text), "Items: %d/%d",
             player->inventory_count, INVENTORY_CAPACITY);
    render_text(renderer, inv_text, 14, 14, 1, 140, 120, 150);
}

/* ── Interaction prompt ────────────────────────────────────────────────── */

void ui_draw_interact_prompt(SDL_Renderer *renderer,
                             const char *label,
                             int screen_x, int screen_y)
{
    if (!renderer || !label) return;

    char buf[80];
    snprintf(buf, sizeof(buf), "[E] %s", label);
    int tw = render_text_width(buf, 1);
    int tx = screen_x - tw / 2;
    int ty = screen_y - 30;

    /* Background */
    render_filled_rect(renderer, tx - 6, ty - 4, tw + 12, 20, 20,15,30, 200);
    render_rect_outline(renderer, tx - 6, ty - 4, tw + 12, 20, 80,60,100, 220);

    render_text(renderer, buf, tx, ty, 1, 220, 200, 255);
}
