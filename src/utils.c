#include <SDL3/SDL.h>
#include <stdio.h>
#include "utils.h"

int loadFile(char *fileName, unsigned char *memory) {
    // Load program into memory
    FILE *filePtr = fopen(fileName, "r");

    if (filePtr == NULL) {
        return 1;
    }

    // Get file size
    fseek(filePtr, 0L, SEEK_END);
    long int fileSize = ftell(filePtr);
    fseek(filePtr, 0L, SEEK_SET);

    unsigned char fileData[fileSize];
    int memLocation = 512;

    fread(fileData, 1, 132, filePtr);
    fgetc(filePtr);

    fclose(filePtr);
    
    int i;
    for (i = 0; i < fileSize; i++) {
        memory[i+512] = fileData[i];
    }
    return 0;
}

void SDLRender(SDL_Renderer *renderer, int *pixels) {
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
            if (pixels[((int)rect.y / 10) + (32 * ((int)rect.x) / 10)] == 1)
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

int pushStack(unsigned short int value, unsigned short int *stack, int stackSize) {
    int i;
    for (i = stackSize - 1; i >= 0; i--) {
        if (stack[i] == 0x0000) {
            stack[i] = value;
            return 0;
        }
    }
    return 1;
}

unsigned short int popStack(unsigned short int *stack, int stackSize) {
    int i;
    for (i = 0; i < stackSize; i++) {
        if (stack[i] != 0x0000) {
            unsigned short int temp = stack[i];
            stack[i] = 0x0000;
            return temp;
        }
    }
    return 0x0000;
}