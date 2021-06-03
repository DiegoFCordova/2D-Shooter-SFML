#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main()
{
	//Window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Demo");
	sf::Event event;

	//Main loop
	while (window.isOpen())
	{
		//Event polls
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			}
		}

		//Update

		//Render
		window.clear(sf::Color(30, 30, 30));	//Get rid of old frame

		//Draw

		window.display();	//Display finished frame
	}

	return 0;
}