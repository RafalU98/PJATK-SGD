#include "livingEntity.h"

void LivingEntity::updateHitBox() {
	//asume damage is 0
	damage = 0;
												  //looks trough a group of the same types here "hitbox"
	GroupBox* hitBoxes = (GroupBox*)GroupBuilder::findGroupByName("hitbox", currentFrame->frameData);
	if (hitBoxes != NULL && hitBoxes->numberOfDataInGroup() > 0) {
		//update hitbox
		SDL_Rect hb = hitBoxes->data.front(); //gives firste member of the list
		hitBox.x = x - currentFrame->offSet.x + hb.x;
		hitBox.y = y - currentFrame->offSet.y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		//update damage
		GroupNumber* damages = (GroupNumber*)GroupBuilder::findGroupByName("damage", currentFrame->frameData);
		if (damages != NULL && damages->numberOfDataInGroup() > 0) {
			damage = damages->data.front();
		}

	}

}

void LivingEntity::updateInvincibleTimer() {
	if (invicibleTimer > 0) {
		invicibleTimer -= TimeController::timeController.dT;
	}
}

void LivingEntity::draw() {
	if (currentFrame != NULL && active) {
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	//draw collisionBos ( for debbuging )
	if (solid && Globals::debugging) {
		//sets the current drawing color 
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}