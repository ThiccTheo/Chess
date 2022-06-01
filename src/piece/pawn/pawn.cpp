#include "pawn.h"

Pawn::Pawn(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	switch (color)
	{
		case 'W':
			sprite.setTextureRect(sf::IntRect(400, 0, 80, 80));
			break;
		case 'B':
			sprite.setTextureRect(sf::IntRect(400, 80, 80, 80));
			break;
	}
}

void Pawn::generateLegalMoves()
{
	legalMoves.emplace_back(2, 3);
}