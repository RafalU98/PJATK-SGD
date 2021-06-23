#include "game.h"
#include "randomNumber.h"

Game::Game(){
	std::string resPath = getResourcePath();
	backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);
	splashImage = loadTexture(resPath + "title.png", Globals::renderer);
	overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer);

	splashShowing = true;
	overlayTimer = 2;

	//holds a list of group types. this list describes the types of groups of data our frames can have!
	std::list<DataGroupType> dataGroupTypes;

	//so what data can a frame have?
	//collisionBoxes (although we have hardcoded the collision boxes)
	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//hitboxes
	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//damage
	DataGroupType dmgType;
	dmgType.groupName = "damage";
	dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

	//add all of these dataTypes to the list
	dataGroupTypes.push_back(colBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(dmgType);

	heroAnimSet = new AnimationSet();
	heroAnimSet->loadAnimationSet("hero.fdset", dataGroupTypes, true, 0, true);

	globAnimSet = new AnimationSet();
	globAnimSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

	grobAnimSet = new AnimationSet();
	grobAnimSet->loadAnimationSet("grob.fdset", dataGroupTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	//build hero entity
	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = Globals::ScreenWidth / 2;
	hero->y = Globals::ScreenHeight / 2;
	//tells keyboard input to manage hero
	heroInput.hero = hero;
	//add hero to the entity list
	Entity::entities.push_back(hero);

	int tileSize = 32;
	//build all the walls for this game
	//first. build walls on top and bottom of screen
	for (int i = 0; i < Globals::ScreenWidth / tileSize; i++){
		//fills in top row
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize/2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create bottom row
		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = Globals::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}
	//now the sides
	for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++){
		//fills in left column
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create right column
		newWall = new Wall(wallAnimSet);
		newWall->x = Globals::ScreenWidth - tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

}

Game::~Game(){
	cleanup(backgroundImage);
	cleanup(splashImage);
	cleanup(overlayImage);

	if (scoreTexture != NULL) // if used
		cleanup(scoreTexture);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete wallAnimSet;
	delete globAnimSet;
	delete grobAnimSet;

	delete hero;

	//delete all of the wall entities
	Entity::removeAllFromList(&walls, true);

	//delete all the enemies
	Entity::removeAllFromList(&enemies, true);
}

void Game::update(){
	//enemy related stuff
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;

	bool quit = false;

	SDL_Event e;
	//setup my time controller before the game starts
	TimeController::timeController.reset();
	//game loop!
	while (!quit){
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		//remove / delete dead enemies
		Entity::removeInactiveEntitiesFromList(&enemies, true);

		//check for any events that might have happened
		while (SDL_PollEvent(&e)){
			//close the window
			if (e.type == SDL_QUIT)
				quit = true;
			//if keydown event
			if (e.type == SDL_KEYDOWN){
				//switch case on which button was pressed
				switch (e.key.keysym.scancode){
				case SDL_SCANCODE_ESCAPE: //esc key
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (splashShowing)
						splashShowing = false;
					if (overlayTimer <= 0 && hero->hp < 1) {
						//cleanup and restart the game
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;

						Glob::globsKilled = 0;
						Grob::grobsKilled = 0;
						if (scoreTexture != NULL) {
							cleanup(scoreTexture);
							scoreTexture = NULL;
						}
						//remove all existing enemies
						for (std::list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++) {
							(*enemy)->active = false;
						}
						hero->revive();
					}
					break;
				}
			}
			heroInput.update(&e);
		}
		//make our overlay timer tick down
		if (hero->hp < 1 && overlayTimer > 0) {
			overlayTimer -= TimeController::timeController.dT;
		}

		//update all entities
		for (std::list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			//update all entities in game world at once
			(*entity)->update();
		}

		//spawn enemies
		if (hero->hp > 0 && !splashShowing) {
			if (enemiesToBuild == enemiesBuilt) {
				enemiesToBuild = enemiesToBuild * 2;
				enemiesBuilt = 0;
			}
			enemyBuildTimer -= TimeController::timeController.dT;
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10) {
				Glob* enemey = new Glob(globAnimSet);
				//set enemies position to somewhere random in the arena
				enemey->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				enemey->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
				enemey->invincibleTimer = 0.1;
				enemies.push_back(enemey);
				Entity::entities.push_back(enemey);

				if (enemiesBuilt % 5 == 0)
				{
					Grob* grob = new Grob(grobAnimSet);
					grob->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16; //random x value between our walls
					grob->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16; //random x value between our walls
					grob->invincibleTimer = 0.01;

					enemies.push_back(grob);
					Entity::entities.push_back(grob);
				}
			}
		}
		//draw all entities
		draw();
	}

}
void Game::draw(){
	//clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);


	if (splashShowing) {
		renderTexture(splashImage, Globals::renderer, 0, 0);
	}
	else {
		//draw the background
		renderTexture(backgroundImage, Globals::renderer, 0, 0);
	
		//sort all entities based on y(depth)
		Entity::entities.sort(Entity::EntityCompare);
		//draw all of the entities
		for (std::list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			(*entity)->draw();
		}

		if (overlayTimer <= 0 && hero->hp < 1) {
			renderTexture(overlayImage, Globals::renderer, 0, 0);
			if (scoreTexture == NULL) {
				SDL_Color color = { 255, 0,0 ,255 };

				std::stringstream ss;

				ss << "ENEMIES KILLED: " << Glob::globsKilled + Grob::grobsKilled;

				std::string resPath = getResourcePath();
				scoreTexture = renderText(ss.str(), resPath + "vermin_vibes_1989.ttf", color, 45, Globals::renderer);
			}
			renderTexture(scoreTexture, Globals::renderer, 40, 50);
		}
	}
	//after we're done drawing/rendering, show it to the screen
	SDL_RenderPresent(Globals::renderer);
}