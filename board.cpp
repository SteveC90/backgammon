#include "board.h"

Board::Board() {
	for(int i=0; i<25; ++i) {
		stacks[i][0] = 0;
		stacks[i][1] = 0;
	}

	stacks[0][0] = 2;
	stacks[0][1] = WHITE;

	stacks[5][0] = 5;
	stacks[5][1] = RED;

	stacks[7][0] = 3;
	stacks[7][1] = RED;

	stacks[11][0] = 5;
	stacks[11][1] = WHITE;

	stacks[12][0] = 5;
	stacks[12][1] = RED;

	stacks[16][0] = 3;
	stacks[16][1] = WHITE;

	stacks[18][0] = 5;
	stacks[18][1] = WHITE;

	stacks[23][0] = 2;
	stacks[23][1] = RED;
}

Board::Board(const Board& oldBoard) {
	for (int i=0; i<25; ++i) {
		stacks[i][0] = oldBoard.getCheckerCountAt(i);
		stacks[i][1] = oldBoard.getPlayerAt(i);
	}
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
					cout << "X";
				else
					cout << "O";
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
					cout << "X";
				else
					cout << "O";
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
			cout << "X";
		else
			cout << "O";
	}
	cout << endl;

	for (int i = 12 * columnWidth; i >= 1; --i)
		cout << "=";
	cout << endl;
}

int Board::getCheckerCountAt(int x) const {
	return stacks[x][0];
}

int Board::getPlayerAt(int x) const {
	return stacks[x][1];
}

//Doesn't do any validation that the move is legal
void Board::moveChecker(int from, int to) {
	// Make to/from 0-based to work as indices
	--from;
	--to;

	Color color = static_cast<Color>(getPlayerAt(from));
	stacks[from][0]--;
	if (getCheckerCountAt(from) == 0) {
		stacks[from][1] = 0; //no player occupies this space;
	}

	//if an enemy checker occupies that space
	if (getCheckerCountAt(to) == 1 && getPlayerAt(to) != color) {
		stacks[to][0] = 0;
		stacks[24][0]++;
		stacks[24][1] = getCheckerCountAt(to);
	}

	stacks[to][0]++;
	stacks[to][1] = color;
}