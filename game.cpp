#include "game.h"

Game::Game(Player &p1, Player &p2) {
	this->p1 = p1;
	this->p2 = p2;
	srand( time(NULL) );
}

void Game::run() {
	board.draw();
	vector<int> diceRoll = chooseFirstPlayer();

	while(true) {
		cout << currentPlayer->toString() << "'s turn" << endl;

		vector<MovePair> moves;
		do {
			cout << "Roll: ";
			for (int i=0; i<diceRoll.size(); ++i)
				cout << diceRoll[i] << " ";
			cout << endl;
			
			moves = currentPlayer -> move(diceRoll);
		} while (!isPlayValid(moves, diceRoll));

		// Move checkers
		for (int i = 0; i < moves.size(); ++i) {
			board.moveChecker(moves[i]);
		}

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

int Game::moveGenerator(vector<int> roll, Board board, vector<MovePair> currentMoves, const Color& color, int max, vector<MoveConfiguration> &all_plays) {
	if(roll.size()==0) {
		MoveConfiguration play(board, currentMoves);
		all_plays.push_back(play);
		return max;
	}

	int newMax = max;

	//iterate through the board from the farthest point from home
	for (int i=0; i<24; ++i) {
		//check at every stack if there is a white checker
		if (board.getCheckerCountAt(i) > 0 && board.getPlayerAt(i)==color) {
			for (int k=0; k<roll.size(); ++k) {
				//if that move is valid, do a recursive call with 
				// roll removed and new board
				if( isMoveValid(MovePair(i+1, i+1+roll[k]), board) ) {
					Board newBoard(board);
					vector<int> temp = roll;
					temp.erase(temp.begin()+k);
					MovePair p(i+1, i+1+roll[k]);
					newBoard.moveChecker(p);
					currentMoves.push_back(p);
					newMax = std::max(newMax, moveGenerator(temp, newBoard, currentMoves, color, max + 1, all_plays));
					currentMoves.pop_back();
				}
			}
		}
	}

	if(currentMoves.size() == newMax ) {
		MoveConfiguration play(board, currentMoves);
		all_plays.push_back(play);
	}
	return newMax;
}

bool Game::isPlayValid(vector<MovePair> moves, const vector<int>& diceRoll) {
	//generate all legal plays
	vector<MoveConfiguration> plays;
	vector<MovePair> v;
	moveGenerator(diceRoll, board, v, currentPlayer->getColor(), 0, plays);

	for (int i=0; i<plays.size(); ++i) {
		plays[i].board.draw();
	}

	cout<< "NUMBER OF LEGAL PLAYS " << plays.size() << endl;


	// Verify player moves are within valid stacks
	for (int i = 0; i < moves.size(); ++i) {
		if (moves[i].to > 25
				|| moves[i].from > 25
				|| moves[i].to <= 0
				|| moves[i].from <= 0) {
			return false;
		}
	}

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

bool Game::isMoveValid(const MovePair& move, const Board &board_state) {
	// Subtract 1 from to/from to get 0-based indices
	int fromIndex = move.from - 1;
	int toIndex = move.to - 1;

	int fromCheckerCount = board_state.getCheckerCountAt(fromIndex);
	int toCheckerCount = board_state.getCheckerCountAt(toIndex);

	Color fromPlayerColor = static_cast<Color>(board_state.getPlayerAt(fromIndex));
	Color toPlayerColor = static_cast<Color>(board_state.getPlayerAt(toIndex));

	// Moves must be within the bounds of the boards
	if (fromIndex < 0
			|| toIndex < 0
			|| fromIndex > 24
			|| toIndex > 23
			|| fromIndex == toIndex) {
		return false;
	}

	// Must move piece in the correct direction
	if (fromPlayerColor == RED && fromIndex < toIndex) {
		return false;
	}
	else if (fromPlayerColor == WHITE && fromIndex > toIndex) {
		return false;
	}

	// Can't move to a stack with 5 pieces
	if (toCheckerCount >= 5) {
		return false;
	}

	// Can't move a piece from an empty stack
	if (fromCheckerCount == 0) {
		return false;
	}

	// Can't move to a stack with 2 or more pieces of the other color
	if (fromPlayerColor != toPlayerColor && toCheckerCount >= 2) {
		return false;
	}

	return true;
}
