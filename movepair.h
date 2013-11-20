#pragma once

struct MovePair {
	int to;
	int from;
	MovePair(){}
	MovePair(int from, int to) {this->to = to; this->from = from;}
};