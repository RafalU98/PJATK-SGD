#ifndef CLEANUP_H
#define CLEANUP_H

#include <utility>
#include <SDL.h>

void cleanup(SDL_Window* window);

void cleanup(SDL_Renderer* renderer);

void cleanup(SDL_Texture* textutre);

void cleanup(SDL_Surface* surface);

#endif