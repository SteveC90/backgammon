#include <cstdlib>
#include "player.h"
#include "game.h"
#include "color.h"

using namespace std;

int main(int argc, char** argv) {
	Player p1(WHITE);
	Player p2(RED);
	Game bkgm(p1, p2); 

	if (argc > 1)
		bkgm.run(true);
	else
		bkgm.run();
}
