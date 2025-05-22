#ifndef ACTOR_CPP_
#define ACTOR_CPP_

#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "GameController.h"

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: GraphObject(imageID, startX, startY, startDirection, size, depth) {
	setDirection(startDirection);
	moveTo(startX, startY);
	this->setVisible(true);
}

Actor::~Actor() {}

void Actor::doSomething() {}

unsigned int Actor::getIDNum() const {
	return imageIDNum;
}

void Actor::setIDNum(int numID) {
	imageIDNum = numID;
}

unsigned int Actor::getHealth() const {
	return health;
}

void Actor::setHealth(int healthAmt) {
	health = healthAmt;
}
StudentWorld* Actor::getStudentWorld() const {
    return studentWorld; // return the pointer to StudentWorld
 }

Ice::Ice(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth) {
	setIDNum(imageID);
	this->setVisible(true);
}

Ice::~Ice(){
}

void Ice::doSomething() {
}

iceMan::iceMan(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth){
	setIDNum(imageID);
	moveTo(30, 60);
	setHealth(10);
	moveTo(startX, startY);  
	this->setVisible(true);
}

iceMan::~iceMan(){

}
void iceMan::doSomething(){

}


Oil::Oil(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth){
	setIDNum(imageID);
	this->setVisible(true);
}

Oil::~Oil(){
	
}

void Oil::doSomething(){
	
}

Gold::Gold(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth)
{
	setIDNum(imageID);
	this->setVisible(true); 
}

Gold::~Gold(){

}

void Gold::doSomething(){

}

Boulder::Boulder(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
    : Actor(imageID, startX, startY, startDirection, size, depth)
{
    setIDNum(imageID);
    setHealth(1000000);  
    this->setVisible(true);
}

Boulder::~Boulder(){

}

void Boulder::doSomething()
{
    if (getHealth() == 0) {
        return;  // do nothing if Boulder is destroyed
    }

    waiting = false;    // Start off ready to fall and then check if actually stable

    int xPos = getX();
    int yPos = getY();

    if (waiting == false) { // check for stability
        for (int i = yPos; i > (yPos - 4); i--) { // check each ice spot 4 squares below boulder y position
            getStudentWorld()->getIceField();
        }
    }

    if (waiting == true) {
        if (waitTimer == 30) {
            GameController::getInstance().playSound(SOUND_FALLING_ROCK);  // Pplay sound when falling
            moveTo(xPos, yPos - 1);  // move Boulder one square down
        }
        else {
            waitTimer++;
        }
    }
}

void Boulder::setStability(bool setting)
{
    stable = setting;
}

bool Boulder::getStability() 
{
    return stable;
}

void Boulder::setWaiting(bool setting)
{
    waiting = setting;
}

bool Boulder::getWaiting() 
{
    return waiting;
}

void Boulder::incrementWaitTimer()
{
    waitTimer++;
}

#endif // ACTOR_CPP_