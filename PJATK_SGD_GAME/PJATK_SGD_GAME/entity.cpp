#include "entity.h"

const int Entity::DIR_UP = 0, Entity::DIR_DOWN = 1, Entity::DIR_RIGHT = 2, Entity::DIR_LEFT = 3, Entity::DIR_NONE = -1;

void Entity::update() { ; } //override me to do something usefull

void Entity::draw() {
	//ovveride me if you want MOAR stuff
	//draws current frame
	if (currentFrame != NULL && active) {
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	//draw collisionBos ( for debbuging )
	if (solid && Globals::debugging) {
		//sets the current drawing color 
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}
}

void Entity::move(float angle) {
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = angleToDirection(angle);
	//if direction change update current animation
	if (direction != newDirection) {
		direction = newDirection;
		changeAnimation(state, false); // e.g. if running right and on state 3, stay on state 3 but goes up 
	}
}

void Entity::updateMovement() {
	updateCollisionBox();
	//store collisionbox before we move 
	lastCollisionBox = collisionBox;

	//reset total moves
	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0) {
		//works out move distance using speed dt(time since last loop) and multiplies by lerp
		float moveDist = moveSpeed*(TimeController::timeController.dT)*moveLerp; 
		if (moveDist > 0) {
			//xmove = distance * cos(angle in rad)
			float xMove = moveDist * (cos(angle * Globals::PI / 180));
			//ymove = distance * sin(angle in rad)
			float yMove = moveDist * (sin(angle * Globals::PI / 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving)
				moveSpeed -= moveDist; //slowing down softly 
		}
	}
	//sliding around
	if (slideAmount > 0) {
		float slideDist = slideAmount * TimeController::timeController.dT * moveLerp;
		if (slideDist > 0) {
			float xMove = slideDist* (cos(slideAngle * Globals::PI / 180));
			float yMove = slideDist * (sin(slideAngle * Globals::PI / 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			slideAmount -= slideDist; //slowing down softly 
		}
		else {
			slideAmount = 0;
		}
	}
	//now that we move, move collisionBox to new spot we are on
	updateCollisionBox();
	//to help with collision checking, union collisionBox with lastcollisionBox
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::updateCollisionBox() {
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y - collisionBoxYOffset;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::updateCollision() {
	if (active && collideWithSolids) {
		// list of potential collisions
		list<Entity*> collisions;

		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			//if we collide with the entiy into our "united collisionbox* -> sdl_unionrect
			if ((*entity)->active // if entity is active 
				&& (*entity)->type != this->type // the entity we are colliding into is not the same as our entity type
				&& (*entity)->solid  // the entity we are colliding into is solid (e.g. wall or enemy)
				&& Entity::checkCollision(collisionBox, (*entity)->collisionBox)) { //we are colliding with it

				//add it to the list
				collisions.push_back(*entity);
			}
		}
		//if we have a list of potential enemies we may hit, check them to do the "collision"
		if (collisions.size() > 0) {
			updateCollisionBox();

			//multisample check for collisions from where we starded to where we want to go

			//first we find the sample distance we should travel between the checks
			float boxTravelSize = 0;
			if (collisionBox.w < collisionBox.h)
				boxTravelSize = collisionBox.w / 4;
			else
				boxTravelSize = collisionBox.h / 4;

			// use sampleBox to check for collisions form start to end moving at boxTravelSize each sample
			SDL_Rect sampleBox = lastCollisionBox;
			float movementAngle = Entity::angleBetweenTwoRects(lastCollisionBox, collisionBox);

			bool foundCollision = false; 
			while (!foundCollision) {
				//check samplebox for collision where it is now
				SDL_Rect intersection;
				for (auto entity = collisions.begin(); entity != collisions.end(); entity++) {
					if (SDL_IntersectRect(&sampleBox, &(*entity)->collisionBox, &intersection)) {
						foundCollision = true;
						moveSpeed = 0;
						moving = false;
						slideAngle = angleBetweenTwoEntities((*entity), this);

						//currently intersecting a entity, now we do collision resolution e.g. if we bumped from going down we get pushed back up
						if (intersection.w < intersection.h) {
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*entity)->collisionBox.x + (*entity)->collisionBox.w / 2)
								sampleBox.x -= intersection.w; //started on left so we bump right
							else
								sampleBox.x += intersection.w; // started on right so we bump left
						}
						else { 
							if (lastCollisionBox.y + lastCollisionBox.h / 2 < (*entity)->collisionBox.y + (*entity)->collisionBox.h / 2)
								sampleBox.y -= intersection.h; //started above so bump up 
							else
								sampleBox.y += intersection.h; //started below so bump down
						}
					}
				}
				//if collisionsFound or sampleBox is at same place as collisionBox, exit the loop
					//if we hit someing
				if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y))
					break;
				//move sample box up to check the next spot
				if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize) {
					float xMove = boxTravelSize * (cos(movementAngle * Globals::PI / 180));
					float yMove = boxTravelSize * (sin(movementAngle * Globals::PI / 180));

					sampleBox.x += xMove;
					sampleBox.y += yMove;
				}
				else {
					sampleBox = collisionBox;
				}
			}
			if (foundCollision) {
				//move our entity to where the sampleBox ended up
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w/2;
				y = sampleBox.y - collisionBoxYOffset;
			}
			updateCollisionBox();
		}
	}
}

float Entity::distanceBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2) {
	SDL_Point e1, e2;
	e1.x = r1.x + r1.w / 2;
	e1.y = r1.y + r1.h / 2;

	e2.x = r2.x + r2.w / 2;
	e2.y = r2.y + r2.h / 2;

	float d = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2))); //distance between 2 points
	return d;
}

float Entity::distanceBetweenTwoEntities(Entity* e1, Entity* e2) {
	float d = abs(sqrt(pow(e2->x - e1 ->x, 2) + pow(e2->y - e1->y, 2)));
	return d;
}

float Entity::angleBetweenTwoEntities(Entity* e1, Entity* e2) {
	float dx, dy;
	float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;
	
	dx = x2 - x1;
	dy = y2 - y1;

	return atan2(dy, dx) * 180 / Globals::PI;
}

bool Entity::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2) {
	if (SDL_IntersectRect(&cbox1, &cbox2, nullptr)) {
		return true;
	}
	else {
		return false;
	}
}

int Entity::angleToDirection(float angle) {
	if ((angle >= 0 && angle <= 45) || (angle >= 270 + 45 && angle <= 360)) {
		return DIR_RIGHT;
	}
	else if (angle >= 45 && angle <= 90 + 45) {
		return DIR_DOWN;
	}
	else if (angle >= 90 + 45 && angle <= 180 + 45) {
		return DIR_LEFT;
	}
	else {
		return DIR_UP;
	}
}

float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2) {
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return atan2(dy, dx) * 180 / Globals::PI;
}

float Entity::angleBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2) {
	float cx1 = r1.x + (r1.w / 2);
	float cy1 = r1.y + (r1.h / 2);

	float cx2 = r2.x + (r2.w / 2);
	float cy2 = r2.y + (r2.h / 2);

	return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}

list<Entity*> Entity::entities;

bool Entity::EntityCompare(const Entity* const& a, const Entity* const& b) {
	if (a != 0 && b != 0) {
		return (a->y < b->y);
	}
	else {
		return false;
	}
}

//OPTIMASATION
void Entity::removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		//if entity is not active
		if (!(*entity)->active) {
			if (deleteEntities)
				delete (*entity);
			entity = entityList->erase(entity);
		}
		else {
			entity++;
		}
	}
}

void Entity::removeAllFromList(list<Entity*>* entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		if (deleteEntities)
			delete (*entity);
		entity = entityList->erase(entity);
	}
}

