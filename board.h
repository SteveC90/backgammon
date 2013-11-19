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
	//second index is count(0) and player(1)
	int stacks[25][2];

public:
	Board();

	void draw();
	int getCheckerCountAt(int x);
	int getPlayerAt(int x);
};