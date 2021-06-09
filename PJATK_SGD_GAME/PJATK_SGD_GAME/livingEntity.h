#ifndef LIVINGENTITY
#define LIVINGENTITY

#include "entity.h"

class LivingEntity : public Entity {
public:
	int hp, hpMax;
	int damage = 0;
	SDL_Rect hitBox; // similar to collision box however describes damaging area 

	float invicibleTimer = 0; //if > 0, cant be hurt ( exclude bug where you can multipleHit per amountOfTime

	virtual void updateHitBox();
	virtual void updateDamage() = 0; // how we get damaged by other things ( up to each class to define )
	virtual void die() = 0; //abstract yp to other class to decide how they die
	virtual void updateInvincibleTimer();

	void draw(); //override entities draw

};

#endif