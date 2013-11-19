#pragma once
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <ctime>
#include <iostream>
#include "movepair.h"
#include "player.h"
#include "board.h"
#include "color.h"

using namespace std;

class Game {
private:
	Player p1, p2;
	Player *currentPlayer;
	Board board;

public:
	Game(Player &p1, Player &p2);
	void start();
	vector<int> roll();
	vector<int> chooseFirstPlayer();
	void swapPlayer();
	bool isPlayValid(vector<MovePair> moves, const vector<int>& diceRoll);
	bool isMoveValid(MovePair move);
	void moveGenerator(vector<int> roll, Board board);
};
