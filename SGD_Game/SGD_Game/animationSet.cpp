#include "animationSet.h"

AnimationSet::~AnimationSet(){
	cleanup(spriteSheet);
	if (whiteSpriteSheet != NULL)
		cleanup(whiteSpriteSheet);
}

Animation* AnimationSet::getAnimation(std::string name){
	for (std::list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++){
		Animation* anim = &(*animation);

		//if we find a match on name, return that animation from the list
		if (name == anim->name) {
			return anim;
		}
	}
	return NULL;
}

//loads the fdset file, take a list of what typt of data to expect
//get pixels from the image if we are working on 8bit image
//get "colorKeying' to make the backgrouncouler transparent
void AnimationSet::loadAnimationSet(std::string fileName, std::list<DataGroupType> &groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture){

	std::ifstream file;
	std::string resPath = getResourcePath();
	file.open(resPath + fileName);
	if (file.good())
	{
		getline(file, imageName);
		if (setColourKey) {
			SDL_Surface* spriteSurface = loadSurface(resPath + imageName, Globals::renderer);

			//for transparency, we will grab the [transparentPixelIndex] from the surface we just made
			SDL_Color* transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
			SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

			spriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);

			if (createWhiteTexture) {
				SDL_Surface* whiteSurface = loadSurface(resPath + "allwhite.png", Globals::renderer);
				surfacePaletteSwap(spriteSurface, whiteSurface);
				SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
				whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false); //create the texture whilst destroying the surface

				cleanup(whiteSurface);
			}
			else{
				whiteSpriteSheet = NULL;
			}
			cleanup(spriteSurface);
		}
		else
			spriteSheet = loadTexture(resPath + imageName, Globals::renderer);

		std::string buffer;
		std::getline(file, buffer);
		std::stringstream ss;
		buffer = Globals::clipOffDataHeader(buffer);
		ss << buffer;
		int numberOfAnimations;
		ss >> numberOfAnimations;

		for (int i = 0; i < numberOfAnimations; i++){
			Animation newAnimation;
			newAnimation.loadAnimation(file, groupTypes);
			animations.push_back(newAnimation);
		}
	}
	file.close();
}