#include <iostream>
#include "Game.h"

/*
 * Class that calls a Game instance
 * that takes care of simple parts of game.
 * 
 * @author MellamoSteve
 */

int main()
{
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init Game Engine
	Game game;

	//Main loop
	while (game.running())
	{
		//Update
		game.update();

		//Render
		game.render();
	}

	return 0;
}