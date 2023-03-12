#pragma once
#include <string>

using namespace std;

class Enemy {
public:
	Enemy(string name, char tile, int level, int attack, int defense, int health, int xp);
	

	//Setters
	void setPosition(int x, int y);

	//Getters
	void getPosition(int& x, int& y);
	string getName() { return _name; }
	char getTile() { return _tile; }

	int attack();
	int takeDamage(int attack);

	//Gets AI move command
	char getMove(int playerX, int playerY);

private:
	string _name;
	char _tile;

	int _level;
	int _attack;
	int _defense;
	int _health;
	int _experienceValue;

	//Position
	int _x;
	int _y;
};

