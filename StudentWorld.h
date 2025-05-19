#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include <string>
#include "Actor.h"
#include <set>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility> // for std::pair
#include <random>    // for std::default_random_engine
using namespace std;


class StudentWorld : public GameWorld
{
private:
	std::string gameStats = "";
	int lives = 0;
	int health = 0;
	int water = 0;
	int oil = 0;
	int gold = 0;
	int score = 0;
	int sonar = 0;
	int level = 0;
	int boulders = 0;
	int goldNuggets = 0;
	int goldNuggetsCollected = 0;
	int oilBarrels = 0;
	int oilBarrelsCollected = 0;
	iceMan* player;
	Ice* ice[63][60];
	//Boulder* bigRocks[9];
	//Gold* gold[9];
	//Oil* oil[9];
	vector<Actor*> objectList;

public:

	StudentWorld(std::string assetDir) : GameWorld(assetDir)
	{
	}
	virtual int init()
	{
		// generate a signed int, then make a tracker (requires a signed integer eg no math in assignment operator)
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
		player = new iceMan(0, 30, 60, dirLeft, 1.0, 0);	// new player object
		int boulderTracker = 0;	// tracks number of rocks on map

		for (int x = 0; x < 64; x++) // generate new map of ice
		{
			for (int y = 0; y <= 59; y++)
			{
				if (x < 30 || x > 33)
				{
					ice[x][y] = new Ice(6, x, y, dirRight, 0.25, 3);
				}
				else if (x >= 30 && x <= 33 && y < 5)
				{
					ice[x][y] = new Ice(6, x, y, dirRight, 0.25, 3);
				}
			};
			if (x == 62) { // object list fills at  x =63 y = 1 for some reason
				int f = 7;
			}
		};

		// Generate a random spot on the map, as long as the spot isnt within 6 squares of anything that isnt an ice object, place the object

		int xPos = 0;
		int yPos = 0;

		// place boulders on the map
		int bouldersPlaced = 0;		// places boulders vs number of boulders that should be on the map
		while (bouldersPlaced != boulders) {
			randomValidLocation(xPos, yPos);
			Boulder* boulder = new Boulder(4, xPos, yPos, dirRight, 1, 1);
			objectList.push_back(boulder);
			//bigRocks[boulderTracker] = new Boulder(4, xPos, yPos, dirDown, 1, 1);
			bouldersPlaced++;
		}

		// place oil barrels on the map
		int oilPlaced = 0;		// places boulders vs number of boulders that should be on the map
		while (oilPlaced != barrels) {
			randomValidLocation(xPos, yPos);
			Oil *oilBarrel = new Oil(5, xPos, yPos, dirRight, 1, 2);
			objectList.push_back(oilBarrel);
			oilBarrel->setVisible(true);
			//ice[xPos][yPos] = new Oil(5, xPos, yPos, dirRight, 1, 2);
			//bigRocks[boulderTracker] = new Oil(4, xPos, yPos, dirDown, 1, 1);
			oilPlaced++;
		}

		// place gold nuggets on the map
		int goldPlaced = 0;		// places boulders vs number of boulders that should be on the map
		while (goldPlaced != golds) {
			randomValidLocation(xPos, yPos);
			Gold* goldNugget = new Gold(7, xPos, yPos, dirRight, 1, 2);
			objectList.push_back(goldNugget);
			goldNugget->setVisible(true);
			//bigRocks[boulderTracker] = new Gold(4, xPos, yPos, dirDown, 1, 1);
			goldPlaced++;
		}

		//objectList.push_back(*bigRocks); // add the vector of bigRocks to the object list

		updateScore(); // ouput scoreboard
		setGameStatText(gameStats);
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		updateScore(); // update the score/lives/level text at top of screen

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
	virtual void cleanUp()
	{
	}

	void updateScore()
	{
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
	void randomValidLocation(int& x, int& y) {

		bool safeSpot = false;
		int yPosition = 0;
		int xPosition = 0;

		while (safeSpot == false) {		//while the spot to place isnt safe, repeat this loop until safe spot is found

			safeSpot = true; // assume the spot is safe, check if it isnt later

			//generate random y position

			std::random_device rd;	// seeds the RNG
			std::mt19937 gen(rd()); // generates the random number from the seed
			std::uniform_int_distribution<> distY(6, 59); //ensures a uniform distribution over the specified range

			yPosition = distY(gen);

			// generate random x position

			std::uniform_int_distribution<> distX(0, 60); //ensures a uniform distribution over the specified range
			xPosition = distX(gen);

			while (xPosition >= 30 && xPosition <= 33) { // if the random x position is in the shaft, reroll it until it isn't
				xPosition = distX(gen);
			}

			////check if spots with 6 squares have another object that isn't ice
			//for (int yCheck = yPosition - 6; yCheck < yPosition + 6; yCheck++) {		// check +/- 6 y squares
			//	for (int xCheck = xPosition - 6; xCheck < xPosition + 6; xCheck++) { // check +/- 6 x squares
			//	}
			//}
			for (int i = 0; i < objectList.size(); i++) {
				for (int yCheck = yPosition - 6; yCheck < yPosition + 6; yCheck++) {		// check +/- 6 y squares
					for (int xCheck = xPosition - 6; xCheck < xPosition + 6; xCheck++) { // check +/- 6 x squares
						if (xCheck >= 0 && yCheck >= 0) {
							if (objectList[i]->getX() == xCheck && objectList[i]->getY() == yCheck) {
								safeSpot = false;
							}
						}
					}
				}
			}
		}
		
		y = yPosition;
		x = xPosition;
	}
};

#endif // STUDENTWORLD_H_
