#include "queen.h"

Queen::Queen(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	name = "queen";

	switch (color)
	{
		case 'W':
			sprite.setTextureRect(sf::IntRect(80, 0, 80, 80));
			break;
		case 'B':
			sprite.setTextureRect(sf::IntRect(80, 80, 80, 80));
			break;
	}
}

void Queen::generateLegalMoves()
{
	generateOrthogonalMoves();
	generateDiagonalMoves();
}