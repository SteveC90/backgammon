#include "player.h"

Player::Player() {
		
}

Player::Player(Color c) {
	this->color = c;
}

vector<MovePair> Player::move(vector<int> roll) {
	cout << "Enter moves as '<from #> <to #>'. Enter 25 as <from #> for the Bar. Enter '0 0' to end turn: " << endl;
	vector<MovePair> moves;

	// Read player input for moves
	for (int i=0; i<roll.size(); ++i) {
		MovePair move;

		cin >> move.from >> move.to;
		if (move.to == -1) 
			move.bearingOff = true;
		else 
			move.bearingOff = false;
		
		if (move.from == 0 && move.to == 0)
			break;
		moves.push_back(move);
	}

	string tmp;
	getline(cin, tmp);

	return moves;
}

string Player::toString() {
	if (color == RED) {
		return "Player Red";
	}
	else if (color == WHITE) {
		return "Player White";
	}

	return "";
}