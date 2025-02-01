#include <SDL3/SDL.h>
#include <time.h>
#include <stdio.h>

void SDLRender(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xF0, 0x00);
        // Draw pixels
        SDL_FRect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = 10;
        rect.h = 10;

        while (rect.x < 640) {
            while (rect.y < 320) {
                SDL_RenderFillRect(renderer, &rect);
                rect.y += 10;
            }
            rect.y = 0;
            rect.x += 10;
        }

        SDL_RenderPresent(renderer);
}

int main() {
    // Memory and stack
    unsigned char memory[4096] = { 0 };
    unsigned char stack[16] = { 0 };


    // Timers
    unsigned char delayTimer = 0xFF;
    unsigned char soundTimer = 0xFF;

    // SDL graphics
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("CHIP-8 emulator", 640, 320, SDL_WINDOW_RESIZABLE, &window, &renderer);


    clock_t clock1 = clock();
    // SDL event loop and fetch-decode-execute loop
    while (1) {
        // Decrement timers if necessary
        if (delayTimer > 0x00) {
            clock_t timeElapsed = (clock() - clock1) * 60 / CLOCKS_PER_SEC;
            int i;
            for (i = 0; i < timeElapsed; i++) {
                if (delayTimer <= 0x00) {
                    break;
                }
                delayTimer -= 0x01;
            }
            printf("%x\n", delayTimer);
        }

        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }
        // Keyboard keys (add more when needed)
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.scancode == SDL_SCANCODE_Q) {
                printf("Q pressed\n");
            }
        }

        SDLRender(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}