#include "DT.h"

float DT::dt = 0, DT::mult = 60;

/* Updates clock */
void DT::updateClock()
{
	dt = (clock.restart().asSeconds());
	std::cout << "\ndt: " << dt;
}
