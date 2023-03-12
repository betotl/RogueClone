#include "GameSystem.h"
#include <iostream>
#include <conio.h>


//Constructor sets up the game
GameSystem::GameSystem(string levelFile) {
	_player.init(1, 100, 10, 10, 0);
	_level.load(levelFile, _player);
}

void GameSystem::playGame() {
	bool isDone = false;
	while (!isDone) {
		_level.print();
		playerMove();
		_level.updateEnemies(_player);
	}
}

void GameSystem::playerMove() {
	char input;
	printf("Enter a move command (w/a/s/d): ");
	input = _getch();
	_level.movePlayer(input, _player);
}