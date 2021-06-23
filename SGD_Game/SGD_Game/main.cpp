#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"
#include "globals.h"
#include "game.h"

#include <cstdlib> //srand, rand
#include <ctime> //time


int main(int argc, char **agv){
	//set the random seed for the random number sequence
	srand(time(0));

	//setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "error initialising SDL" << std::endl;
		return 1;
	}
	//setup window
	SDL_Window *window = SDL_CreateWindow("GAME_SGD_BATTLE W/ SLIMES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		Globals::ScreenWidth*Globals::ScreenScale, Globals::ScreenHeight*Globals::ScreenScale
		, SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (window == nullptr){
		SDL_Quit(); 
		std::cout << "window error" << std::endl;
		return 1;
	}

	//setup renderer
	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == nullptr){
		cleanup(window);
		SDL_Quit();
		std::cout << "renderer error" << std::endl;
		return 1;
	}
	//this is the size to draw things at, before we scale it to the screen size dimensions mentioned in createWindow
	SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);

	//initialise sdl_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_Quit();
		std::cout << "sdl image did not initialise" << std::endl;
		return 1;
	}
	//initialise text to font 
	if (TTF_Init() != 0){
		SDL_Quit();
		std::cout << "sdl ttf did not initialise" << std::endl;
		return 1;
	}
	//inialise sdl mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Quit();
		std::cout << "mixer did not initalise" << std::endl;
		return 1;
	}

	Game game;
	game.update();

	cleanup(Globals::renderer);
	cleanup(window);
	//cleanup(texture);

	SDL_Quit();
	return 0;


}