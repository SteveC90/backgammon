#include "game.h"

Game::Game(Player &p1, Player &p2) {
	debug = false;
	this->p1 = p1;
	this->p2 = p2;
	srand( time(NULL) );
}

void Game::run(bool debug) {
	this->debug = debug;

	board.draw();
	vector<int> diceRoll = chooseFirstPlayer();

	// Game loop
	// TODO: change while(true) condition so we can end the game
	bool playIsValid;
	while(true) {
		cout << currentPlayer->toString() << "'s turn" << endl;

		// Get valid moves from player
		vector<MovePair> moves;
		do {
			cout << "Roll: ";
			for (int i=0; i<diceRoll.size(); ++i)
				cout << diceRoll[i] << " ";
			cout << endl;
			
			moves = currentPlayer -> move(diceRoll);

			playIsValid = isPlayValid(moves, diceRoll);
			if (!playIsValid)
				cout << "Invalid moves. Re-enter move choices." << endl;
		} while (!playIsValid);

		// Move checkers
		for (int i = 0; i < moves.size(); ++i) {
			board.moveChecker(moves[i], currentPlayer->getColor());
		}

		cout << "current Player: "<< currentPlayer->toString() << endl;
		swapPlayer();

		cout << "Next Player: "<< currentPlayer->toString() << endl;
		diceRoll = roll();
		board.draw();
		cout << "\n\n";
	}
}

vector<int> Game::roll() {
	vector<int> rollVector;

	if (debug) {
		cout << "Enter rolls: ";
		int die1, die2;
		cin >> die1 >> die2;

		rollVector.push_back(die1);
		rollVector.push_back(die2);
	} else {
		rollVector.push_back(rand()%6 + 1);
		rollVector.push_back(rand()%6 + 1);
	}

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


int Game::moveGenerator(vector<int> roll, Board board, vector<MovePair> currentMoves, const Player* const player, int max, vector<MoveConfiguration> &all_plays) {
	const Color color = player->getColor();
	if(roll.size() == 0) {
		MoveConfiguration play(board, currentMoves);
		all_plays.push_back(play);
		return max;
	}

	int newMax = max;

	//if the player has a piece in the bar
	if(board.getCheckerCountOnBar(color) > 0) {
		for (int k=0; k<roll.size(); ++k) {

			int i = 0;
			if (color == RED) {
				i = -25;
			}
			i = abs(i + roll[k]);

			MovePair p(25, i);

			if( isMoveValid(p, board) ) {
				Board newBoard(board);
				vector<int> temp = roll;
				temp.erase(temp.begin() + k);
				newBoard.moveChecker(p, color);
				currentMoves.push_back(p);
				newMax = std::max(newMax, moveGenerator(temp, newBoard, currentMoves, player, max + 1, all_plays));
				currentMoves.pop_back();
			}

			// If roll is a double
			if(roll.size()>1 && roll[k]==roll[k+1]) break;
		}
	}
	// if the player is in a bearing off state
	else if (board.playerCanBearOff(player)) {
		//cout << "Bearing off state?"<<endl;
		int startingIndex = -5;
		int endIndex = 1;
		if (color == WHITE) {
			startingIndex = 18;
			endIndex = 24;
		}
		//loop through everything in the player's home board
		for (int i=startingIndex; i < endIndex; i++) {
			//cout << "Looping through home board" << endl;
			if (board.getPlayerAt(abs(i)) == color && board.getCheckerCountAt(abs(i)) > 0) {
				for (int k=0; k<roll.size(); ++k) {
					int j;
					if (color == WHITE)
						j = i+1;
					else 
						j = i-1;

					int distanceFromEdge = endIndex - i;
						//cout << "distanceFromEdge: "<<distanceFromEdge <<", roll[k]: "<<roll[k]<<endl;
					//not bearing off, but attempt to move closer to edge
					if (distanceFromEdge > roll[k]) {
						MovePair p(abs(j), abs(j+roll[k]));
						//cout << "Move Pair: " << abs(j) << " " << abs(j+roll[k]) <<endl;
						if( isMoveValid(p, board) ) {
							Board newBoard(board);
							vector<int> temp = roll;
							temp.erase(temp.begin()+k);
							newBoard.moveChecker(p);
							currentMoves.push_back(p);
							newMax = std::max(newMax, moveGenerator(temp, newBoard, currentMoves, player, max + 1, all_plays));
							currentMoves.pop_back();
						}
					}

					//bearing off from a stack index that is less than the roll
					if (distanceFromEdge < roll[k]) {
						//must check all previous stacks to see if they are clear
						bool canBearOff = true;
						for (int m = i-1; m > startingIndex; m--) {
							if (board.getPlayerAt(abs(m)) == color && board.getCheckerCountAt(abs(m)) > 0) {
								canBearOff = false;
								break;
							}
						}
						//bear off and do the recursive call
						if (canBearOff) {
							MovePair p(abs(j));
							//cout << "Bearing off from index > roll" <<endl;
							Board newBoard(board);
							vector<int> temp = roll;
							temp.erase(temp.begin()+k);
							newBoard.bearOff(abs(i));
							currentMoves.push_back(p);
							newMax = std::max(newMax, moveGenerator(temp, newBoard, currentMoves, player, max + 1, all_plays));
							currentMoves.pop_back();
						}
					}

					//bearing off from index == roll 
					if (distanceFromEdge == roll[k]) {
						MovePair p(abs(j));
						//cout << "Bearing off from roll index" <<endl;
						Board newBoard(board);
						vector<int> temp = roll;
						temp.erase(temp.begin()+k);
						newBoard.bearOff(abs(i));
						currentMoves.push_back(p);
						newMax = std::max(newMax, moveGenerator(temp, newBoard, currentMoves, player, max + 1, all_plays));
						currentMoves.pop_back();	
					}
					if(roll.size()>1 && roll[0]==roll[1]) break;

				}
			}
		}
	}
	//all other move types
	else {

		//iterate through the board from the farthest point from home
		for (int i=0; i<24; ++i) {
			// Check at every stack if there is a white checker
			if (board.getCheckerCountAt(i) > 0 && board.getPlayerAt(i) == color) {
				for (int k=0; k<roll.size(); ++k) {
					// If that move is valid, do a recursive call with 
					// roll removed and new board
					int j = i+1;
					if (color == RED) {
						j = -j;
					}
					MovePair p(abs(j), abs(j + roll[k]));
					if( isMoveValid(p, board) ) {
						Board newBoard(board);
						vector<int> temp = roll;
						temp.erase(temp.begin() + k);
						newBoard.moveChecker(p);
						currentMoves.push_back(p);
						newMax = std::max(newMax, moveGenerator(temp, newBoard, currentMoves, player, max + 1, all_plays));
						currentMoves.pop_back();
					}
					//if roll is a double
					if(roll.size() > 1 && roll[0] == roll[1]) break;
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
	int maxDiceCanUse = moveGenerator(diceRoll, board, v, currentPlayer, 0, plays);

	// for (int i=0; i<plays.size(); ++i) {
	// 	plays[i].board.draw();
	// }

	// cout<< "NUMBER OF LEGAL PLAYS " << plays.size() << endl;

	if (moves.size() < maxDiceCanUse) {
		return false;
	}

	for (int i = 0; i < plays.size(); ++i) {
		if (std::equal(moves.begin(), moves.end(), plays[i].moves.begin())) {
			cout << "This is a valid move!!!!!!"<<endl;
			return true;
		}
	}

	return false;
}

bool Game::isMoveValid(const MovePair& move, const Board &board_state) {
	// Subtract 1 from to/from to get 0-based indices
	int fromIndex = move.from - 1;
	int toIndex = move.to - 1;

	// Moves must be within the bounds of the boards
	if (fromIndex < 0
			|| toIndex < 0
			|| fromIndex > 24
			|| toIndex > 23
			|| fromIndex == toIndex) {
		return false;
	}

	// If trying to move a piece from the bar, assume there is a piece
	// The check for whether there actually is a piece on the bar is in the moveGenerator
	int fromCheckerCount = (fromIndex == 24) ? 1 : board_state.getCheckerCountAt(fromIndex);
	int toCheckerCount = board_state.getCheckerCountAt(toIndex);

	Color fromPlayerColor = (fromIndex == 24) ? NONE : static_cast<Color>(board_state.getPlayerAt(fromIndex));
	Color toPlayerColor = static_cast<Color>(board_state.getPlayerAt(toIndex));

	// Must move piece in the correct direction
	if (fromPlayerColor == RED && fromIndex < toIndex) {
		return false;
	}
	else if (fromPlayerColor == WHITE && fromIndex > toIndex && fromIndex != 24) {
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
