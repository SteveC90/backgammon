#include "ai.h"

const float Ai::pipProbabilities[24] = {
	0.3055, 0.3333, 0.3888, 0.4166,
	0.4166, 0.4722, 0.1666, 0.1666,
	0.1388, 0.0833, 0.0555, 0.0833,
	0,      0,      0.0277, 0.0277,
	0,      0.0277, 0,      0.0277,
	0,      0,      0,      0.0277
};

vector<MovePair> Ai::move(vector<int> roll, vector<MoveConfiguration>& plays) {
	return plays[evaluatePlays(plays)].moves;
}

int Ai::evaluatePlays(vector<MoveConfiguration> &Plays) const {
	int maxIndex;
	float maxEvalSeen = -1000;
	for (int i = 0; i < Plays.size(); ++i) {
		float value = 0;	
		value += evaluateBlotDanger(Plays[i]);
		value += evaluteBlockadingFactor(Plays[i]);
		value += evaluateBarFactor(Plays[i]);

		Plays[i].moveEval = value;
		if (value > maxEvalSeen) {
			maxEvalSeen = value;
			maxIndex = i;
		}
	}

	cout << "Blot Danger: " << evaluateBlotDanger(Plays[maxIndex]) << endl;
	cout << "Blockading Factor: " << evaluteBlockadingFactor(Plays[maxIndex]) << endl;
	cout << "Bar Factor: " << evaluateBarFactor(Plays[maxIndex]) <<endl;

	return maxIndex;
}

float Ai::evaluateBlotDanger(const MoveConfiguration& configuration) const {
	float value = 0;
	const float maxValue = 3.1656;
	const Board &board = configuration.board;
	const int directionToEnemy = (color == WHITE) ? 1 : -1;
	const Color enemyColor = (color == WHITE) ? RED : WHITE;

	int blotCount = 0;
	// Iterate through board
	for (int stackIndex = 0; stackIndex < 24; ++stackIndex) {
		// Found a blot
		if (board.getPlayerAt(stackIndex) == color
				&& board.getCheckerCountAt(stackIndex) == 1) {
			++blotCount;
			// Iterate through enemy positions
			for (int enemyStackIndex = stackIndex + directionToEnemy;
					enemyStackIndex < 24 && enemyStackIndex >= 0;
					enemyStackIndex += directionToEnemy) {
				// Found blot danger
				// Add probability of being hit to value
				if (board.getPlayerAt(enemyStackIndex) == enemyColor
						&& board.getCheckerCountAt(enemyStackIndex) > 0) {
					value += pipProbabilities[abs(enemyStackIndex - stackIndex - 1)];
				}
			}
		}
	}

	return (blotCount == 0) ? 0 : -(value / (maxValue * blotCount));
}

float Ai::evaluteBlockadingFactor(const MoveConfiguration& configuration) const {
	//iterate trough all legal boards
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

float Ai::evaluateBarFactor(const MoveConfiguration& configuration) const {
	Color opponentColor = RED;
	int homeStart = 18;
	int homeEnd = 24;

	if (color == RED) {
		homeStart = -5;
		homeEnd = 1;
		opponentColor = WHITE;
	}

	Board tempBoard = configuration.board;

	int numOnBar = tempBoard.getCheckerCountOnBar(opponentColor);

	if (numOnBar == 0) {
		return 0;
	}

	int sum = numOnBar;

	for (int j = homeStart; j < homeEnd; ++j) {
		if (tempBoard.getPlayerAt(abs(j)) == color) {

			if (tempBoard.getCheckerCountAt(abs(j)) > 1) {
				sum++;
			} else if (tempBoard.getCheckerCountAt(abs(j)) == 1) {
				sum--;
			}
		}
	}
	return (float)sum / 6;

}