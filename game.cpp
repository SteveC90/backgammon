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

		vector<MovePair> moves;
		do {
			cout << "Roll: ";
			for (int i=0; i<diceRoll.size(); ++i)
				cout << diceRoll[i] << " ";
			cout << endl;
			
			moves = currentPlayer -> move(diceRoll);
		} while (!areValidMoves(moves, diceRoll));

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

void Game::moveGenerator(vector<int> roll, Board board) {
	if(roll.size()==0) {
		board.draw();
	}

	for (int i=0; i<24; ++i) {
		if (board.getCheckerCountAt(i) > 0 && board.getPlayerAt(i)==WHITE) {
			
		}
	}


}

bool Game::areValidMoves(vector<MovePair> moves, const vector<int>& diceRoll) {	
	// Ensure moves match dice rolls
	vector<int> diceRollCopy = diceRoll;
	for (int i = 0; i < moves.size(); ++i) {
		MovePair move = moves[i];
		int distanceMoved = move.to - move.from;

		vector<int>::iterator findIter = find(
			diceRollCopy.begin(), 
			diceRollCopy.end(), 
			distanceMoved);

		// Player tried to use a value they didn't roll
		if (findIter == diceRollCopy.end()) {
			return false;
		}

		// Moves copies of the item searched for to the end of the vector
		std::remove(
			diceRollCopy.begin(), 
			diceRollCopy.end(), 
			distanceMoved);

		// Removes last element of the vector
		diceRollCopy.erase(
			diceRollCopy.end() - 1,
			diceRollCopy.end());
	}
	
	// User didn't use all rolls
	// Check if more rolls could have been used
	if (moves.size() < diceRoll.size()) {

	}
	// Check validity of moves

	return true;
}
