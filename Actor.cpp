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

Actor::~Actor() {
}

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

Ice::~Ice() {
}

void Ice::doSomething() {
}

iceMan::iceMan(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth) {
	setIDNum(imageID);
	moveTo(30, 60);
	setHealth(10);
	moveTo(startX, startY);
	this->setVisible(true);
}

iceMan::~iceMan() {

}
void iceMan::doSomething() {
	int ch;
	if (getStudentWorld()->getKey(ch) == true)
	{
		switch (ch) {
		case KEY_PRESS_LEFT:
			if (left != getDirection()) { setDirection(left); }
			if (getX() - 1 < 0) { break; }
			else {
				moveTo(getX() - 1, getY());
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j < getY() + 4; j++) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
							}

						}
					}
				}
			}
			break;
		case KEY_PRESS_RIGHT:
			if (right != getDirection()) { setDirection(right); }
			if (getX() + 4 == 64) { break; }
			else {
				moveTo(getX() + 1, getY());
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j < getY() + 4; j++) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
							}
						}
					}
				}
			}
			break;
		case KEY_PRESS_UP:
			if (up != getDirection()) { setDirection(up); }
			if (getY() + 4 == 64) { break; }
			else {
				moveTo(getX(), getY() + 1);
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j < getY() + 4; j++) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
							}
						}
					}
				}
			}
			break;
		case KEY_PRESS_DOWN:
			if (down != getDirection()) { setDirection(down); }
			if (getY() - 1 < 0) { break; }
			else {
				moveTo(getX(), getY() - 1);
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j > getY() - 1; j--) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
							}
						}
					}
				}
			}
			break;
		}
	}
}


int iceMan::getWaterUnits() {
	return waterUnits;
}

int iceMan::getSonarUnits() {
	return sonarUnits;
}

void iceMan::addWater() {
	waterUnits += 5;
}

void iceMan::addSonar() {
	sonarUnits++;
}

Oil::Oil(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth) {
	setIDNum(imageID);
	this->setVisible(false);
}

Oil::~Oil() {

}

void Oil::doSomething() {
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

Gold::~Gold() {

}

void Gold::doSomething() {
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

Boulder::~Boulder() {

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
		// check each 4 x-coordinate ice spots below boulder's y position
		if (getStudentWorld()->getIceField(xPos, yPos - 1) == nullptr && getStudentWorld()->getIceField(xPos + 1, yPos - 1) == nullptr && getStudentWorld()->getIceField(xPos + 2, yPos - 1) == nullptr && getStudentWorld()->getIceField(xPos + 3, yPos - 1) == nullptr) { 
			waiting = true;	// all 4 places below the boulder are empty, start the falling countdown
		}
	}

	if (waiting == true) {
		if (waitTimer >= 30) {
			if (waitTimer == 30) { // only play the sound once
				GameController::getInstance().playSound(SOUND_FALLING_ROCK);  // play sound when falling
			}
			if (getStudentWorld()->getIceField(xPos, yPos - 1) != nullptr || yPos == 0) { // if the spot below boulder is an object or the bottom of map
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

Sonar::Sonar(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth)
{
	setIDNum(imageID);
	this->setVisible(true);
	maxNumSonar = 300 - (10 * getStudentWorld()->getLevel());
	maxNumSonar = max(100, maxNumSonar); // max number of ticks water can be alive, requires unsigned int for max function
}

Sonar::~Sonar() {
}

void Sonar::doSomething() {
	if (getHealth() == 0) {
		return;  // do nothing if sonar is destroyed
	}

	if (abs(getStudentWorld()->getPlayer()->getX() - getX()) <= 3 || abs(getStudentWorld()->getPlayer()->getY() - getY() <= 3)) { // if player is within 3 units
		setHealth(0);
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);  // play sound when picked up
		getStudentWorld()->getPlayer()->addSonar(); // tell the Iceman object that it just received a new Sonar Kit
		getStudentWorld()->increaseScore(75); // Increase score by 75 points
	}
	tick++;
	if (tick >= maxNumSonar) { // check to see if its tick lifetime has elapsed, and if so, set its state to dead
		setHealth(0);
	}
}

Water::Water(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
	: Actor(imageID, startX, startY, startDirection, size, depth)
{
	setIDNum(imageID);
	this->setVisible(false);
	maxNumWater = 300 - (10 * getStudentWorld()->getLevel());
	maxNumWater = max(100, maxNumWater); // max number of ticks water can be alive, requires unsigned int for max function
}

Water::~Water() {

}

void Water::doSomething() {
	if (getHealth() == 0) {
		return;  // do nothing if sonar is destroyed
	}
	if (abs(getStudentWorld()->getPlayer()->getX() - getX()) <= 3 || abs(getStudentWorld()->getPlayer()->getY() - getY() <= 3)) { // if player is within 3 units
		setHealth(0);
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);  // play sound when picked up
		getStudentWorld()->getPlayer()->addWater(); // tell the Iceman object that it just received 5 water squirts
		getStudentWorld()->increaseScore(100); // increase score by 100 points
	}
	tick++;
	if (tick >= maxNumWater) { // check to see if its tick lifetime has elapsed, and if so, set its state to dead
		setHealth(0);
	}
}

#endif // ACTOR_CPP_