#include <cstdlib>
#include "player.h"
#include "game.h"
#include "color.h"

using namespace std;

int main() {
	Player p1(WHITE);
	Player p2(RED);
	Game bkgm(p1, p2); 

	bkgm.start();

}