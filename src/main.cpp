#include "Game.h"
#include "main.h"

int main(int argc, char ** argv) {

	//Init game engine
	Game game;

	//Game loop
	while(game.isRunning())
	{
		game.updateDeltaTime();
		game.update();
		game.render();
	}

	return 0;
}