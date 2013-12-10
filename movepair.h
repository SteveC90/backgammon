#pragma once

struct MovePair {
	int to;
	int from;
	MovePair() {}
	MovePair(int from, int to) { this->to = to; this->from = from; }

	bool operator ==(const MovePair& other) const {
		return (this->to == other.to && this->from == other.from);
	}
};