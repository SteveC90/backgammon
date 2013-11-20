#pragma once
#include "board.h"
#include "movepair.h"

struct MoveConfiguration {
	Board board;
	MovePair move;

	MoveConfiguration(Board board, MovePair move) : board(board), move(move) {}
};