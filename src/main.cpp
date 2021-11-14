#include "Game.h"
#include "PoPossibEngin.h"
#include "main.h"

int main(int argc, char ** argv) {

	////Init game engine
	//Game game;

	////Game loop
	//while(game.isRunning())
	//{
	//	game.updateDeltaTime();
	//	game.update();
	//	game.render();
	//}

	PoPossibEngin poPossibEngin;

	poPossibEngin.run();

	return 0;
}