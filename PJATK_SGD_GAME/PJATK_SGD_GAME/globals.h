#ifndef GLOBALS
#define GLOBALS


#include <string>
#include <iostream>
#include <SDL.h>

class Globals {
public:
	//math helpers
	static const float PI;

	//useful for developing phases
	static bool debugging;

	//sdl related stuff
	static int ScreenWidth, ScreenHeight, ScreenScale;
	static SDL_Renderer* renderer;
};


#endif
