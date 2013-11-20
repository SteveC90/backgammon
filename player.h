#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include "movepair.h"
#include "color.h"

using namespace std;

class Player {
private:
	Color color;
public:
	Player();
	Player(Color c);
	vector<MovePair> move(vector<int> roll);
	Color getColor() { return color; }
	string toString();
};