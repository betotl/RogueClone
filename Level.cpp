#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>
#include <fstream>
#include <cstdio>

#include <Windows.h>

Level::Level() {

}

void Level::load(string fileName, Player& player) {
	//Loads the level
	ifstream ifs;
	ifs.open(fileName);
	if (ifs.fail()) {
		perror(fileName.c_str());
		exit(1);
	}

	string line;
	while (getline(ifs, line)) {
		_levelData.push_back(line);
	}
	ifs.close();

	//Process the level
	char tile;
	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			tile = _levelData[i][j];
			switch (tile) {
			case '@': //Payer
				player.setPosition(j, i);
				break;
			case 'S': //Snake
				_enemies.push_back(Enemy("Snake", tile, 1, 3, 1, 10, 10));
				_enemies.back().setPosition(j, i);
				break;
			case 'g':
				_enemies.push_back(Enemy("Goblin", tile, 2, 10, 5, 35, 50));
				_enemies.back().setPosition(j, i);
				break;
			case 'O':
				_enemies.push_back(Enemy("Ogre", tile, 4, 20, 20, 200, 500));
				_enemies.back().setPosition(j, i);
				break;
			case 'D':
				_enemies.push_back(Enemy("Dragom", tile, 100, 2000, 2000, 2000, 500000));
				_enemies.back().setPosition(j, i);
				break;
			case 'B':
				_enemies.push_back(Enemy("Bandit", tile, 3, 15, 10, 100, 250));
				_enemies.back().setPosition(j, i);
				break;
			}
		}
	}
}

void Level::print() {
	cls();
	//cout << string(100, '\n');
	for (int i = 0; i < _levelData.size(); i++) {
		printf("%s\n", _levelData[i].c_str());
	}
	printf("\n");
}

void Level::movePlayer(char input, Player& player) {
	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);

	switch (input) {
	case 'w': //up
	case 'W':
		processPlayerMove(player, playerX, playerY - 1);
		break;
	case 's': //down
	case 'S':
		processPlayerMove(player, playerX, playerY + 1);
		break;
	case 'a': //left
	case 'A':
		processPlayerMove(player, playerX - 1, playerY);
		break;
	case 'd': //right
	case 'D':
		processPlayerMove(player, playerX + 1, playerY);
		break;
	default:
		printf("INVALID INPUT");
		system("PAUSE");
		break;
	}
}

void Level::updateEnemies(Player& player) {
	char aiMove;
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	player.getPosition(playerX, playerY);
	for (int i = 0; i < _enemies.size(); i++) {
		aiMove = _enemies[i].getMove(playerX, playerY);
		_enemies[i].getPosition(enemyX, enemyY);
		switch (aiMove) {
		case 'w': //up
			processEnemyMove(player, i, enemyX, enemyY - 1);
			break;
		case 's': //down
			processEnemyMove(player, i, enemyX, enemyY + 1);
			break;
		case 'a': //left
			processEnemyMove(player, i, enemyX - 1, enemyY);
			break;
		case 'd': //right
			processEnemyMove(player, i, enemyX + 1, enemyY);
			break;
		default:
			break;
		}
	}
}

char Level::getTile(int x, int y) {
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile) {
	_levelData[y][x] = tile;
}

void Level::processPlayerMove(Player& player, int targetX, int targetY) {
	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);

	switch (moveTile) {
	case '.':
		player.setPosition(targetX, targetY);
		setTile(playerX, playerY, '.');
		setTile(targetX, targetY, '@');
	case '#':
		break;
	default:
		battleMonster(player, targetX, targetY);
		break;
	}
}

void Level::processEnemyMove(Player& player, int enemyIndex, int targetX, int targetY) {
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	_enemies[enemyIndex].getPosition(enemyX, enemyY);
	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);

	switch (moveTile) {
	case '.':
		_enemies[enemyIndex].setPosition(targetX, targetY);
		setTile(enemyX, enemyY, '.');
		setTile(targetX, targetY, _enemies[enemyIndex].getTile());
	case '@':
		battleMonster(player, enemyX, enemyY);
		break;
	default:
		break;
	}
}

void Level::battleMonster(Player& player, int targetX, int targetY) {
	
	int enemyX;
	int enemyY;
	int playerX;
	int playerY;
	int attackRoll = 0;
	int attackResult;
	string enemyName;

	player.getPosition(playerX, playerY);

	for (int i = 0; i < _enemies.size(); i++) {

		_enemies[i].getPosition(enemyX, enemyY);
		enemyName = _enemies[i].getName();

		if (targetX == enemyX && targetY == enemyY) {
			//Battle
			attackRoll = player.attack();
			printf("\nPlayer attacked %s with a roll of %d\n", enemyName.c_str(), attackRoll);
			attackResult = _enemies[i].takeDamage(attackRoll);
			if (attackResult != 0) {
				setTile(targetX, targetY, 'x');
				print();
				printf("Monster died!\n");
				system("PAUSE");
				setTile(targetX, targetY, '.');
				player.addExperience(attackResult);

				_enemies[i] = _enemies.back();
				_enemies.pop_back();
				i--;
				return;
			}
			//Monster turn
			attackRoll = _enemies[i].attack();
			printf("%s attacked player with a roll of %d\n", enemyName.c_str(), attackRoll);
			attackResult = player.takeDamage(attackRoll);
			if (attackResult != 0) {
				setTile(playerX, playerY, 'x');
				print();
				printf("You died!\n");
				system("PAUSE");
				exit(0);
			}
			system("PAUSE");
		}
	}
}



void Level::cls() {
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}