#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "globals.h"

#include <SDL_mixer.h>
#include <iostream>

int main(int argc, char** agv) {

	//setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "ERROR ON INITIALISATION" << std::endl;
		return 1;
	}

	//setup window
	SDL_Window* window = SDL_CreateWindow("SGD GRA v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale
		,SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (window == nullptr) {
		SDL_Quit();
		std::cout << "Error on Window" << std::endl;
		return 1;
	}

	//setup renderer
	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == nullptr) {
		cleanup(window);
		SDL_Quit();
		std::cout << "Error on Renderer" << std::endl;
		return 1;
	}

	//size to draw things before window scaling (e.g. fullscreen)
	SDL_RenderSetLogicalSize(Globals::renderer, 640, 352);

	//initilaise sdl_image to use png
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Quit();
		std::cout << "Error on SDL Image" << std::endl;
		return 1;
	}

	//initialise text to font
	if (TTF_Init() != 0) {
		SDL_Quit();
		std::cout << "Error on SDL ttf" << std::endl;
		return 1;
	}

	//initialise sdl mixer 
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Quit();
		std::cout << "Error on SDL mixer" << std::endl;
		return 1;
	}

	//load up a texture to draw on screen
	string resPath = getResourcePath();
	SDL_Texture* texture = loadTexture(resPath + "map.png", Globals::renderer);

	//run game for 5000ticks;
	while (SDL_GetTicks() < 5000) {
		//clear screen
		SDL_RenderClear(Globals::renderer);
		//Draw what we want
		renderTexture(texture, Globals::renderer, 0, 0);
		//show the image 
		SDL_RenderPresent(Globals::renderer);
	}

	cleanup(Globals::renderer);
	cleanup(window);
	cleanup(texture);

	SDL_Quit();
	return 0;
}