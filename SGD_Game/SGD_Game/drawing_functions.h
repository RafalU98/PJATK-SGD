#ifndef DRAWINGFUNCS_H
#define DRAWINGFUNCS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"

//Loads an image into a texture on the rendering device
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

//Loads an image up as a surface. Generally we want to do this if we want to palette swap
SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren);

//Copies the palette of 1 surface to another
void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette);

//Converts a surface to a texture and optionally deletes the surface
SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface = false);

//Draw an SDL_Texture to an SDL_Renderer at some destination rect and scale it to the desired "window size"
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);

//Draw an SDL_Texture to an SDL_Renderer at position x, y
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,SDL_Rect *clip = nullptr);

//render the message we want to desplay
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);

//render the message we want to desplay
SDL_Texture* renderText(const std::string &message, TTF_Font*font, SDL_Color color, SDL_Renderer *renderer);

#endif