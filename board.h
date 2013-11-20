#pragma once
#include <cstdlib>
#include <stack>
#include <iostream>
#include "player.h"
#include "color.h"
#include "movepair.h"

using namespace std;

class Board {
private:
	static const int columnWidth = 3;

	//first index is stack number
	//second index is 0 - count and 1 - player color
	int stacks[25][2];

	int piecesRemaining[2];

	void fillStack(const int stackIndex, const int piecesCount, const Color color);

public:
	Board();
	Board(const Board& oldBoard);

	void draw() const;
	int getCheckerCountAt(int x) const;
	Color getPlayerAt(int x) const;
	void moveChecker(const MovePair& move);
	bool playerCanBearOff(const Player* const player) const;
};