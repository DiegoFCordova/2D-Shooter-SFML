#pragma once

#include <iostream>
#include "Game.h"
#include "DT.h"



/*
 * Class that calls a Game instance
 * that takes care of simple parts of game.
 * 
 * @author Diego Cordova
 */

int main()
{
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init Game Engine
	Game game;
	DT d;

	//Main loop
	while (game.running())
	{
		//Update
		game.update();

		//Render
		game.render();

		d.updateClock();
	}

	return 0;
}