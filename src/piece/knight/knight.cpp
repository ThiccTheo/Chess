#include "knight.h"

Knight::Knight(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	switch (color)
	{
	case 'W':
		sprite.setTextureRect(sf::IntRect(240, 0, 80, 80));
		break;
	case 'B':
		sprite.setTextureRect(sf::IntRect(240, 80, 80, 80));
		break;
	}
}

void Knight::generateLegalMoves()
{
	//northeast
	auto& [x, y] = indices;

	if ((x + 1) + ((y - 2) * 8) >= 0 && (x + 1) + ((y - 2) * 8) <= 63 && x < 7)
	{
		legalMoves.emplace_back(x + 1, y - 2);
	}

	//northwest
	if ((x - 1) + ((y - 2) * 8) >= 0 && (x - 1) + ((y - 2) * 8) <= 63 && x > 0)
	{
		legalMoves.emplace_back(x - 1, y - 2);
	}

	//southeast
	if ((x + 1) + ((y + 2) * 8) >= 0 && (x + 1) + ((y + 2) * 8) <= 63 && x < 7)
	{
		legalMoves.emplace_back(x + 1, y + 2);
	}

	//southwest
	if ((x - 1) + ((y + 2) * 8) >= 0 && (x - 1) + ((y + 2) * 8) <= 63 && x > 0)
	{
		legalMoves.emplace_back(x - 1, y + 2);
	}

	//eastnorth
	if ((x + 2) + ((y - 1) * 8) >= 0 && (x + 2) + ((y - 1) * 8) <= 63 && x < 6)
	{
		legalMoves.emplace_back(x + 2, y - 1);
	}

	//eastsouth
	if ((x + 2) + ((y + 1) * 8) >= 0 && (x + 2) + ((y + 1) * 8) <= 63 && x < 6)
	{
		legalMoves.emplace_back(x + 2, y + 1);
	}

	//westnorth
	if ((x - 2) + ((y - 1) * 8) >= 0 && (x - 2) + ((y - 1) * 8) <= 63 && x > 1)
	{
		legalMoves.emplace_back(x - 2, y - 1);
	}

	//westsouth
	if ((x - 2) + ((y + 1) * 8) >= 0 && (x - 2) + ((y + 1) * 8) <= 63 && x > 1)
	{
		legalMoves.emplace_back(x - 2, y + 1);
	}
}