#pragma once
#include <iostream>
#include "../../vendor/vendor.h"

class Game
{
public:
	static void run();
	static sf::RenderWindow window;
	static char winner;
private:
	static void init();
	static const sf::Vector2u RESOLUTION;
};
