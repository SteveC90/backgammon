#pragma once
#include "board.h"
#include "movepair.h"

struct MoveConfiguration {
	Board board;
	vector<MovePair> moves;

	MoveConfiguration(Board board, vector<MovePair> moves) : board(board), moves(moves) {}
};