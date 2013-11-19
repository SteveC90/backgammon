#pragma once
#include <cstdlib>
#include <stack>
#include <iostream>
#include "color.h"

using namespace std;

class Board {
private:
	static const int columnWidth = 3;

	//first index is stack number
	//second index is 0 - count and 1 - player color
	int stacks[25][2];

public:
	Board();
	Board(const Board& oldBoard);

	void draw() const;
	int getCheckerCountAt(int x) const;
	int getPlayerAt(int x) const;
	void moveChecker(int from, int to);
};