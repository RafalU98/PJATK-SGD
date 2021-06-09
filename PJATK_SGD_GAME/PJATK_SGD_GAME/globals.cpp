#include "globals.h"
#include <math.h>

const float Globals::PI = M_PI;

//usefull for the developing phase
bool Globals::debugging = true;

//sdl related
int Globals::ScreenWidth = 640, Globals::ScreenHeight = 352, Globals::ScreenScale = 2;
SDL_Renderer* Globals::renderer = NULL;

std::string Globals::clipOffDataHeader(std::string data) {
	int pos = data.find(":", 0); //returns where we find the : in string 

	if (pos != -1) {
		data = data.substr(pos + 1, data.length() - pos + 2);
	}
	return data;
}