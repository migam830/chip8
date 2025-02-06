#ifndef UTILS_H
#define UTILS_H
#include <SDL3/SDL.h>

int loadFile(char *fileName, unsigned char *memory);
void SDLRender(SDL_Renderer *renderer, int *pixels);
void updateKeyboard(int *keyboard, int value, SDL_Scancode scanCode);

#endif