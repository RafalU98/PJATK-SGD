#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "animation.h"

class AnimationSet {
public:
	string imageName; //holds name of spriteSheet
	SDL_Texture* spriteSheet; //holds the spriteSheet image for all animations
	SDL_Texture* whiteSpriteSheet = NULL; //access pixeldata from the sheet to "swap" colors e.g. to show when dmg is made
	list<Animation> animations;

	~AnimationSet(); //destructor to properly clean any dynamic memory e.g. textures

	Animation* getAnimation(string name);

																			//set a "color" to transperent
																										//works for 8bit images, set the transperent color
																																	  //set white texture for example "fast movement of character" << dash like
	void loadAnimationSet(string fileName, list<DataGroupType>& groupTypes, bool setCoulourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);

};


#endif 

