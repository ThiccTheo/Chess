#include "king.h"

King::King(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	switch (color)
	{
	case 'W':
		sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
		break;
	case 'B':
		sprite.setTextureRect(sf::IntRect(0, 80, 80, 80));
		break;
	}
}

void King::generateLegalMoves()
{
}