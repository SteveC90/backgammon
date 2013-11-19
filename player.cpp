#include "player.h"

Player::Player() {
		
}

Player::Player(Color c) {
	this->color = c;
}

vector<MovePair> Player::move(vector<int> roll) {
	cout << "Enter moves as '<from #> <to #>'. Enter '0 0' to end turn: " << endl;
	vector<MovePair> moves;

	for (int i=0; i<roll.size(); ++i) {
		MovePair move;

		cin >> move.from >> move.to;
		if (move.from == 0 && move.to == 0)
			break;
		moves.push_back(move);
	}

	string tmp;
	getline(cin, tmp);

	return moves;
}