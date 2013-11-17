#include "game.h"

Game::Game(Player &p1, Player &p2) {
	this->p1 = p1;
	this->p2 = p2;
	srand( time(NULL) );
}

void Game::start() {
	board.draw();
	vector<int> diceRoll = chooseFirstPlayer();

	while(true) {
		cout << "Player " << currentPlayer->getColor() << "'s turn" << endl;
		cout << "Roll: ";
		for (int i=0; i<diceRoll.size(); ++i)
			cout << diceRoll[i] << " ";
		cout << endl;

		vector<MovePair> moves;
		do {
			moves = currentPlayer -> move(diceRoll);
		} while (!areValidMoves(moves));

		swapPlayer();
		diceRoll = roll();
		board.draw();
		cout << "\n\n";
	}
}

vector<int> Game::roll() {
	vector<int> rollVector;

	rollVector.push_back(rand()%6 + 1);
	rollVector.push_back(rand()%6 + 1);

	if(rollVector[0] == rollVector[1]) {
		rollVector.push_back(rollVector[0]);
		rollVector.push_back(rollVector[0]);
	}

	return rollVector;
}

vector<int> Game::chooseFirstPlayer() {
	vector<int> result;
	do {
		result = roll();

		if(result[0] < result[1]) {
			currentPlayer = &p1;
		} else {
			currentPlayer = &p2;
		}
	} while (result[0] == result[1]);

	return result;
}

void Game::swapPlayer() {
	if ( currentPlayer->getColor() == p1.getColor() )
		currentPlayer = &p2;
	else 
		currentPlayer = &p1;
}

bool Game::areValidMoves(vector<MovePair> moves) {

}
