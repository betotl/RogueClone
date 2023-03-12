#pragma once

using namespace std;

class Player {
public:
	Player();
	void init(int level, int health, int attack, int defense, int experience);

	int attack();
	int takeDamage(int attack);

	//Setters
	void setPosition(int x, int y);
	void addExperience(int experience);
	
	//Getters
	void getPosition(int& x, int& y);

private:
	//Properties
	int _level;
	int _health;
	int _attack;
	int _defense;
	int _experience;

	//Position
	int _x;
	int _y;
};

