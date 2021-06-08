#ifndef DRAWINGFUNCS_H
#define DRAWINGFUNCS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"

using namespace std;

//Loads image into texture from renderer
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

//loads image to surface (if we want to swap colors of something)
SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren);

//copy color pallate to another
void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette);

//convert surfaece to texture and delete surface if we want to
SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface = false);

//draw texture at position rect (resize and fit )
//helps with animations to draw per frame
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr);

//draw texture without overlapping (draw with desired size)
//helps with animations to draw per frame
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr);

//take text and make a texture of it
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer);

//render text and make texture 
SDL_Texture* renderText(const std::string &message, TTF_Font*font,
	SDL_Color color, SDL_Renderer *renderer);

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);

#endif