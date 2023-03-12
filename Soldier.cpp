#include "Soldier.h"
#include <random>

//Constructor
Soldier::Soldier(string name, char tile, int level, int attack, int defense, int health, int xp) {
	_name = name;
	_tile = tile;
	_level = level;
	_attack = attack;
	_defense = defense;
	_health = health;
	_experienceValue = xp;
}

//Sets the position of the soldier
void Soldier::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

//Gets the position of the soldier using reference variables
void Soldier::getPosition(int& x, int& y) {
	x = _x;
	y = _y;
}

//Gets random attack rolll from 0 to _attack
int Soldier::attack() {
	//we use a static random engine so it only initializes once
	static default_random_engine randomEngine(time(NULL));
	uniform_int_distribution<int> attackRoll(0, _attack);
	//Return the random roll
	return attackRoll(randomEngine);
}

//Makes the soldier take damage according to its defense. Returns 0 if he didn't die, 
//and _experienceValue if he did.
int Soldier::takeDamage(int attack) {
	attack -= _defense;
	//check if the attack does damage
	if (attack > 0) {
		_health -= attack;
		//check if he died
		if (_health <= 0) {
			return _experienceValue;
		}
	}
	return 0;
}

//This does the soldier AI
char Soldier::getMove(int playerX, int playerY) {
	int distance;
	//We get the distance from the player
	//so we know if we should move towards him
	//or just ambe around randomly
	int dx = _x - playerX;
	int dy = _y - playerY; 
	int adx = abs(dx);
	int ady = abs(dy);

	distance = adx + ady;

	//If the distance is closer than 6 tiles, then we move towards the player
	if (distance <= 5) {
		//Moving along X axis
		if (adx > ady) {
			//if player is left, move left. Otherwise move right
			if (dx > 0) {
				return 'a';
			}
			else {
				return 'd';
			}
		}
		else { //Move along Y axis
			if (dy > 0) {
				return 'w';
			}
			else {
				return 's';
			}
		}
	}
}