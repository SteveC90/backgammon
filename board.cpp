#include "board.h"

Board::Board() {
	piecesRemaining[RED - 1] = 0;
	piecesRemaining[WHITE - 1] = 0;

	bar[0] = 0;
	bar[1] = 0;

	for(int i=0; i<24; ++i) {
		fillStack(i, 0, NONE);
	}

	fillStack(0, 2, WHITE);
	fillStack(5, 5, RED);
	fillStack(7, 3, RED);
	fillStack(11, 5, WHITE);
	fillStack(12, 5, RED);
	fillStack(16, 3, WHITE);
	fillStack(18, 5, WHITE);
	fillStack(23, 2, RED);

/*	fillStack(22, 4, WHITE);
	fillStack(21, 3, WHITE);
	fillStack(20, 2, WHITE);

	fillStack(3, 3, RED);
	fillStack(2, 2, RED);*/

	/*fillStack(0, 1, WHITE);
	fillStack(1, 2, RED);*/
}

Board::Board(const Board& oldBoard) {
	piecesRemaining[RED - 1] = 0;
	piecesRemaining[WHITE - 1] = 0;

	bar[0] = oldBoard.getCheckerCountOnBar(RED);
	bar[1] = oldBoard.getCheckerCountOnBar(WHITE);

	for (int i=0; i<24; ++i) {
		fillStack(i, oldBoard.getCheckerCountAt(i), oldBoard.getPlayerAt(i));
	}
}

bool Board::playerCanBearOff(const Player* const player) const {
	int piecesCount = 0;
	int startingIndex = 0;
	Color color = player->getColor();

	// Make sure we're checking the right home area
	if (color == WHITE) {
		startingIndex = 18;
	}
	else if (color == NONE) {
		return false;
	}

	// Count pieces in home area
	for (int i = startingIndex; i < startingIndex + 6; ++i) {
		if (stacks[i][1] == color) {
			piecesCount += stacks[i][0];
		}
	}

	cout << "Home count: " << piecesCount << endl;

	// If all pieces are in home area we can bear off
	if (piecesCount == piecesRemaining[color - 1]) {
		return true;
	}

	return false;
}

void Board::fillStack(const int stackIndex, const int piecesCount, const Color color) {
	if (piecesCount > 5 || piecesCount < 0) {
		throw 1;
	}

	stacks[stackIndex][0] = piecesCount;
	stacks[stackIndex][1] = color;
	piecesRemaining[color - 1] += piecesCount;
}

void Board::draw() const {
	// Top of board
	for (int i = 13 * columnWidth; i >= 1; --i) {
		cout << "=";
	}
	cout << endl;

	// First row of numbers
	for (int i = 12; i >= 7; --i) {
		cout.width(columnWidth);
		cout << i;
	}
	cout.width(columnWidth);
	cout << "||";
	for (int i = 6; i >= 1; --i) {
		cout.width(columnWidth);
		cout << i;
	}
	cout << endl;

	// Print pieces for top rows
	for (int row = 1; row <= 5; ++row) {
		for (int col = 12; col >= 1; --col) {
			cout.width(columnWidth);

			if (col == 6) {
				cout << "||";
				cout.width(columnWidth);
			}

			if (stacks[col - 1][0] >= row) {
				if (stacks[col - 1][1] == RED)
					cout << "R";
				else
					cout << "W";
			} else {
				cout << " ";
			}
		}
		cout << endl;
	}

	// Middle row
	for (int i = 12; i >= 1; --i) {
		cout.width(columnWidth);

		if (i == 6) {
			cout << "||";
		}
		else {
			cout << " ";
		}
	}
	cout << endl;

	// Print pieces for bottom rows
	for (int row = 5; row >= 1; --row) {
		for (int col = 13; col <= 24; ++col) {
			cout.width(columnWidth);

			if (col == 19) {
				cout << "||";
				cout.width(columnWidth);
			}

			if (stacks[col - 1][0] >= row) {
				if (stacks[col - 1][1] == RED)
					cout << "R";
				else
					cout << "W";
			} else {
				cout << " ";
			}
		}
		cout << endl;
	}

	// Second row of numbers
	for (int i = 13; i <= 18; ++i) {
		cout.width(columnWidth);
		cout << i;
	}
	cout.width(columnWidth);
	cout << "||";
	for (int i = 19; i <= 24; ++i) {
		cout.width(columnWidth);
		cout << i;
	}
	cout << endl;

	// Bottom of board
	for (int i = 13 * columnWidth; i >= 1; --i) {
		cout << "=";
	}
	cout << endl;

	// Print bar
	cout << "Bar: ";
	if (bar[1] > 0) {
		for (int i = 0; i < bar[1]; ++i) {
			cout << "W";
		}

		if (bar[0] > 0) {
			cout << endl;
			cout << "Bar: ";
		}
	}
	for (int i = 0; i < bar[0]; ++i) {
		cout << "R";
	}
	cout << endl;

	// Bottom of output
	for (int i = 13 * columnWidth; i >= 1; --i) {
		cout << "=";
	}
	cout << endl;
}

int Board::getCheckerCountAt(int x) const {
	return stacks[x][0];
}

Color Board::getPlayerAt(int x) const {
	return static_cast<Color>(stacks[x][1]);
}

int Board::getCheckerCountOnBar(const Color color) const {
	if (color == RED) {
		return bar[0];
	}
	else if (color == WHITE) {
		return bar[1];
	}

	return 0;
}

// Doesn't do any validation that the move is legal
void Board::moveChecker(const MovePair& move, Color color) {
	if(move.bearingOff) {
		bearOff(move.from-1);
		return;
	}

	// Make to/from 0-based to work as indices
	int fromIndex = move.from - 1;
	int toIndex = move.to - 1;

	// If no color was specified, assume the fromIndex has the color
	if (fromIndex < 24 && fromIndex >= 0)
		color = static_cast<Color>(getPlayerAt(fromIndex));

	if (fromIndex == 24) {
		if (color == RED) {
			--bar[0];
		}
		else if (color == WHITE) {
			--bar[1];
		}
		else {
			// Shouldn't get here unless you passed in a
			// color other than red or white
			throw 1;
		}
	}
	else {
		--stacks[fromIndex][0];
		if (getCheckerCountAt(fromIndex) == 0) {
			stacks[fromIndex][1] = NONE; //no player occupies this space;
		}
	}

	// If an enemy checker occupies that space...
	Color enemyColor = getPlayerAt(toIndex);
	if (getCheckerCountAt(toIndex) == 1 && enemyColor != color) {
		stacks[toIndex][0] = 0;

		if (enemyColor == RED) {
			++bar[0];
		}
		else if (enemyColor == WHITE) {
			++bar[1];
		}
	}

	// cout << "BAR RED: " << bar[0] << endl;
	// cout << "BAR WHITE: " << bar[1] << endl << endl;

	stacks[toIndex][0]++;
	stacks[toIndex][1] = color;
}

void Board::bearOff(int index) {
	Color color = static_cast<Color>(getPlayerAt(index));
	piecesRemaining[color-1]--;
	stacks[index][0]--;
	if (getCheckerCountAt(index) == 0) {
		stacks[index][1] = NONE; //no player occupies this space;
	}
}

int Board::getRemainingPieces(const Color c) const {
	return piecesRemaining[c-1];
}