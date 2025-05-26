#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir){
}
int StudentWorld::init(){
	// generate a signed int, then make a tracker (requires a signed integer eg. no math in assignment operator)
	int boulders = getLevel() / 2 + 2;
	int golds = 5 - getLevel() / 2;
	int barrels = 2 + getLevel();
	boulders = std::min(boulders, 9);
	golds = std::max(golds, 2);
	barrels = std::min(barrels, 21);

	GraphObject::Direction dirLeft = GraphObject::Direction::left; // direction player will start off facing
	GraphObject::Direction dirRight = GraphObject::Direction::right; // direction ice will start off facing
	GraphObject::Direction dirDown = GraphObject::Direction::down; // direction boulder will start off facing
	GraphObject::Direction dirUp = GraphObject::Direction::up;
	player = new iceMan(0, 30, 60, dirLeft, 1.0, 0);	// generate new player object

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

	int xPos = 0; // x position of boulder
	int yPos = 0; // y position of boulder

	int bouldersPlaced = 0;		// tracks boulders placed vs number of boulders that should be on the map

	while (bouldersPlaced != boulders) {
		randomValidLocation(xPos, yPos);
		while (yPos < 20) { // boulders must be distributed between x=0,y=20 and x=60,y=56, inclusive (so they have room to fall)
			randomValidLocation(xPos, yPos); //  reroll boulder position if necessary 
		}
		Boulder* boulder = new Boulder(4, xPos, yPos, dirRight, 1, 1); // Boulder builds from the bottom left corner of the boulder
		
		// clear out the 4 x 4 ice beneath the boulder
		for (int i = yPos; i < (yPos + 4); i++) {
			for (int n = xPos; n < (xPos + 4); n++) {
				delete ice[n][i];
			}
		}
		
		objectList.push_back(boulder);	// push object to object list

		bouldersPlaced++;
	}

	// place oil barrels on the map
	int oilPlaced = 0;		// places boulders vs number of boulders that should be on the map
	while (oilPlaced != barrels) {
		randomValidLocation(xPos, yPos);
		Oil* oilBarrel = new Oil(5, xPos, yPos, dirRight, 1, 2);
		objectList.push_back(oilBarrel);	// push object to object list
		oilPlaced++;
	}

	// place gold nuggets on the map
	int goldPlaced = 0;		// places boulders vs number of boulders that should be on the map
	while (goldPlaced != golds) {
		randomValidLocation(xPos, yPos);
		Gold* goldNugget = new Gold(7, xPos, yPos, dirRight, 1, 2);
		objectList.push_back(goldNugget);	// push object to object list
		goldPlaced++;
	}

	updateScore(); // ouput scoreboard
	setGameStatText(gameStats);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
	for (int i = 0; i < objectList.size(); i++) { // move for all the static objects
		switch (objectList[i]->getID()) { // do something depending on which object it is
		case 4: // boulder objects
			objectList[i]->doSomething();
			break;
		case 5:	// oil barrel objects
			objectList[i]->doSomething();
			break;
		case 7: // gold nugget objects
			objectList[i]->doSomething();
			break;
		}
	}
	//for each of the actors in the game world{
	//	if (actor[i] is still active / alive){
	//		ask each actor to do something (e.g. move) 
	//		tellThisActorToDoSomething(actor[i]);
	//		if (theplayerDiedDuringThisTick() == true) {
	//			return GWSTATUS_PLAYER_DIED;
	//	}
	//		if (theplayerCompletedTheCurrentLevel() == true){
	//			return GWSTATUS_FINISHED_LEVEL;
	//		}
	//	}
	//}


	updateScore(); // update the score/lives/level text at top of screen
	//removeDeadGameObjects(); // delete dead game objects return the proper result

	if (player->getHealth() == 0) {	// check for player death
		return GWSTATUS_PLAYER_DIED;
	}

	//if the player has collected all of the Barrels on the level, then return the result that the player finished the level
	//if (oilBarrels == oilBarrelsCollected) {
	//	GameController::getInstance().playSound(GWSTATUS_FINISHED_LEVEL);
	//	return GWSTATUS_FINISHED_LEVEL;
	//}

	return GWSTATUS_CONTINUE_GAME; 	// the player hasn’t completed the current level and hasn’t died, let them continue playing the current level

}
void StudentWorld:: cleanUp(){
	delete player;
	std::vector<Actor*>().swap(objectList);  // Replaces with an empty vector and frees memory
	delete ice;
}

void StudentWorld::updateScore(){
	lives = getLives();
	score = getScore();
	level = getLevel();
	score = getScore();
	//Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
	gameStats = "Lvl: " + std::to_string(level);
	gameStats += " Lives: " + std::to_string(lives);
	gameStats += " Hlth: " + std::to_string(health);
	gameStats += " % Wtr : " + std::to_string(water);
	gameStats += " Gld: " + std::to_string(gold);
	gameStats += " Oil Left: " + std::to_string(oil);
	gameStats += " Sonar: " + std::to_string(sonar);
	gameStats += " Scr: " + std::to_string(score);
}

// generate a random spot on the map, as long as the spot isnt within 6 squares of anything that isnt an ice object
void StudentWorld::randomValidLocation(int& x, int& y) { 

	bool safeSpot = false;
	int yPosition = 0;
	int xPosition = 0;

	while (safeSpot == false) {		//while the spot to place isnt safe, repeat this loop until safe spot is found

		safeSpot = true; // assume the spot is safe, check if it isnt later

		//generate random y position

		std::random_device rd;	// seeds the RNG
		std::mt19937 gen(rd()); // generates the random number from the seed
		std::uniform_int_distribution<> distY(0, 56); //ensures a uniform distribution over the specified range

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
iceMan* StudentWorld::getPlayer() {
	return player;
}

Ice* StudentWorld:: getIceField(int x, int y) {
	return ice[x][y];
}


vector<Actor*> StudentWorld:: getObjectList() {
	return objectList;
}

void StudentWorld::incrementOil() {
	oilBarrelsCollected++;
}

void StudentWorld::incrementGold() {
	goldNuggetsCollected++;
}