#include "board.h"

Board::Board() {
	piecesRemaining[RED - 1] = 0;
	piecesRemaining[WHITE - 1] = 0;

	for(int i=0; i<25; ++i) {
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
}

Board::Board(const Board& oldBoard) {
	piecesRemaining[RED - 1] = 0;
	piecesRemaining[WHITE - 1] = 0;

	for (int i=0; i<25; ++i) {
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
	for (int i = 12 * columnWidth; i >= 1; --i)
		cout << "=";
	cout << endl;

	for(int i=12; i>0; --i) {
		cout.width(columnWidth);
		cout << i;
	}
	cout << endl;

	for (int row=1; row<=5; ++row) {
		for (int col=11; col>=0; --col) {
			cout.width(columnWidth);
			if (stacks[col][0] >= row) {
				if (stacks[col][1] == RED)
					cout << "R";
				else
					cout << "W";
			} else {
				cout << " ";
			}
		}
		cout << endl;
	}

	cout << endl;

	for (int row=5; row>=1; --row) {
		for (int col=12; col<=23; ++col) {
			cout.width(columnWidth);
			if (stacks[col][0] >= row) {
				if (stacks[col][1] == RED)
					cout << "R";
				else
					cout << "W";
			} else {
				cout << " ";
			}
		}
		cout << endl;
	}

	for(int i=13; i<=24; ++i) {
		cout.width(columnWidth);
		cout << i;
	}

	cout << endl;

	for (int i = 12 * columnWidth; i >= 1; --i)
		cout << "=";
	cout << endl;

	cout << "Bar: ";
	for (int i=0; i<stacks[24][0]; ++i) {
		if (stacks[24][1] == RED)
			cout << "R";
		else
			cout << "W";
	}
	cout << endl;

	for (int i = 12 * columnWidth; i >= 1; --i)
		cout << "=";
	cout << endl;
}

int Board::getCheckerCountAt(int x) const {
	return stacks[x][0];
}

Color Board::getPlayerAt(int x) const {
	return static_cast<Color>(stacks[x][1]);
}

//Doesn't do any validation that the move is legal
void Board::moveChecker(const MovePair& move) {
	// Make to/from 0-based to work as indices
	int fromIndex = move.from - 1;
	int toIndex = move.to - 1;

	Color color = static_cast<Color>(getPlayerAt(fromIndex));
	stacks[fromIndex][0]--;
	if (getCheckerCountAt(fromIndex) == 0) {
		stacks[fromIndex][1] = NONE; //no player occupies this space;
	}

	//if an enemy checker occupies that space
	if (getCheckerCountAt(toIndex) == 1 && getPlayerAt(toIndex) != color) {
		stacks[toIndex][0] = 0;
		stacks[24][0]++;
		stacks[24][1] = color;
	}

	stacks[toIndex][0]++;
	stacks[toIndex][1] = color;
}