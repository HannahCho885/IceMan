#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include "Actor.h"
#include <string>
#include <set>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility> // for std::pair
#include <random>    // for std::default_random_engine
using namespace std;
class Actor;

class StudentWorld : public GameWorld
{
private:
	std::string gameStats = "";
	int goodieChance = 0;
	int gold = 0;
	int boulders = 0;
	int numGoldNuggets = 0;
	int goldNuggetsCollected = 0;
	int numOilBarrels = 0;
	int oilBarrelsCollected = 0;
	iceMan* player;
	Ice* ice[64][60];
	vector<Actor*> objectList;
	

public:

	StudentWorld(std::string assetDir);

	~StudentWorld();

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void updateScore();

	void randomValidLocation(int& x, int& y);

	void StudentWorld::randomValidOpenLocation(int& x, int& y);

	void incrementOil();

	void incrementGold();

	int getGold();

	void decrementGold();

	iceMan* getPlayer();

	Ice* getIceField(int x, int y);

	void setIceField(int x, int y, Ice* temp);

	vector<Actor*> getObjectList();

	void addToObjectList(Actor* object);

	bool checkCollision(int x, int y, Actor*& object);

	bool checkRadialCollision(int x, int y, int range, int targetID, Actor* objectHit);
};

#endif // STUDENTWORLD_H_
