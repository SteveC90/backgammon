#pragma once
#include "board.h"
#include "movepair.h"

struct MoveConfiguration {
	Board board;
	vector<MovePair> moves;
	double moveEval;

	MoveConfiguration(Board board, vector<MovePair> moves) : board(board), moves(moves) { moveEval = 0; }
	MoveConfiguration() { moveEval = 0; }
	MoveConfiguration(const MoveConfiguration& copy) : board(copy.board), moves(copy.moves), moveEval(copy.moveEval) { }
};