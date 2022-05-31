#include "bishop.h"

Bishop::Bishop(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	switch (color)
	{
	case 'W':
		sprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
		break;
	case 'B':
		sprite.setTextureRect(sf::IntRect(160, 80, 80, 80));
		break;
	}
}

void Bishop::generateLegalMoves()
{

}