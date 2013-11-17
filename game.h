#pragma once
#include <cstdlib>
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
	bool areValidMoves(vector<MovePair> moves);
};