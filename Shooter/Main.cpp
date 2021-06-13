#include <iostream>
#include "Game.h"

/*
 * Class to call ///Think on explanation for this
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