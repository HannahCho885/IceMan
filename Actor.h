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

private:
	int waterUnits = 0;
	int sonarUnits = 0;

};

class Oil : public Actor
{
public:
    Oil(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

    ~Oil();

	void doSomething();

private:

};

class Gold : public Actor
{
public:
    Gold(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);

    ~Gold();

	void doSomething();

private:
	bool tempGold = false;
	int tick = 30;
};

class Boulder : public Actor
{
public:


	Boulder(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 1);

	~Boulder();

	void doSomething();

	void setStability(bool setting);


	bool  getStability();

	void setWaiting(bool setting);

	bool  getWaiting();

	void incrementWaitTimer();

private:

	bool stable;
	bool waiting;
	int waitTimer = 0;

};

#endif // ACTOR_H_

