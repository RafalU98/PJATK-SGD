#include "drawing_functions.h"


//Loads image into texture from renderer
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		cout<< "LoadTexture error"<<endl;
	}
	return texture;
}

//loads image to surface (if we want to swap colors of something)
SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren){
	SDL_Surface *surface = IMG_Load(file.c_str());
	if (surface == nullptr)
	{
		cout << "LoadSurface error" << endl;
	}
	return surface;
}

//copy color pallate to another
void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette)
{
	SDL_SetPaletteColors(surface->format->palette, palette->format->palette->colors, 0, palette->format->palette->ncolors);
}

//convert surfaece to texture and delete surface if we want to
SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface)
{
	SDL_Texture*texture = SDL_CreateTextureFromSurface(ren, surface);
	if (cleanSurface)
		cleanup(surface);

	return texture;
}

//draw texture at position rect (resize and fit )
//helps with animations to draw per frame
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

//draw texture without overlapping (draw with desired size)
//helps with animations to draw per frame
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip)
{
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


//take text and make a texture of it
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	//TODO: store fonts externally from this function call. Bad performance warning!!!
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		cout<<"TTF_OpenFont error"<<endl;
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		cout << "TTF_RenderText error" << endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		cout<< "CreateTexture"<<endl;
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}


//render text and make texture
SDL_Texture* renderText(const std::string &message, TTF_Font *font,
	SDL_Color color, SDL_Renderer *renderer)
{
	if (font == nullptr){
		cout << "TTF_OpenFont" << endl;
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		cout<<"TTF_RenderText"<<endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		cout<<"CreateTexture"<<endl;
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	//	TTF_CloseFont(font);
	return texture;
}

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer) {
	SDL_Surface* saveSurface = NULL;
	SDL_Surface* infoSurface = NULL;
	infoSurface = SDL_GetWindowSurface(SDLWindow);
	if (infoSurface == NULL) {
		std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
	}
	else {
		unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
		if (pixels == 0) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
			return false;
		}
		else {
			if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
				pixels = NULL;
				return false;
			}
			else {
				saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
				if (saveSurface == NULL) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
					return false;
				}
				SDL_SaveBMP(saveSurface, filepath.c_str());
				SDL_FreeSurface(saveSurface);
				saveSurface = NULL;
			}
			delete[] pixels;
		}
		SDL_FreeSurface(infoSurface);
		infoSurface = NULL;
	}
	return true;
}
