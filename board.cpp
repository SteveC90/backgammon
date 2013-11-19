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

void Board::draw() {
	for(int i=12; i>0; --i) {
		cout.width(3);
		cout << i;
	}
	cout << endl;

	for (int row=1; row<=5; ++row) {
		for (int col=11; col>=0; --col) {
			cout.width(3);
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
			cout.width(3);
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
		cout.width(3);
		cout << i;
	}

	cout << endl;

	cout << "Bar: ";
	for (int i=0; i<stacks[24][0]; ++i) {
		if (stacks[24][1] == RED)
			cout << "X";
		else
			cout << "O";
	}
	cout << endl;
}

int Board::getCheckerCountAt(int x) {
	return stacks[x][0];
}

int Board::getPlayerAt(int x) {
	return stacks[x][1];
}