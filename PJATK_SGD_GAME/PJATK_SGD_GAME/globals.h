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

	//clips of header  e.g. "clip: 50 114 44 49" turns to "50 114 44 49"
	static std::string clipOffDataHeader(std::string date);

};


#endif
