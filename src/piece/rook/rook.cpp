#include "rook.h"

Rook::Rook(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	switch (color)
	{
		case 'W':
			sprite.setTextureRect(sf::IntRect(320, 0, 80, 80));
			break;
		case 'B':
			sprite.setTextureRect(sf::IntRect(320, 80, 80, 80));
			break;
	}
}

void Rook::generateLegalMoves()
{

}