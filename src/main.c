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

void updateKeyboard(int *keyboard, int value, SDL_Scancode scanCode) {
    switch (scanCode) {
        case SDL_SCANCODE_1:
            keyboard[1] = value;
            break;
        case SDL_SCANCODE_2:
            keyboard[2] = value;
            break;
        case SDL_SCANCODE_3:
            keyboard[3] = value;
            break;
        case SDL_SCANCODE_4:
            keyboard[12] = value; // C
            break;
        case SDL_SCANCODE_Q:
            keyboard[4] = value;
            break;
        case SDL_SCANCODE_W:
            keyboard[5] = value;
            break;
        case SDL_SCANCODE_E:
            keyboard[6] = value;
            break;
        case SDL_SCANCODE_R:
            keyboard[13] = value; // D
            break;
        case SDL_SCANCODE_A:
            keyboard[7] = value;
            break;
        case SDL_SCANCODE_S:
            keyboard[8] = value;
            break;
        case SDL_SCANCODE_D:
            keyboard[9] = value;
            break;
        case SDL_SCANCODE_F:
            keyboard[14] = value; // E
            break;
        case SDL_SCANCODE_Z:
            keyboard[10] = value; // A
            break;
        case SDL_SCANCODE_X:
            keyboard[0] = value;
            break;
        case SDL_SCANCODE_C:
            keyboard[11] = value; // B
            break;
        case SDL_SCANCODE_V:
            keyboard[15] = value; // F
            break;
    }
}

int main() {
    // Memory and stack
    unsigned char memory[4096] = { 0 };
    unsigned char stack[16] = { 0 };


    // Timers
    unsigned char delayTimer = 0xFF;
    unsigned char soundTimer = 0xFF;

    // Keyboard keys (index converted to hex is number / letter on key)
    int keyboard[16] = { 0 };

    // SDL graphics
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("CHIP-8 emulator", 640, 320, SDL_WINDOW_RESIZABLE, &window, &renderer);


    clock_t clock1 = clock();

    int running = 1;
    // SDL event loop and fetch-decode-execute loop
    while (running != 0) {
        clock_t timeElapsed = (clock() - clock1) * 60 / CLOCKS_PER_SEC;
        // Decrement timers if necessary
        // Implement timer functionality later

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
            // Keyboard keys (add more when needed)
            if (event.type == SDL_EVENT_KEY_DOWN) {
                updateKeyboard(keyboard, 1, event.key.scancode);
            }
            if (event.type == SDL_EVENT_KEY_UP) {
                updateKeyboard(keyboard, 0, event.key.scancode);
            }
        }

        SDL_Delay(50);
        SDLRender(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}