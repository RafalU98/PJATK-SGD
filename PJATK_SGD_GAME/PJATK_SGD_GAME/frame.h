#ifndef FRAME
#define FRAME

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "globals.h"
#include "groupBuilder.h"

class Frame {
public:
	int frameNumber; //frame index
	SDL_Rect clip; //region on the spriteSheet where this frame is located
	float duration; //how long a frame should run for
	SDL_Point offSet; //help aling frame in animation

	list<Group*> frameData; //holds groups of addition data for frame e.g. strings numbers and position

	void Draw(SDL_Texture* spriteSheet, float x, float y);

	void loadFrame(ifstream& file, list<DataGroupType>& groupsTypes);
};



#endif
