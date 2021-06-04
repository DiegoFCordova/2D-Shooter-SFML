#include <iostream>
#include "Game.h"

//using namespace sf;	//Use to make code more neat later

int main()
{
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