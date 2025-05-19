#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"

class Actor : public GraphObject
{
public:

	Actor(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0)
		: GraphObject(imageID, startX, startY, startDirection, size, depth)
	{
		setDirection(startDirection);
		moveTo(startX, startY);
		this->setVisible(true);
	}
	~Actor()
	{
		
	}
	void doSomething()
	{

	}

	unsigned int getIDNum() const
	{
		return imageIDNum;
	}
	void setIDNum(int numID)
	{
		imageIDNum = numID;
	}
	unsigned int getHealth() const
	{
		return health;
	}
	void setHealth(int healthAmt)
	{
		health = healthAmt;
	}
private:
	int imageIDNum = 0;
	int health = 0;
};

class Ice : public Actor
{
public:
	Ice(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		this->setVisible(true);
	}
	~Ice()
	{
		
	}
	void doSomething()
	{

	}

private:

};

class iceMan : public Actor
{
public:
	iceMan(int imageID, int startX, int startY, Direction startDirection, float size = 1.0f, unsigned int depth = 0)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		moveTo(30, 60);
		setHealth(10);
		moveTo(startX, startY);
		this->setVisible(true);
	}
	~iceMan()
	{
		
	}
	void doSomething()
	{

	}
private:
	int waterUnits = 0;
	int sonarUnits = 0;
};

class Oil : public Actor
{
public:
	Oil(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		//this->setVisible(true);
	}
	~Oil()
	{
		
	}
	void doSomething()
	{

	}

private:

};

class Gold : public Actor
{
public:
	Gold(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		//this->setVisible(true);
	}
	~Gold()
	{
		
	}
	void doSomething()
	{

	}

private:

};
class Boulder : public Actor
{
public:
	Boulder(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 1)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		setHealth(1000000);
		this->setVisible(true);
	}
	~Boulder()
	{
		
	}
	void doSomething()
	{
		if (getHealth() == 0) {
			return;
		}
		int xPos = getX();
		int yPos = getY();

		if (stability == true) {
			for (int i = yPos - 4; i < yPos; i++) {
				
			}
		}
		if (waiting = true) {
			if (waitTimer == 30) {
				//GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
				moveTo(xPos, yPos - 1);
			}
			else {
				waitTimer++;
			}
		}
	}

private:
	bool stability = true;
	bool waiting = false;
	int waitTimer = 0;

};
#endif // ACTOR_H_
