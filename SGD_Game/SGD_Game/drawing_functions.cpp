#include "drawing_functions.h"


//Loads an image into a texture on the rendering device
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		std::cout<< "LoadTexture error"<< std::endl;
	}
	return texture;
}

//Loads an image up as a surface. Generally we want to do this if we want to palette swap
SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren){
	SDL_Surface *surface = IMG_Load(file.c_str());
	if (surface == nullptr){
		std::cout << "LoadSurface error" << std::endl;
	}
	return surface;
}

//Copies the palette of 1 surface to another
void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette) {
	SDL_SetPaletteColors(surface->format->palette, palette->format->palette->colors, 0, palette->format->palette->ncolors);
}

//Converts a surface to a texture and optionally deletes the surface
SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface) {
	SDL_Texture*texture = SDL_CreateTextureFromSurface(ren, surface);
	if (cleanSurface)
		cleanup(surface);

	return texture;
}


void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip) {
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer){

	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		std::cout<<"TTF_OpenFont error"<< std::endl;
		return nullptr;
	}
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		std::cout << "TTF_RenderText error" << std::endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		std::cout<< "CreateTexture"<< std::endl;
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}


SDL_Texture* renderText(const std::string &message, TTF_Font *font,
	SDL_Color color, SDL_Renderer *renderer)
{
	if (font == nullptr){
		std::cout << "TTF_OpenFont" << std::endl;
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		std::cout<<"TTF_RenderText"<< std::endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		std::cout<<"CreateTexture"<< std::endl;
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	//	TTF_CloseFont(font);
	return texture;
}

