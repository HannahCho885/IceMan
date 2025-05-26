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
	Ice* ice[64][60];
	vector<Actor*> objectList;
	

public:

	StudentWorld(std::string assetDir);

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void updateScore();

	void randomValidLocation(int& x, int& y);

	void incrementOil();

	void incrementGold();

	iceMan* getPlayer();

	Ice* getIceField(int x, int y);

	vector<Actor*> getObjectList();
};

#endif // STUDENTWORLD_H_
