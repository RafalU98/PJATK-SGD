#include "cleanup.h"

void cleanup(SDL_Window *window){
	if (!window){
		return;
	}
	SDL_DestroyWindow(window);
}
void cleanup(SDL_Renderer *renderer){
	if (!renderer){
		return;
	}
	SDL_DestroyRenderer(renderer);
}
void cleanup(SDL_Texture *texture){
	if (!texture){
		return;
	}
	SDL_DestroyTexture(texture);
}
void cleanup(SDL_Surface *surface){
	if (!surface){
		return;
	}
	SDL_FreeSurface(surface);
}
