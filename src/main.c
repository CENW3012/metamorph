#include <SDL3/SDL.h>
#ifdef HAVE_SDL3_IMAGE
#include <SDL3_image/SDL_image.h>
#endif
#include "game.h"

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    /* Change CWD to the executable's directory so that relative paths
       like "assets/..." and "maps/..." resolve correctly regardless of
       how or from where the binary is launched. */
    {
        char *base = SDL_GetBasePath();
        if (base) {
            SDL_SetCurrentDirectory(base);
            SDL_free(base);
        }
    }

#ifdef HAVE_SDL3_IMAGE
    /* SDL3_image < 3.4 requires explicit initialisation to activate format
       decoders (e.g. PNG).  3.4 removed IMG_Init/IMG_Quit entirely, so the
       block below is compiled out when building against those headers. */
#if SDL_IMAGE_MAJOR_VERSION == 3 && SDL_IMAGE_MINOR_VERSION < 4
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_WEBP) & IMG_INIT_PNG))
        SDL_Log("IMG_Init: PNG support unavailable: %s", SDL_GetError());
#endif
#endif

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

    /* Enable alpha blending globally. */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Game *game = game_init(window, renderer);
    if (!game) {
        SDL_Log("game_init failed");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* ── Main loop ─────────────────────────────────────────────────────── */
    while (game->running && game->state != GAME_STATE_QUIT) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                game->running = 0;
            game_handle_event(game, &event);
        }

        /* Clear */
        SDL_SetRenderDrawColor(renderer, 8, 6, 12, 255);
        SDL_RenderClear(renderer);

        /* Update + render */
        game_update(game);
        game_render(game);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); /* ~60 FPS cap */
    }

    game_cleanup(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
#ifdef HAVE_SDL3_IMAGE
#if SDL_IMAGE_MAJOR_VERSION == 3 && SDL_IMAGE_MINOR_VERSION < 4
    IMG_Quit();
#endif
#endif
    SDL_Quit();
    return 0;
}

