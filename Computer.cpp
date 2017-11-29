#include "Computer.h"
#include <stdio.h>
#include <vector>

Computer::Computer(Logic& l, Board& b, Side s):Player(l, b, s) {}

int Computer::moveValue(Move * move) const {
	int i, cunt = 1;
	// go-over every direction, and sum the number of blocks that will change
	for (i = 0; i < 8; i++) {
		cunt = cunt + move->getDir(i);
	}
	return cunt;
}

int Computer::checkNext(Move * move) const {
	Board bnext(b);
	vector<Move*> enmopts;
	// check the enemy side, then update the copied board and check for enemy moves
	if (s == BLACK) {
		bnext.update(WHITE, move);
		enmopts = l.allowedActions(bnext, WHITE);
	} else {
		bnext.update(BLACK, move);
		enmopts = l.allowedActions(bnext, BLACK);
	}
	// if no possible move was found - return 0
	if (enmopts.size() == 0) {
		return 0;
	}
	int max = 0, current = 0, i, j;
	// go over every possible enemy move
	for (i = 0; i < enmopts.size(); i++) {
		// check how many blocks will change
		current = moveValue(enmopts[i]);
		// check if the value is bigger than max value
		if (current > max)
			max = current;
		// free memory
		delete enmopts[i];
	}
	return max;
}

bool Computer::doMove() {
	vector<Move*> options;
	// check possible moves
	options = l.allowedActions(b, s);
	if (options.size() == 0) {
		cout << "No possible moves, turn pass" << endl;
		return false;
	}
	int i, min = -1, current;
	Move * best = NULL;
	// go over every possible move
	for (i = 0; i < options.size(); i++) {
		// check its value to the enemy
		current = checkNext(options[i]);
		// check if this is the first move, or if its value smaller than min
		if ((best == NULL) || (current < min)) {
			// save the current move as the new best, and its value in min
			min = current;
			best = options[i];
		}
	}
	// change the board whith the chosen move
	b.update(s, best);
	// print the chosen move
	cout << "Enemy played ";
	best->pMove();
	cout << endl;
	// free memory
	for (i = 0; i < options.size(); i++)
		delete options[i];
	return true;
}
