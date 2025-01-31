#include <SDL3/SDL.h>

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("CHIP-8 emulator", 500, 500, SDL_WINDOW_RESIZABLE, &window, &renderer);

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xF0, 0x00);
        // Draw pixels
        SDL_FRect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = 100;
        rect.h = 100;
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}