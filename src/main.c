#include <SDL3/SDL.h>
#include <time.h>
#include <stdio.h>
#include "utils.h"

int main(int argc, char *argv[]) {
    // Memory and stack (addresses are integers but store hex values)
    unsigned char memory[4096] = { 0 };
    unsigned short int stack[16] = { 0x0000 };

    if (loadFile(argv[1], memory) != 0) {
        printf("Error loading file\n");
        return 1;
    }

    // Registers
    unsigned short int programCounter = 0x200;
    unsigned short int indexRegister = 0x0000;
    unsigned char generalRegisters[16] = { 0x00 };


    // Timers
    unsigned char delayTimer = 0xFF;
    unsigned char soundTimer = 0xFF;

    // Keyboard keys (index converted to hex is number / letter on key)
    int keyboard[16] = { 0 };

    // SDL graphics
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    // Pixels on screen
    int pixels[2048] = { 0 };

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
            
            // Update keyboard
            if (event.type == SDL_EVENT_KEY_DOWN) {
                updateKeyboard(keyboard, 1, event.key.scancode);
            }
            if (event.type == SDL_EVENT_KEY_UP) {
                updateKeyboard(keyboard, 0, event.key.scancode);
            }
        }

        SDL_Delay(10);
        SDLRender(renderer, pixels);
        int i;
        for (i = 0; i < 16; i++) {
            if (keyboard[i] == 1) {
                printf("%x pressed\n", i);
            }
        }


        // Fetch-decode-execute cycle

        // Fetch
        unsigned short int instruction = (memory[programCounter] << 8) | memory[programCounter+1];
        programCounter += 2;

        // Decode + execute
        
        // Meanings of different nibbles
        unsigned char X = (instruction & 0x0F00) >> 8;
        unsigned char Y = (instruction & 0x00F0) >> 4;
        unsigned char N = instruction & 0x000F;
        unsigned char NN = instruction & 0x00FF;
        unsigned short int NNN = instruction & 0x0FFF;

        // Coordinates for draw instruction
        unsigned char xCoord;
        unsigned char yCoord;
        // Execute instruction based on value of first nibble
        switch ((instruction & 0xF000) >> 12) {
            case 0x0:
                if (instruction == 0x00E0) {
                    // Clear screen
                    int i;
                    for (i = 0; i < 2048; i++) {
                        pixels[i] = 0;
                    }
                }
                else if (instruction == 0x00EE) {
                    programCounter = popStack(stack, 16);
                }
                break;
            case 0x1:
                programCounter = NNN;
                break;
            case 0x2:
                pushStack(programCounter, stack, 16);
                programCounter = NNN;
                break;
            case 0x3:
                if (generalRegisters[X] == NN) {
                    programCounter += 2;
                }
                break;
            case 0x4:
                if (generalRegisters[X] != NN) {
                    programCounter += 2;
                }
                break;
            case 0x5:
                // Instruction should end in 0
                if ((generalRegisters[X] == generalRegisters[Y]) && (N == 0x0)) {
                    programCounter += 2;
                }
                break;
            case 0x6:
                generalRegisters[X] = NN;
                break;
            case 0x7:
                generalRegisters[X] += NN;
                break;
            case 0x8:
                switch (N) {
                    case 0x0:
                        generalRegisters[X] = generalRegisters[Y];
                        break;
                    case 0x1:
                        generalRegisters[X] = generalRegisters[X] | generalRegisters[Y];
                        break;
                    case 0x2:
                        generalRegisters[X] = generalRegisters[X] & generalRegisters[Y];
                        break;
                    case 0x3:
                        generalRegisters[X] = generalRegisters[X] ^ generalRegisters[Y];
                        break;
                    case 0x4:
                        if (generalRegisters[X] + generalRegisters[Y] > 255) {
                            generalRegisters[15] = 0x1;
                        }
                        else {
                            generalRegisters[15] = 0x0;
                        }
                        generalRegisters[X] = generalRegisters[X] + generalRegisters[Y];
                        break;
                    case 0x5:
                        if (generalRegisters[X] > generalRegisters[Y]) {
                            generalRegisters[15] = 1;
                        }
                        else {
                            generalRegisters[15] = 0;
                        }
                            
                        generalRegisters[X] = generalRegisters[X] - generalRegisters[Y];
                        break;
                    case 0x7:
                        if (generalRegisters[Y] > generalRegisters[X]) {
                            generalRegisters[15] = 1;
                        }
                        else {
                            generalRegisters[15] = 0;
                        }
                            
                        generalRegisters[X] = generalRegisters[Y] - generalRegisters[X];
                        break;
                }
                break;
            case 0x9:
                if ((generalRegisters[X] != generalRegisters[Y]) && (N == 0x0)) {
                    programCounter += 2;
                }
                break;
            case 0xA:
                indexRegister = NNN;
                break;
            case 0xD:
                xCoord = generalRegisters[X] % 64;
                yCoord = generalRegisters[Y] % 32;
                generalRegisters[15] = 0x00;
                int i;
                for (i = 0; i < N; i++) {
                    unsigned char byte = memory[indexRegister+i];
                    int j;
                    for (j = 0; j < 8; j++) {
                        unsigned char memoryPixel = byte >> 7;
                        int screenPosition = yCoord + (32 * xCoord);
                        if (memoryPixel == 0x01 && pixels[screenPosition] == 1) {
                            pixels[screenPosition] = 0;
                            generalRegisters[15] = 1;
                        }
                        else if (memoryPixel == 0x01 && pixels[screenPosition] == 0) {
                            pixels[screenPosition] = 1;
                        }
                        byte <<= 1;
                        xCoord++;
                        if (xCoord >= 64) {
                            break;
                        }
                    }
                    xCoord = generalRegisters[X] % 64;
                    yCoord++;
                    if (yCoord >= 32) {
                        break;
                    }
                }
                break;
            default:
                printf("Unknown instruction\n");
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}