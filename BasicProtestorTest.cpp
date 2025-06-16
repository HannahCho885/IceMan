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

bool Actor::getDeath() {
	return isDead;
}
void Actor::setDeath(bool set) {
	isDead = set;
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
				Actor* temp = nullptr;
				if (getStudentWorld()->checkRadialCollision(getX() - 1, getY(), 3, 4, temp)) {
					break;
				}
				moveTo(getX() - 1, getY());
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j < getY() + 4; j++) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
								GameController::getInstance().playSound(SOUND_DIG); // play sound
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
				Actor* temp = nullptr;
				if (getStudentWorld()->checkRadialCollision(getX() + 1, getY(), 3, 4, temp)) {
					break;
				}
				moveTo(getX() + 1, getY());
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j < getY() + 4; j++) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
								GameController::getInstance().playSound(SOUND_DIG); // play sound
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
				Actor* temp = nullptr;
				if (getStudentWorld()->checkRadialCollision(getX(), getY() + 1, 3, 4, temp)) {
					break;
				}
				moveTo(getX(), getY() + 1);
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j < getY() + 4; j++) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
								GameController::getInstance().playSound(SOUND_DIG); // play sound
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
				Actor* temp = nullptr;
				if (getStudentWorld()->checkRadialCollision(getX(), getY() - 1, 3, 4, temp)) {
					break;
				}
				moveTo(getX(), getY() - 1);
				for (int i = getX(); i < getX() + 4; i++) {
					for (int j = getY(); j > getY() - 1; j--) {
						if (j >= 0 && j < 60) {
							if (getStudentWorld()->getIceField(i, j) != nullptr) {
								delete getStudentWorld()->getIceField(i, j);
								getStudentWorld()->setIceField(i, j, nullptr);
								GameController::getInstance().playSound(SOUND_DIG); // play sound
							}
						}
					}
				}
			}
			break;
		case KEY_PRESS_SPACE:	// fire a squirt of water
			if (waterUnits > 0) {
				int squirtX = 0;
				int squirtY = 0;
				switch (getDirection()) {
				case down:
					squirtX = getX();
					squirtY = getY() - 4;
					break;
				case up:
					squirtX = getX();
					squirtY = getY() + 4;
					break;
				case left:
					squirtX = getX() - 4;
					squirtY = getY();
					break;
				case right:
					squirtX = getX() + 4;
					squirtY = getY();
					break;
				}
				GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);  // play sound when used
				if (squirtX >= 0 && squirtX <= 60 && squirtY >= 0 && squirtY <= 60) {
					Actor* temp = nullptr;
					if (!getStudentWorld()->checkRadialCollision(squirtX, squirtY, 3, 4, temp) && !getStudentWorld()->checkRadialCollision(squirtX, squirtY, 0, 6, temp)) {
						Squirt* squirt = new Squirt(3, squirtX, squirtY, getDirection(), 1, 1);
						getStudentWorld()->addToObjectList(squirt);
						squirt->setStudentWorld(getStudentWorld());
					}
				}
				waterUnits--;
			}
			break;
		case KEY_PRESS_ESCAPE:	// abort the level
			setHealth(0);
			setDeath(true);
			break;
		case 'z':	// use sonar kit
			if (sonarUnits > 0) {
				sonarUnits--;
				Actor* temp = nullptr;
				getStudentWorld()->checkRadialCollision(getX(), getY(), 12, 5, temp);	// check for nearby Oil
				if (temp != nullptr) {
					temp->setVisible(true);
				}
				getStudentWorld()->checkRadialCollision(getX(), getY(), 12, 7, temp);	// check for nearby Gold
				if (temp != nullptr) {
					temp->setVisible(true);
				}
			}
			break;
		case 'Z':	// use sonar kit
			if (sonarUnits > 0) {
				sonarUnits--;
				Actor* temp = nullptr;
				getStudentWorld()->checkRadialCollision(getX(), getY(), 12, 5, temp);	// check for nearby Oil
				if (temp != nullptr) {
					temp->setVisible(true);
				}
				getStudentWorld()->checkRadialCollision(getX(), getY(), 12, 7, temp);	// check for nearby Gold
				if (temp != nullptr) {
					temp->setVisible(true);
				}
			}
			break;
		case KEY_PRESS_TAB:	// place gold nugget on map
			if (getStudentWorld()->getGold() > 0) {
				Gold* goldNugget = new Gold(7, getX(), getY(), getDirection(), 1, 2);
				goldNugget->setTemp(true);
				goldNugget->setVisible(true);
				goldNugget->setStudentWorld(getStudentWorld());
				getStudentWorld()->addToObjectList(goldNugget); // push object to object list	
				getStudentWorld()->decrementGold();
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
	sonarUnits = sonarUnits + 2;
}

//Protestor Logic
protestor::protestor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth) :
	Actor(imageID, startX, startY, startDirection, size, depth) {
	setIDNum(imageID);
	this->setVisible(true);
	moveTo(startX, startY);
	setHealth(6);
}

protestor::~protestor() {

}

void protestor::leave_the_oilfield() {
	int startLocationX = getX();
	int startLocationY = getY();

	if (startLocationX == 60 && startLocationY == 60) {
		setHealth(0);
		setDeath(true);
		this->setVisible(false);
	}
	else {
		moveTo(60, 60);
	}
}

bool protestor::facingTowardIceMan() {

	Direction dir = getDirection();
	int playerX = getStudentWorld()->getPlayer()->getX();
	int playerY = getStudentWorld()->getPlayer()->getY();

	int protestorX = getX();
	int protestorY = getY();

	switch (dir) {
	case left:
		if (playerY - 4 <= protestorY <= playerY + 4) {
			return true;
		}
		else {
			return false;
		}
	case right:
		if (playerY - 4 <= protestorY <= playerY + 4) {
			return true;
		}
		else {
			return false;
		}
	case up:
		if (playerX - 4 <= protestorX <= playerX + 4) {
			return true;
		}
		else {
			return false;
		}
	case down:
		if (playerX - 4 <= protestorX <= playerX + 4) {
			return true;
		}
		else {
			return false;
		}
	}
}

void protestor::doSomething() { //every tick enemy will check location of player and follow 
	int playerX = getStudentWorld()->getPlayer()->getX();
	int playerY = getStudentWorld()->getPlayer()->getY();

	int protestorX = getX();
	int protestorY = getY();

	Direction dir = Direction(rand() % 4);


	if (tick % 4 >= 0) {
		if (getHealth() > 0) {
			if (protestorX == 60 && protestorY == 60) {
				protestor::leave_the_oilfield();
			}
			if (tick % 4 == 0) {
				Actor* temp = nullptr;
				getStudentWorld()->checkRadialCollision(protestorX, protestorY, 4, 0, temp); //check for iceMan
				if (temp != nullptr) {
					if (canYell == true) { //if 15 ticks pass through
						if (facingTowardIceMan() == true) { //if facing the direction of Iceman, then yell
							GameController::getInstance().playSound(SOUND_PROTESTER_YELL);
							int i = getStudentWorld()->getPlayer()->getHealth();
							getStudentWorld()->getPlayer()->setHealth(i - 2);
							canYell = false;
						}
					}
					if (tick % 15 == 0) {
						canYell = true;
					}
				}
				getStudentWorld()->checkRadialCollision(protestorX, protestorY, 3, 4, temp); //check for boulders
				if (temp != nullptr) { setDirection(dir); } //if its a boulder, then switch to a different direction

				if (getDirection() == right) {
					if (protestorX + 8 >= 58) { setDirection(dir); }
					else {
						for (int i = 0; i < 8; i++) {
							moveTo(protestorX + 1, protestorY);
							cout << "moving right" << endl;
						}
					}
					setDirection(left);
				}
				if (getDirection() == left) {
					if (protestorX - 8 <= 2) { setDirection(dir); }
					else {
						for (int i = 0; i < 8; i++) {
							moveTo(protestorX - 1, protestorY);
							cout << "moving left" << endl;
						}
					}
				}
				setDirection(dir);
			}
			if (getDirection() == up) {
				if (protestorY + 8 >= 59) { setDirection(dir); }
				else {
					for (int i = 0; i < 8; i++) {
						moveTo(protestorY + 1, protestorY);
						cout << "moving up" << endl;
					}
				}
				setDirection(dir);
			}
			if (getDirection() == down) {
				if (protestorY - 8 <= 1) { setDirection(dir); }
				else {
					for (int i = 0; i < 8; i++) {
						moveTo(protestorY - 1, protestorY);
						cout << "moving down" << endl;
					}
				}
				setDirection(dir);
			}
		}
		}
		else {
			GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
			protestor::leave_the_oilfield();
		}

	tick++;
}
	

	//Hardcore Protestor Logic
	hardcoreProtestor::hardcoreProtestor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth) :
		protestor(imageID, startX, startY, startDirection, size, depth) {
		setIDNum(imageID);
		this->setVisible(true);
		moveTo(startX, startY);
		setHealth(20);

	}

	hardcoreProtestor::~hardcoreProtestor() {

	}

	void hardcoreProtestor::leave_the_oilfield(int x, int y) {
		moveTo(x, y);
		this->setHealth(0);
		setDeath(true);
	}

	void hardcoreProtestor::doSomething() { //every tick enemy will check location of player and follow 
		//int playerX = getStudentWorld()->getPlayer()->getX();
		//int playerY = getStudentWorld()->getPlayer()->getY();

		//int protestorX = getX();
		//int protestorY = getY();

		tick++;

		maxTickWait = 3 - getStudentWorld()->getLevel() / 4;
		ticksToWaitBetweenMoves = std::max(0, maxTickWait);


		if (getHealth() == 0 && getDeath() == true) { // do nothing if dead
			return;
		}

		if (getHealth() == 0 && getX() == 60 && getY() == 60) { // if at 0 hp and at exit point, set death to true
			setDeath(true);
		}

		if (getHealth() == 0) {
			moveTo(60, 60);
		}

		if (tick % ticksToWaitBetweenMoves == 0) {
			Actor* temp = nullptr;
			if (getStudentWorld()->checkRadialCollision(getX(), getY(), 4, 0, temp) && canYell == true && isFacingIceMan()) {
				GameController::getInstance().playSound(SOUND_PROTESTER_YELL); // play sound
				temp->setHealth(temp->getHealth() - 2);	// injure the player by 2 hp
				canYell = false;
				tick++;
				return;
			}
			if (!getStudentWorld()->checkRadialCollision(getX(), getY(), 4, 0, temp)) {	// if the player isnt within a range of 4
				moves = 16 + getStudentWorld()->getLevel() * 2;	// calculate total moves to make
				int timesMoved = 0;
				while (timesMoved < moves) {
					if (getY() == getStudentWorld()->getPlayer()->getY()) {	// if the HCP is on the same y axis as the player
						if (getX() < getStudentWorld()->getPlayer()->getX()) { // if the HCP is to the left of the player
							if (!getStudentWorld()->checkCollision(getX() + 1, getY(), temp)) {	// as long as there is nothing to the right of HCP
								setDirection(right);
								moveTo(getX() + 1, getY());	// move right
								tick++;
								return;
							}
						}
						else if (getX() > getStudentWorld()->getPlayer()->getX()) { // if the HCP is to the right of the player
							if (!getStudentWorld()->checkCollision(getX() + 1, getY(), temp)) { // as long as there is nothing to the left of HCP
								setDirection(left);
								moveTo(getX() - 1, getY()); // move left
								tick++;
								return;
							}
						}
					}
					else if (getX() == getStudentWorld()->getPlayer()->getX()) { // if the HCP is on the same x axis as the player
						if (getY() < getStudentWorld()->getPlayer()->getY()) { // if the HCP is under the player
							if (!getStudentWorld()->checkCollision(getX(), getY() + 1, temp)) {	// as long as there is nothing above HCP
								setDirection(up);
								moveTo(getX(), getY() + 1);	// move up
								tick++;
								return;
							}
						}
						else if (getY() > getStudentWorld()->getPlayer()->getY()) { // if the HCP is above the player
							if (!getStudentWorld()->checkCollision(getX(), getY() - 1, temp)) { // as long as there is nothing below HCP
								setDirection(down);
								moveTo(getX(), getY() - 1); // move down
								tick++;
								return;
							}
						}
					}
					timesMoved++;
				}

			}
		}
		if (tick % (ticksToWaitBetweenMoves * 15) == 0 && canYell == false) {
			canYell = true;
		}
	}

	bool hardcoreProtestor::isFacingIceMan() {
		if (getX() > getStudentWorld()->getPlayer()->getX() && getDirection() == left && getY() == getStudentWorld()->getPlayer()->getY()) {
			return true;
		}
		if (getX() < getStudentWorld()->getPlayer()->getX() && getDirection() == right && getY() == getStudentWorld()->getPlayer()->getY()) {
			return true;
		}
		if (getY() < getStudentWorld()->getPlayer()->getY() && getDirection() == up && getX() == getStudentWorld()->getPlayer()->getX()) {
			return true;
		}
		if (getY() > getStudentWorld()->getPlayer()->getY() && getDirection() == down && getX() == getStudentWorld()->getPlayer()->getX()) {
			return true;
		}
		return false;
	}

	Oil::Oil(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
		: Actor(imageID, startX, startY, startDirection, size, depth) {
		setIDNum(imageID);
		this->setVisible(false);
		setHealth(1);
	}

	Oil::~Oil() {

	}

	void Oil::doSomething() {
		if (this->getHealth() == 0)	// If object is dead, do nothing
		{
			return;
		}
		int dx = getStudentWorld()->getPlayer()->getX() - getX();
		int dy = getStudentWorld()->getPlayer()->getY() - getY();

		if (!this->isVisible() && (dx * dx + dy * dy) <= 16) { // if oil is not visible and <= 4 units away from player
			this->setVisible(true);
			return;
		}

		if ((dx * dx + dy * dy) <= 9) { // if oil is <= 3 units away from player
			this->setHealth(0); // prep for object death
			setDeath(true);
			GameController::getInstance().playSound(SOUND_FOUND_OIL); // play sound
			getStudentWorld()->increaseScore(1000); // Increase score by 1000 points
			getStudentWorld()->incrementOil(); // Inform the StudentWorld object that it was picked up
		}
	}

	Gold::Gold(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		this->setVisible(false);
		setHealth(1);
	}

	Gold::~Gold() {

	}

	void Gold::doSomething() {
		if (tempGold == false) { // permanent gold only lootable by IceMan
			if (this->getHealth() == 0)	// If object is dead, do nothing
			{
				return;
			}

			int dx = getStudentWorld()->getPlayer()->getX() - getX();
			int dy = getStudentWorld()->getPlayer()->getY() - getY();

			if (!this->isVisible() && (dx * dx + dy * dy) <= 16) { // if gold is not visible and <= 4 units away from player
				this->setVisible(true);
				return;
			}

			if ((dx * dx + dy * dy) <= 9) { // if gold is <= 3 units away from player
				this->setHealth(0); // prep for object death
				setDeath(true);
				GameController::getInstance().playSound(SOUND_GOT_GOODIE); // play sound
				getStudentWorld()->increaseScore(10); // Increase score by 10 points
				getStudentWorld()->incrementGold(); // Inform the StudentWorld object that it was picked up
			}
		}
		else {	// temp gold only lootable by Protestors
			tick++;
			if (tick == 100) { // check to see if its tick lifetime has elapsed, and if so, set its state to dead
				setHealth(0);
				setDeath(true);
			}
			Actor* temp = nullptr;
			if (getStudentWorld()->checkRadialCollision(getX(), getY(), 1, 1, temp) || getStudentWorld()->checkRadialCollision(getX(), getY(), 1, 2, temp)) { // check if protestor steps on gold nugget
				GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD); // play sound
				getStudentWorld()->increaseScore(25); // Increase score by 25 points
				temp->setHealth(0);
				setHealth(0);
				setDeath(true);
			}
		}

	}
	void Gold::setTemp(bool setting) {
		tempGold = setting;
	}

	Boulder::Boulder(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		setHealth(1);
		this->setVisible(true);
	}

	Boulder::~Boulder() {

	}

	void Boulder::doSomething()
	{
		if (getHealth() == 0) {
			return;  // do nothing if Boulder is destroyed
		}
		int xPos = getX();
		int yPos = getY();

		// check each 4 x-coordinate ice spots below boulder's y position
		if (getStudentWorld()->getIceField(xPos, yPos - 1) == nullptr && getStudentWorld()->getIceField(xPos + 1, yPos - 1) == nullptr && getStudentWorld()->getIceField(xPos + 2, yPos - 1) == nullptr && getStudentWorld()->getIceField(xPos + 3, yPos - 1) == nullptr) {
			falling = true; // all 4 places below the boulder are empty, start the falling countdown
		}

		if (falling == true) {
			if (fallTimer >= 30) {	// once the boulder has waited 30 ticks
				if (fallTimer == 30) { // play the falling sound once
					GameController::getInstance().playSound(SOUND_FALLING_ROCK);  // play sound when falling
				}
				Actor* temp = nullptr;
				if (yPos == 0) { // if the boulder is at the bottom of the map
					setHealth(0);   // kill boulder
					setDeath(true);
					fallTimer = 0;
					falling = false;
				}

				for (int i = xPos; i <= xPos + 3; i++) { // check within 3 units of the boulder
					for (int j = yPos - 1; j < yPos; j++) {
						if (getStudentWorld()->checkCollision(i, j, temp)) { // if the boulder hits an object
							if (temp->getID() == 6 || temp->getID() == 4) { // if the object is ice or a boulder
								setHealth(0);   // kill boulder
								setDeath(true);
								fallTimer = 0;
								falling = false;
							}
						}
					}
				}
				//check of boulder falls on a protestor
				if (getStudentWorld()->checkRadialCollision(getX(), getY(), 3, 1, temp) || getStudentWorld()->checkRadialCollision(getX(), getY(), 3, 2, temp)) {
					temp->setDeath(true);	// reduce the protestor health to effectly zero
				}
				//check if boulder falls on player
				if (getStudentWorld()->checkRadialCollision(getX(), getY(), 3, 0, temp)) {
					temp->setHealth(0);	// reduce the player health to zero
					temp->setDeath(true);
				}
				getStudentWorld()->checkRadialCollision(getX(), getY(), 3, 1, temp);
				moveTo(xPos, yPos - 1);  // move Boulder one square down
				fallTimer++;
			}
			else {
				fallTimer++;
			}
		}
	}

	void Boulder::incrementWaitTimer()
	{
		fallTimer++;
	}

	void Actor::setStudentWorld(StudentWorld* inputStudentWorld) {
		studentWorld = inputStudentWorld;
	}

	Sonar::Sonar(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		this->setVisible(true);
		setHealth(1);
	}

	Sonar::~Sonar() {
	}

	void Sonar::doSomething() {
		maxNumSonar = 300 - (10 * getStudentWorld()->getLevel());
		maxNumSonar = max(100, maxNumSonar); // max number of ticks water can be alive, requires unsigned int for max function

		if (getHealth() == 0) {
			return;  // do nothing if sonar is destroyed
		}

		int dx = getStudentWorld()->getPlayer()->getX() - getX();
		int dy = getStudentWorld()->getPlayer()->getY() - getY();

		if ((dx * dx + dy * dy) <= 9) { // if sonar kit is <= 3 units away from player
			setHealth(0);
			GameController::getInstance().playSound(SOUND_GOT_GOODIE);  // play sound when picked up
			getStudentWorld()->getPlayer()->addSonar(); // tell the Iceman object that it just received a new Sonar Kit
			getStudentWorld()->increaseScore(75); // Increase score by 75 points
		}
		tick++;
		if (tick >= maxNumSonar) { // check to see if its tick lifetime has elapsed, and if so, set its state to dead
			setHealth(0);
			setDeath(true);
		}
	}

	Water::Water(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		this->setVisible(true);
		setHealth(1);
	}

	Water::~Water() {

	}

	void Water::doSomething() {
		maxNumWater = 300 - (10 * getStudentWorld()->getLevel());
		maxNumWater = max(100, maxNumWater); // max number of ticks water can be alive, requires unsigned int for max function

		if (getHealth() == 0) {
			return;  // do nothing if water is consumed
		}

		int dx = getStudentWorld()->getPlayer()->getX() - getX();
		int dy = getStudentWorld()->getPlayer()->getY() - getY();

		if ((dx * dx + dy * dy) <= 9) { // if water is <= 3 units away from player
			setHealth(0);
			setDeath(true);
			GameController::getInstance().playSound(SOUND_GOT_GOODIE);  // play sound when picked up
			getStudentWorld()->getPlayer()->addWater(); // tell the Iceman object that it just received 5 water squirts
			getStudentWorld()->increaseScore(100); // increase score by 100 points
		}

		tick++;
		if (tick >= maxNumWater) { // check to see if its tick lifetime has elapsed, and if so, set its state to dead
			setHealth(0);
			setDeath(true);
		}
	}
	Squirt::Squirt(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
		: Actor(imageID, startX, startY, startDirection, size, depth)
	{
		setIDNum(imageID);
		this->setVisible(true);
		setHealth(10);
	}

	Squirt::~Squirt() {

	}

	void Squirt::doSomething() {

		if (getHealth() == 0) {
			return;  // do nothing if water is consumed
		}

		int xPos = getX();
		int yPos = getY();
		Actor* temp = nullptr;

		for (int i = xPos - 3; i <= xPos + 3; i++) {
			for (int j = yPos - 3; j <= yPos; j++) {
				int dx = i - xPos;
				int dy = j - yPos;
				if (dx * dx + dy * dy <= 9) { // Euclidean distance <= 3
					if (i >= 0 && i <= 60 && j >= 0 && j <= 60) {
						if (getStudentWorld()->checkCollision(i, j, temp)) {
							if (temp->getID() == 1 || temp->getID() == 2) {
								if (temp->getHealth() == 1) {
									temp->setHealth(temp->getHealth() - 1); // damage protestor or hardcore protestor
								}
								if (temp->getHealth() >= 2) {
									temp->setHealth(temp->getHealth() - 2); // damage protestor or hardcore protestor
								}
								setHealth(0);
								setDeath(true);
								break;
							}
						}
					}
				}
			}
		}
		if (distance == 4) { // if squirt traveled full distance, kill it
			setHealth(0);
			setDeath(true);
			return;
		}

		switch (getDirection()) {
		case up:
			if (getStudentWorld()->checkCollision(getX(), getY() + 1, temp)) {
				setHealth(0);
				setDeath(true);
			}
			else {
				moveTo(xPos, yPos + 1);  // move squirt one square up
			}
			break;
		case down:
			if (getStudentWorld()->checkCollision(getX(), getY() - 1, temp)) {
				setHealth(0);
				setDeath(true);
			}
			else {
				moveTo(xPos, yPos - 1);  // move squirt one square down
			}
			break;
		case left:
			if (getStudentWorld()->checkCollision(getX() - 1, getY(), temp)) {
				setHealth(0);
				setDeath(true);
			}
			else {
				moveTo(xPos - 1, yPos);  // move squirt one square left
			}
			break;
		case right:
			if (getStudentWorld()->checkCollision(getX() + 1, getY(), temp)) {
				setHealth(0);
				setDeath(true);
			}
			else {
				moveTo(xPos + 1, yPos);  // move squirt one square right
			}
			break;
		}
		distance++;

	}

	void Squirt::setDirection() {

	}

#endif // ACTOR_CPP_
