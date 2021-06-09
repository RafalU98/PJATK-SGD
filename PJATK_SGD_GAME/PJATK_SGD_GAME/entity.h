#ifndef ENTITY
#define ENTITY

#include "globals.h"
#include "timeController.h"
#include "animationSet.h"

//Abstract Class. cannot instantiatie of type Entity e.g. cannot do Entity entity;
class Entity {
public:
	//reference constans
	static const int DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE;

	//quick label to see what the entity is doing
	int state;

	float x, y;
	int direction;
	bool solid = true;// is this thing solid? can things pass true?
	bool collideWithSolids = true; //sometimes we are solid but I can pass trough other solids
	bool active = true; //entity turned on or off
	string type = "entity"; // what type of entity is it? e.g. hero,enemy,wall etc.
	bool moving; // is the entity moving
	float angle; // angle to move entity
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle; //direction we are pushed to
	float slideAmount; //how had we are pushed
	float moveLerp = 4; //smooths out the movement
	float totalXMove, totalYMove; //everytime we move sth, we keep track of it ( incase of needing to retrack)

	SDL_Rect collisionBox; // box describing the size of our entity and when it "hits" another entity 
	SDL_Rect lastCollisionBox; //where our collisionbox was last 
	int collisionBoxW, collisionBoxH; // our default collisionBox size
	float collisionBoxYOffset; // from my entities y value, where should I "sit" the collisionBox

	AnimationSet *animSet; //all animations this entity can have
	Animation* currentAnim; //current animation the entity is using
	Frame* currentFrame; //the current frame of animation
	float frameTimer; //keeps track from frame to frame

	//VIRTUAL FUNCTIONS
	virtual void update();
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();

	virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0; //abstract function
	virtual void updateCollision(); //how we bump into stuff in the world

	//HELP FUNCTIONS
	static float distanceBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2);
	static float distanceBetweenTwoEntities(Entity* e1, Entity* e2);
	static float angleBetweenTwoEntities(Entity* e1, Entity* e2);
	static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2); 
	static int angleToDirection(float angle);
	static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2);

	//global list of entites i can refer to at any time
	static list<Entity*> entities; 
	static bool EntityCompare(const Entity* const &a, const Entity* const &b); //helps with sorting the entities with comparing (sorts based on the y value)
	static void removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities); //remove enemies from the list, and a bool if want to delete the completely
	static void removeAllFromList(list<Entity*>* entityList, bool deleteEntities); //empty the whole list 








};

#endif