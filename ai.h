#pragma once
#include "player.h"

using namespace std;

class Ai : public Player {
private:
	static const float pipProbabilities[24];
	Color color;

	void evaluatePlays(vector<MoveConfiguration> &Plays) const;
	float evaluateBlotDangerForColor(const MoveConfiguration& configuration) const;
	float evaluteBlockadingFactor(const MoveConfiguration& configuration) const;
public:
	Ai() : Player() { }
	Ai(Color c) : Player(c) { };
	vector<MovePair> move(vector<int> roll, vector<MoveConfiguration>& plays);
	Color getColor() const { return color; }
	string toString();
};