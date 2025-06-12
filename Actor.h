#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "GameController.h"


class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

    virtual ~Actor();

    virtual void doSomething();

    unsigned int getIDNum() const;

    void setIDNum(int numID);

    unsigned int getHealth() const;

    void setHealth(int healthAmt);

	StudentWorld* getStudentWorld() const; // Return pointer to StudentWorld
	
	void setStudentWorld(StudentWorld* studentWorld);

private:
	StudentWorld* studentWorld = nullptr; // pointer to StudentWorld
    int imageIDNum = 0;
    int health = 0;
	int annoyance = 0;

};

class Ice : public Actor
{
public:
    Ice(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

	~Ice();

    void doSomething();

private:

};

class iceMan : public Actor
{
public:
    iceMan(int imageID, int startX, int startY, Direction startDirection, float size = 1.0f, unsigned int depth = 0);

	~iceMan();

	void doSomething();

	int getWaterUnits();

	int getSonarUnits();

	void addWater();

	void addSonar();

private:
	int waterUnits = 5;
	int sonarUnits = 1;

};

class Oil : public Actor
{
public:
    Oil(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

    ~Oil();

	void doSomething();

private:
	int tick = 0;
	
};

class Gold : public Actor
{
public:
    Gold(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

    ~Gold();

	void doSomething();

	void setTemp(bool setting);

private:
	bool tempGold = false;
	int tick = 0;
};

class Boulder : public Actor
{
public:


	Boulder(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 1);

	~Boulder();

	void doSomething();

	void incrementWaitTimer();

private:

	bool falling = false;
	int fallTimer = 0;
	int tick = 0;

};

class Sonar : public Actor
{
public:
	Sonar(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

	~Sonar();

	void doSomething();

private:
	int tick = 0;
	int maxNumSonar = 0;
};

class Water : public Actor
{
public:
	Water(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

	~Water();

	void doSomething();

private:
	int tick = 0;
	int maxNumWater = 0;
};



class Squirt : public Actor
{
public:
	Squirt(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

	~Squirt();

	void doSomething();

	void setDirection();

private:
	int tick = 0;
	int distance = 0;
};

#endif // ACTOR_H_
