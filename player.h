#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include "movepair.h"
#include "moveconfiguration.h"
#include "color.h"

using namespace std;

class Player {
private:
	Color color;
	
public:
	Player();
	Player(Color c);
	virtual vector<MovePair> move(vector<int> roll, vector<MoveConfiguration>& plays);
	Color getColor() const { return color; }
	string toString();
};