#include "StudentWorld.h"
#include <string>
#include <stdexcept>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) {
}

int StudentWorld::init() {
	// generate a signed int for how many of each to spawn (requires a signed integer eg. no math in assignment operator)
	int boulders = getLevel() / 2 + 2;
	int golds = 5 - getLevel() / 2;
	int barrels = 2 + getLevel();
	boulders = std::min(boulders, 9);
	numGoldNuggets = std::max(golds, 2);
	numOilBarrels = std::min(barrels, 21);

	GraphObject::Direction dirLeft = GraphObject::Direction::left; // direction player will start off facing
	GraphObject::Direction dirRight = GraphObject::Direction::right; // direction ice will start off facing
	GraphObject::Direction dirDown = GraphObject::Direction::down; // direction boulder will start off facing
	GraphObject::Direction dirUp = GraphObject::Direction::up;
	player = new iceMan(0, 30, 60, dirLeft, 1.0, 0);	// generate new player object
	player->setStudentWorld(this);
	
	enemy = new protestor(1, 59, 60, dirLeft, 1.0, 0); 	//generate new protestor object
	enemy->setStudentWorld(this);
	objectList.push_back(enemy);

	for (int i = 0; i < 64; i++) { 	// initialize the ice map to nullptr to prevent read errors
		for (int j = 0; j < 60; j++) {
			ice[i][j] = nullptr;
		}
	}
	for (int x = 0; x <= 63; x++) // generate new map of ice
	{
		for (int y = 0; y <= 59; y++)
		{
			if (x < 30 || x > 33)
			{
				ice[x][y] = new Ice(6, x, y, dirRight, 0.25, 3);
			}
			else if (x >= 30 && x <= 33 && y < 4)
			{
				ice[x][y] = new Ice(6, x, y, dirRight, 0.25, 3);
			}
		};
	};

	int xPos = 0; // x position of object to be placed
	int yPos = 0; // y position of object to be placed

	int bouldersPlaced = 0;		// tracks boulders placed vs number of boulders that should be on the map

	while (bouldersPlaced != boulders) {
		randomValidLocation(xPos, yPos);
		while (yPos < 20) { // boulders must be distributed between x=0,y=20 and x=60,y=56, inclusive (so they have room to fall)
			randomValidLocation(xPos, yPos); //  reroll boulder position if necessary 
		}
		Boulder* boulder = new Boulder(4, xPos, yPos, dirRight, 1, 1); // Boulder builds from the bottom left corner of the boulder
		boulder->setStudentWorld(this);

		// clear out the 4 x 4 ice beneath the boulder
		for (int i = yPos; i < (yPos + 4); i++) {
			for (int n = xPos; n < (xPos + 4); n++) {
				if (ice[n][i] != nullptr) {
					delete ice[n][i];
					ice[n][i] = nullptr;
				}
			}
		}
		objectList.push_back(boulder);	// push object to object list
		bouldersPlaced++;
	}

	// place oil barrels on the map
	int oilPlaced = 0;		// places boulders vs number of boulders that should be on the map
	while (oilPlaced != numOilBarrels) {
		randomValidLocation(xPos, yPos);
		Oil* oilBarrel = new Oil(5, xPos, yPos, dirRight, 1, 2);
		oilBarrel->setStudentWorld(this);
		objectList.push_back(oilBarrel);	// push object to object list
		oilPlaced++;
	}

	// place gold nuggets on the map
	int goldPlaced = 0;		// places boulders vs number of boulders that should be on the map
	while (goldPlaced != numGoldNuggets) {
		randomValidLocation(xPos, yPos);
		Gold* goldNugget = new Gold(7, xPos, yPos, dirRight, 1, 2);
		goldNugget->setTemp(false);
		goldNugget->setStudentWorld(this);
		objectList.push_back(goldNugget);	// push object to object list
		goldPlaced++;
	}

	goodieChance = getLevel() * 30 + 290; // 1 in goodieChance chance for a Water Pool or Sonar Kit to spawn

	updateScore(); // ouput scoreboard
	setGameStatText(gameStats);
	return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move() {

	std::random_device rd;	// seeds the RNG
	std::mt19937 gen(rd()); // generates the random number from the seed
	std::uniform_int_distribution<> itemSpawn(0, goodieChance - 1); //ensures a uniform distribution over the specified range
	std::uniform_int_distribution<> itemRoll(0, 4); //ensures a uniform distribution over the specified range
	if (itemSpawn(gen) == 0) {
		GraphObject::Direction dirRight = GraphObject::Direction::right; // direction object will start off facing

		if (itemRoll(gen) == 0) {	// 1 in 5 chance to spawn a sonar kit
			Sonar* sonarKit = new Sonar(8, 0, 60, dirRight, 1, 2);
			sonarKit->setStudentWorld(this);
			objectList.push_back(sonarKit);	// push object to object list
		}
		else {	// else spawn a water pool
			int xPos = 0; // x position of object to be placed
			int yPos = 0; // y position of object to be placed
			Actor* temp = nullptr;
			randomValidOpenLocation(xPos, yPos); 			// ensure water isnt placeed on anything else in a 4x4 square
			Water* waterPool = new Water(9, xPos, yPos, dirRight, 1, 2);
			waterPool->setStudentWorld(this);
			objectList.push_back(waterPool);	// push object to object list
		}
	}


	player->doSomething();	// do player action

	for (int i = 0; i < objectList.size(); i++) { // move for all the static objects
		if (objectList[i] != nullptr) {
			objectList[i]->doSomething();
		}
	}

	updateScore(); // update the score/lives/level text at top of screen

	for (int i = 0; i < objectList.size(); i++) { // delete dead game objects
		if (objectList[i] != nullptr) {
			if (objectList[i]->getHealth() == 0 && objectList[i]->getDeath() == true) {
				delete objectList[i];
				objectList[i] = nullptr;
			}
		}
	}

	if (player->getHealth() == 0) {	// check for player death
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	//if the player has collected all of the Barrels on the level, then return the result that the player finished the level
	if (numOilBarrels == oilBarrelsCollected) {
		GameController::getInstance().playSound(GWSTATUS_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME; 	// the player hasn’t completed the current level and hasn’t died, let them continue playing the current level

}
void StudentWorld::cleanUp() {
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (ice[i][j] != nullptr) {
				delete ice[i][j];
			}
		}
	}
	delete[] player;
	for (Actor* actor : objectList) { // clears out objectList
		delete actor;
	}
	oilBarrelsCollected = 0;
	goldNuggetsCollected = 0;
	objectList.clear();
}

void StudentWorld::updateScore() {
	//example:    Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
	gameStats = "Lvl: " + std::to_string(getLevel());
	gameStats += "  Lives: " + std::to_string(getLives());
	gameStats += "  Hlth: " + std::to_string(player->getHealth()*10);	// note: set to % instead of raw amount
	gameStats += "%  Wtr: " + std::to_string(player->getWaterUnits());
	gameStats += "  Gld: " + std::to_string(goldNuggetsCollected);
	gameStats += "  Oil Left: " + std::to_string(numOilBarrels - oilBarrelsCollected);
	gameStats += "  Sonar: " + std::to_string(player->getSonarUnits());
	gameStats += "  Scr: " + std::to_string(getScore());
	setGameStatText(gameStats);
}

// generate a random spot on the map, as long as the spot isnt within 6 squares of anything that isnt an ice object or in the shalft
void StudentWorld::randomValidLocation(int& x, int& y) {

	bool safeSpot = false;
	int yPosition = 0;
	int xPosition = 0;


	while (safeSpot == false) {		//while the spot to place isnt safe, repeat this loop until safe spot is found

		safeSpot = true; // assume the spot is safe, check if it isnt later

		//generate random y position

		std::random_device rd;	// seeds the RNG
		std::mt19937 gen(rd()); // generates the random number from the seed
		std::uniform_int_distribution<> distY(0, 56);

		yPosition = distY(gen);

		// generate random x position

		std::uniform_int_distribution<> distX(0, 60); //ensures a uniform distribution over the specified range
		xPosition = distX(gen);

		while (xPosition >= 30 && xPosition <= 33) { // if the random x position is in the mine shaft, reroll it until it isn't
			xPosition = distX(gen);
		}

		for (int i = 0; i < objectList.size(); i++) {  // check the positiongenerated against the spot of every other object
			for (int yCheck = yPosition - 6; yCheck <= yPosition + 6; yCheck++) {  // check +/- 6 y squares
				for (int xCheck = xPosition - 6; xCheck <= xPosition + 6; xCheck++) { // check +/- 6 x squares
					if (xCheck >= 0 && yCheck >= 0) {
						// calculate squared Euclidean distance
						int dx = xCheck - xPosition;
						int dy = yCheck - yPosition;
						if (dx * dx + dy * dy <= 36) { // if the distance is less than 6 squares
							if (objectList[i]->getX() == xCheck && objectList[i]->getY() == yCheck) {
								safeSpot = false;
							}
						}
					}
				}
			}
		}
	}
	y = yPosition;
	x = xPosition;
}
// generate a random spot on the map, as long as the spot isnt within 6 squares of anything
void StudentWorld::randomValidOpenLocation(int& x, int& y) {

	bool safeSpot = false;
	int yPosition = 0;
	int xPosition = 0;


	while (safeSpot == false) {		//while the spot to place isnt safe, repeat this loop until safe spot is found

		safeSpot = true; // assume the spot is safe, check if it isnt later

		//generate random y position

		std::random_device rd;	// seeds the RNG
		std::mt19937 gen(rd()); // generates the random number from the seed
		std::uniform_int_distribution<> distY(0, 56);

		yPosition = distY(gen);

		// generate random x position

		std::uniform_int_distribution<> distX(0, 60); //ensures a uniform distribution over the specified range
		xPosition = distX(gen);

		for (int yCheck = yPosition; yCheck <= yPosition + 3 && safeSpot; yCheck++) {  // check +/- 4 y squares
			for (int xCheck = xPosition; xCheck <= xPosition + 3 && safeSpot; xCheck++) { // check +/- 4 x squares
				Actor* temp = nullptr;
				if (checkCollision(xCheck, yCheck, temp) == true) {
					safeSpot = false;
				}
			}
		}
	}
	y = yPosition;
	x = xPosition;
}

StudentWorld:: ~StudentWorld() {	// Destructor (copy of cleanUp())
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (ice[i][j] != nullptr) {
				delete ice[i][j];
			}
		}
	}
	delete[] player;	// delete player IceMan from memory

	for (Actor* actor : objectList) { // clears out objectList from memory
		delete[] actor;
	}
	objectList.clear();
}

iceMan* StudentWorld::getPlayer() {
	return player;
}

Ice* StudentWorld::getIceField(int x, int y) {
	return ice[x][y];
}

void StudentWorld::setIceField(int x, int y, Ice* temp) {
	ice[x][y] = temp;
}

vector<Actor*> StudentWorld::getObjectList() {
	return objectList;
}

void StudentWorld::incrementOil() {
	oilBarrelsCollected++;
}

void StudentWorld::incrementGold() {
	goldNuggetsCollected++;
}

bool StudentWorld::checkCollision(int x, int y, Actor*& object) {	// check for collision, return true and what you collided with if you collided
	for (int i = 0; i < objectList.size(); i++) {	// check for collision with any object in the list
		if (objectList[i] != nullptr) {
			if (objectList[i]->getX() == x && objectList[i]->getY() == y) {
				object = objectList[i];
				return true;
			}
		}
	}
	if (player->getX() == x && player->getY() == y) {	// check for collision with player
		object = player;
		return true;
	}
	if (x >= 0 && x <= 63 && y >= 0 && y <= 59) {
		if (ice[x][y] != nullptr) {	// check for collision with ice
			object = ice[x][y];
			return true;
		}
	}
	return false;
}

void StudentWorld::addToObjectList(Actor* object) {
	objectList.push_back(object);
}

bool StudentWorld::checkRadialCollision(int x, int y, int range, int targetID, Actor*& objectHit) {
	for (int i = 0; i < objectList.size(); i++) {
		if (objectList[i] != nullptr) {
			if (objectList[i]->getID() == targetID) {	// check if its the target object
				int dx = x - objectList[i]->getX();
				int dy = y - objectList[i]->getY();
				double distance = std::sqrt(dx * dx + dy * dy);

				if (distance <= range) {
					objectHit = objectList[i];  // set the pointer to the object hit
					objectList[i]->setVisible(true);
					return true;
				}
			}
		}
	}
	if (player->getID() == targetID) {	// check if its the target object
		int dx = x - player->getX();
		int dy = y - player->getY();
		double distance = std::sqrt(dx * dx + dy * dy);

		if (distance <= range) {
			objectHit = player;  // set the pointer to the object hit
			return true;
		}
	}
	return false;
}

int StudentWorld::getGold() {
	return goldNuggetsCollected;
}

void StudentWorld::decrementGold() {
	goldNuggetsCollected--;
}