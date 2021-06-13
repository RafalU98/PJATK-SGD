#ifndef LIVINGENTITY
#define LIVINGENTITY

#include "entity.h"

class LivingEntity : public Entity{
public:
	int hp, hpMax;
	int damage = 0;
	SDL_Rect hitBox; //similar to collisionBox, but describes damaging area

	float invincibleTimer = 0; // if > 0, then cant be hurt

	virtual void updateHitBox();
	virtual void updateDamages() = 0; //how we get damaged by other things
	virtual void die() = 0; 
	virtual void updateInvincibleTimer();

	void draw(); //overriding entities draw
};

#endif