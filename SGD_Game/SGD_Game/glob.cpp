#include "glob.h"

int attackDistance = 100;

const std::string Glob::GLOB_ANIM_UP = "up";
const std::string Glob::GLOB_ANIM_DOWN = "down";
const std::string Glob::GLOB_ANIM_LEFT = "left";
const std::string Glob::GLOB_ANIM_RIGHT = "right";

const std::string Glob::GLOB_ANIM_IDLE_UP = "idleUp";
const std::string Glob::GLOB_ANIM_IDLE_DOWN = "idleDown";
const std::string Glob::GLOB_ANIM_IDLE_LEFT = "idleLeft";
const std::string Glob::GLOB_ANIM_IDLE_RIGHT = "idleRight";

const std::string Glob::GLOB_ANIM_ATTACK_UP = "attackUp";
const std::string Glob::GLOB_ANIM_ATTACK_DOWN = "attackDown";
const std::string Glob::GLOB_ANIM_ATTACK_LEFT = "attackLeft";
const std::string Glob::GLOB_ANIM_ATTACK_RIGHT = "attackRight";


const std::string Glob::GLOB_ANIM_TELEGRAPH_UP = "telegraphUp";
const std::string Glob::GLOB_ANIM_TELEGRAPH_DOWN = "telegraphDown";
const std::string Glob::GLOB_ANIM_TELEGRAPH_LEFT = "telegraphLeft";
const std::string Glob::GLOB_ANIM_TELEGRAPH_RIGHT = "telegraphRight";

const std::string Glob::GLOB_ANIM_DIE = "die";


const int Glob::GLOB_STATE_IDLE = 0;
const int Glob::GLOB_STATE_MOVE = 1;
const int Glob::GLOB_STATE_ATTACK = 2;
const int Glob::GLOB_STATE_TELEGRAPH = 3;
const int Glob::GLOB_STATE_DEAD = -1;

const int Glob::GLOB_AI_NORMAL = 0;
const int Glob::GLOB_AI_CHASE = 1;

int Glob::globsKilled = 0;


Glob::Glob(AnimationSet* animSet) {
	this->animSet = animSet;

	type = "enemy";

	//defaults
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 10 + (rand() % 20); //10-29
	damage = 0;
	collisionBoxW = 18;
	collisionBoxH = 20;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
	
	collisionBoxYOffset = -14;

	direction = DIR_DOWN;
	changeAnimation(GLOB_STATE_IDLE, true);

	updateCollisionBox();
}

void Glob::update() {
	//check if dead 
	if (hp < 1 && state != GLOB_STATE_DEAD) {
		changeAnimation(GLOB_STATE_DEAD, true);
		moving = false;
		die();
	}

	think();

	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();
	updateAnimation();
	updateInvincibleTimer();
}

void Glob::think() {
	if (state == GLOB_STATE_IDLE || state == GLOB_STATE_MOVE) {
		thinkTimer -= TimeController::timeController.dT;
		//time to choose an action
		if (thinkTimer <= 0) {
			//reset the Timer
			thinkTimer = rand() % 5; //0 to 5 seconds
			int action = rand() % 10; // 0-9

			if (action < 3) {
				moving = false;
				aiState = GLOB_AI_NORMAL;
				changeAnimation(GLOB_STATE_IDLE, true);
			}
			else
			{
				findNearestTarget();
				//found a target and its alive, CHASEE
				if (target != NULL && target->hp > 0) {
					float dist = Entity::distanceBetweenTwoEntities(this, target);
					// if in range, ATTACK
					if (dist < attackDistance) {
						telegraph(); // telegraph our attack first so the players have a chance to dodge
						aiState = GLOB_AI_NORMAL;

					}
					else{
						//otherwise move up to the target
						aiState = GLOB_AI_CHASE;
						moving = true;
						changeAnimation(GLOB_STATE_MOVE, state != GLOB_STATE_MOVE);
					}
				}
				else {
					//no target, go idle again
					moving = false;
					aiState = GLOB_AI_NORMAL;
					changeAnimation(GLOB_STATE_IDLE, true);
				}
			}
		}
	}
	// if chasing a target then do that
	if (aiState == GLOB_AI_CHASE && hp > 0 && active) {
		//get angle between me and target
		angle = Entity::angleBetweenTwoEntities(this, target);
		//move that way
		move(angle);
	}
}

void Glob::telegraph() {
	moving = false;
	frameTimer = 0;
	changeAnimation(GLOB_STATE_TELEGRAPH, true);
}

void Glob::attack() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(GLOB_STATE_ATTACK, true);
}

void Glob::die() {
	moving = false;
	state = GLOB_STATE_DEAD;
	changeAnimation(state, true);

	//add to scorecounter
	Glob::globsKilled++;
}

void Glob::findNearestTarget() {
	target = NULL;
	//find closest target
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
		if ((*entity)->type == "hero" && (*entity)->active) {
			//if found first hero in list just set them as target
			if (target == NULL) {
				target = (LivingEntity*)(*entity); //if cant cast to living entity. throw casting exception
			}
			else {
				//otherwise is the hero closer then the previous target?
				if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target)) {
					target = (LivingEntity*)(*entity);
				}
			}
		}
	}
}

void Glob::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;

	if (state == GLOB_STATE_IDLE) {
		if (direction = DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_DOWN);
		if (direction = DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_UP);
		if (direction = DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_LEFT);
		if (direction = DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_RIGHT);
	}
	else if (state == GLOB_STATE_MOVE) {
		if (direction = DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_DOWN);
		if (direction = DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_UP);
		if (direction = DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_LEFT);
		if (direction = DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_RIGHT);
	}
	else if (state == GLOB_STATE_ATTACK) {
		if (direction = DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_DOWN);
		if (direction = DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_UP);
		if (direction = DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_LEFT);
		if (direction = DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_RIGHT);
	}else if (state == GLOB_STATE_TELEGRAPH) {
		if (direction = DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_DOWN);
		if (direction = DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_UP);
		if (direction = DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_LEFT);
		if (direction = DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_RIGHT);
	}
	else if (state == GLOB_STATE_DEAD) {
		currentAnim = animSet->getAnimation(GLOB_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Glob::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	//if we are in move state but not going anywhere
	if (state == GLOB_STATE_MOVE && !moving) {
		changeAnimation(GLOB_STATE_IDLE, true);
	}
	//if we are idle but moving 
	if (state != GLOB_STATE_MOVE && moving) {
		changeAnimation(GLOB_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	//time to change frames 
	if (frameTimer >= currentFrame->duration) {
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			if (state == GLOB_STATE_TELEGRAPH) {
				//done telegraphing, now attack
				attack();
			}
			else if (state == GLOB_STATE_ATTACK) {
				changeAnimation(GLOB_STATE_MOVE, true);
			}
			else if (state == GLOB_STATE_DEAD) {
				frameTimer = 0;
				//if somehow alive again
				if (hp > 0)
					changeAnimation(GLOB_STATE_MOVE, true);
				else
					active = false;
			}
			else {
				//loop anim 
				currentFrame = currentAnim->getFrame(0);
			}
		}
		else {
			//otherwise jsut move to next frame
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Glob::updateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "hero") {
				//reference as livingentity so we can acces the hitbox
				LivingEntity* enemy = (LivingEntity*)(*entity);
				//if enemy hits me
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)) {
					hp -= enemy->damage;

					if (hp > 0) {
						invincibleTimer = 0.1;
					}
					//angle from other entity towards this entity so you get thrown into that direction
					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 250;
				}
			}
		}
	}
}
