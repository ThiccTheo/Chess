#pragma once
#include <iostream>
#include "../../vendor/vendor.h"

class Game
{
public:
	static void run();
	static sf::RenderWindow window;
	static char winner;
	static const sf::Vector2u RESOLUTION;
private:
	static void init();
};
