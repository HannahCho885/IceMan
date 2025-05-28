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
    studentWorld = getStudentWorld();
}

Actor::~Actor() {}

void Actor::doSomething() {
}

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
	this->setVisible(false);
}

Oil::~Oil(){
	
}

void Oil::doSomething(){
    if (this->getHealth() > 0) {    // If object not dead
        if (this->isVisible() == false) { // if oil is not visible and <= 4 units away from player
            this->isVisible() == true;
            return;
        }
        else if (abs(getStudentWorld()->getPlayer()->getX() - getX()) <= 3 || abs(getStudentWorld()->getPlayer()->getY() - getY() <= 3)) { // if oil is <= 3 units away from player
            this->setHealth(0); // prep for object death
            GameController::getInstance().playSound(SOUND_FOUND_OIL);   // play sound
            getStudentWorld()->increaseScore(1000); // Increase score by 1000 points
            getStudentWorld()->incrementOil();// Inform the StudentWorld object that it was picked up
        }
    }
}

Gold::Gold(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth)
{
	setIDNum(imageID);
	this->setVisible(false); 
}

Gold::~Gold(){

}

void Gold::doSomething(){
    if (this->getHealth() > 0) {
        if (this->isVisible() == false) { // if oil is not visible and <= 4 units away from player
            this->isVisible() == true;
            return;
        }
        else if (abs(getStudentWorld()->getPlayer()->getX() - getX()) <= 3 || abs(getStudentWorld()->getPlayer()->getY() - getY() <= 3)) { // if oil is <= 3 units away from player
            this->setHealth(0); // prep for object death
            GameController::getInstance().playSound(SOUND_GOT_GOODIE);   // play sound
            getStudentWorld()->increaseScore(10); // Increase score by 10 points
            getStudentWorld()->incrementGold();// Inform the StudentWorld object that it was picked up
        }
    }
    if (tempGold == true) { // check to see if its tick lifetime has elapsed, and if so, set its state to dead
        if (tick == 30) {
            this->setHealth(0);
        }
        else {
            tick--;
        }
    }
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
        for (int i = xPos; i > (xPos + 4); i++) { // check each 4 x-coordinate ice spots below boulder's y position
            if(getStudentWorld()->getIceField(i, yPos-1) == nullptr) {
                waiting = true;
            }
        }
    }

    if (waiting == true) {
        if (waitTimer >= 30) {
            if (waitTimer == 30) { // only play the sound once
                GameController::getInstance().playSound(SOUND_FALLING_ROCK);  // Pplay sound when falling
                waiting = true;
            }
            if (getStudentWorld()->getIceField(xPos, yPos-1) != nullptr || yPos == 0) { // If the spot below boulder is an object or the bottom of map
                setHealth(0);   // kill boulder
                waitTimer = 0;
                waiting = false;
            }
            else {
                moveTo(xPos, yPos - 1);  // move Boulder one square down
            }

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

void Actor::setStudentWorld(StudentWorld* inputStudentWorld) {
    studentWorld = inputStudentWorld;
}

#endif // ACTOR_CPP_