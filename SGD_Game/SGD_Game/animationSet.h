#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "animation.h"

class AnimationSet{
public:
	std::string imageName;
	SDL_Texture* spriteSheet; //holds the spritesheet with the animations
	SDL_Texture* whiteSpriteSheet = NULL; //white texture for damage
	std::list<Animation> animations;

	~AnimationSet(); //deconstructor to clean up all the stuff 

	Animation* getAnimation(std::string name);

	void loadAnimationSet(std::string fileName, std::list<DataGroupType> &groupTypes, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
};

#endif