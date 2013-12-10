#pragma once

struct MovePair {
	//these ints are not indexes in the board.
	int to;
	int from;
	bool bearingOff;
	MovePair() {}
	MovePair(int from, int to) {this->to = to; this->from = from; this->bearingOff = false;}
	MovePair(int from) {this->from = from; this->bearingOff = true;}

	bool operator ==(const MovePair& other) const {
		if (this->bearingOff == true && other.bearingOff == true) {
			return (this->from == other.from);
		}
		return (this->to == other.to && this->from == other.from);
	}
};