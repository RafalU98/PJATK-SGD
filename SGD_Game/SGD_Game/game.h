#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "glob.h"

class Game{
public:
	AnimationSet* heroAnimSet;
	AnimationSet* wallAnimSet;
	AnimationSet* globAnimSet;

	SDL_Texture* backgroundImage;

	Hero *hero;
	KeyboardInput heroInput;

	std::list<Entity*> walls;
	std::list<Entity*> enemies;

	Game();
	~Game();

	void update();
	void draw();

};


#endif