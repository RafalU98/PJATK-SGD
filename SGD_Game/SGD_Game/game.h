#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "glob.h"
#include "grob.h"

class Game{
public:
	AnimationSet* heroAnimSet;
	AnimationSet* wallAnimSet;
	AnimationSet* globAnimSet;
	AnimationSet* grobAnimSet;

	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* overlayImage;

	SDL_Texture* scoreTexture = NULL; // for drawing text to screen

	Hero *hero;
	KeyboardInput heroInput;

	std::list<Entity*> walls;
	std::list<Entity*> enemies;

	bool splashShowing;
	float overlayTimer;

	Game();
	~Game();

	void update();
	void draw();

};


#endif