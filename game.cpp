#include "game.h"

const float Game::pipProbabilities[24] = {
	0.3055, 0.3333, 0.3888, 0.4166,
	0.4166, 0.4722, 0.1666, 0.1666,
	0.1388, 0.0833, 0.0555, 0.0833,
	0,      0,      0.0277, 0.0277,
	0,      0.0277, 0,      0.0277,
	0,      0,      0,      0.0277
};

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
	while(board.getRemainingPieces(RED) > 0 && board.getRemainingPieces(WHITE) > 0) {
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

		swapPlayer();
		diceRoll = roll();
		board.draw();
		cout << "\n\n";
	}

	if (board.getRemainingPieces(p1.getColor()) == 0)
		cout << p1.toString() << " wins!!!\n";
	else 
		cout << p2.toString() << " wins!!!\n";
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

	// If maxDiceCanUse is 1, then remove configurations
	// that don't use the larger of the two rolls
	if (diceRoll.size() == 2 && maxDiceCanUse == 1) {
		vector<MoveConfiguration> maxDiePlays;
		for (int i = 0; i < plays.size(); ++i) {
			int maxDieRoll = max(diceRoll[0], diceRoll[1]);
			int dieUsedInPlay = abs(plays[i].moves[0].to - plays[i].moves[0].from);

			if (maxDieRoll == dieUsedInPlay) {
				maxDiePlays.push_back(plays[i]);
			}
		}

		// If there were any plays that use the larger roll,
		// replace the var 'plays' with that subset of plays
		if (maxDiePlays.size() > 0) {
			plays = maxDiePlays;
		}
	}

	// Remove plays that don't use the max possible dice
	for (int i = 0; i < plays.size(); ++i) {
		if (plays[i].moves.size() < maxDiceCanUse) {
			plays.erase(plays.begin() + i);
			--i;
		}
	}

	evaluatePlays(plays);

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

void Game::evaluatePlays(vector<MoveConfiguration> &Plays) const {
	for (int i = 0; i < Plays.size(); ++i) {
		float value = 0;		
		value += evaluateBlotDangerForColor(currentPlayer->getColor(), Plays[i]);
		value += evaluteBlockadingFactor(Plays[i]);

		Plays[i].moveEval = value;
	}
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

float Game::evaluateBlotDangerForColor(const Color& color, const MoveConfiguration& configuration) const {
	float value = 0;
	const float maxValue = 3.1656;
	const Board &board = configuration.board;
	const int directionToEnemy = (color == WHITE) ? 1 : -1;
	const Color enemyColor = (color == WHITE) ? RED : WHITE;

	// Iterate through board
	for (int stackIndex = 0; stackIndex < 24; ++stackIndex) {
		// Found a blot
		if (board.getPlayerAt(stackIndex) == color
				&& board.getCheckerCountAt(stackIndex) == 1) {
			// Iterate through enemy positions
			for (int enemyStackIndex = stackIndex + directionToEnemy;
					enemyStackIndex < 24 && enemyStackIndex >= 0;
					enemyStackIndex += directionToEnemy) {
				// Found blot danger
				// Add probability of being hit to value
				if (board.getPlayerAt(enemyStackIndex) == enemyColor
						&& board.getCheckerCountAt(enemyStackIndex) > 0) {
					value += Game::pipProbabilities[abs(enemyStackIndex - stackIndex - 1)];
				}
			}
		}
	}

	return (value / maxValue);
}

float Game::evaluteBlockadingFactor(const MoveConfiguration& configuration) const {
	//iterate trough all legal boards
	Color color = currentPlayer->getColor();
	Color opponent = RED;
	if (color == RED)
		opponent = WHITE;

	double evalSum = 0;
	Board tempBoard = configuration.board;
	int opponentPieces = 0;

	int end = 24;
	int j = 0;
	//a count for the contiguous block
	int contiguousBlock = 0;
	//keeps track of the last checker encountered in a block
	int lastBlockingChecker = 0;
	if (color == RED) {
		lastBlockingChecker = -23;
		end = 1;
		j = -23;
	}
	for ( ; j < end; ++j) {
		if (tempBoard.getPlayerAt(abs(j)) == color && tempBoard.getCheckerCountAt(abs(j)) > 1) {
			++contiguousBlock;
			//lastBlockingChecker = abs(j);
		} else {
			double eval = 0;

			//a contiguous block of 7 or more is not any more effective than 6
			if (contiguousBlock > 6) {
				eval += 36;
			} else {
				eval += contiguousBlock * contiguousBlock;
			}

			/*if ( contiguousBlock > 0 && abs(lastBlockingChecker - abs(j) + contiguousBlock) > 0) {
				eval /= abs(lastBlockingChecker - abs(j) + contiguousBlock) + 1;
				contiguousBlock = 0;
				lastBlockingChecker = j-1;
			}*/

			eval *= ((double) (tempBoard.getRemainingPieces(opponent) - opponentPieces)) /  tempBoard.getRemainingPieces(opponent);
			if (tempBoard.getPlayerAt(abs(j)) == opponent) {
				opponentPieces += tempBoard.getCheckerCountAt(abs(j));
			}

			/*if (contiguousBlock > 0) {
				//cout << "Opponents: " << opponentPieces << endl;
				cout << "Contiguous block of: " << contiguousBlock << ", at: " << abs(j) + 1 << ", Payoff: " << eval/37 << endl;
			}*/
			contiguousBlock = 0;
			evalSum += eval/37;

		}
	}

	return evalSum;
}