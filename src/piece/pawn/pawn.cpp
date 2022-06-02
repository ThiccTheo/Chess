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

	canBeEnPassantEd = false;
	isPawn = true;
}

Pawn::Pawn() = default;

void Pawn::generateLegalMoves()
{	
	bool canMove1Space{ true };
	bool canMove2Spaces{ onSpawnTile ? true : false };

	for (const auto& PIECE : pieces)
	{
		//single pawn move
		if (color == 'W' && indices.y - 1 >= 0 && indices.x == PIECE->indices.x && indices.y - 1 == PIECE->indices.y)
		{
			canMove1Space = false;
			canMove2Spaces = false;
		}
		else if (color == 'B' && indices.y + 1 <= 7 && indices.x == PIECE->indices.x && indices.y + 1 == PIECE->indices.y)
		{
			canMove1Space = false;
			canMove2Spaces = false;
		}

		//double pawn move
		if (color == 'W' && canMove1Space && indices.y - 2 >= 0 && indices.x == PIECE->indices.x && indices.y - 2 == PIECE->indices.y)
		{
			std::cout << "beep\n";
			canMove2Spaces = false;
		}
		else if (color == 'B' && canMove1Space && indices.y + 2 <= 7 && indices.x == PIECE->indices.x && indices.y + 2 == PIECE->indices.y)
		{
			canMove2Spaces = false;
		}

		//diagonal capture
		if (color == 'W')
		{
			if (indices.y - 1 >= 0 && indices.x - 1 >= 0)
			{
				sf::Vector2i pos{ indices.x - 1, indices.y - 1 };
				if (PIECE->indices == pos && PIECE->color == 'B')
				{
					legalMoves.emplace_back(pos);
				}
			}
			if (indices.y - 1 >= 0 && indices.x - 1 <= 7)
			{
				sf::Vector2i pos{ indices.x + 1, indices.y - 1 };
				if (PIECE->indices == pos && PIECE->color == 'B')
				{
					legalMoves.emplace_back(pos);
				}
			}
		}
		else if (color == 'B')
		{
			if (indices.y + 1 <= 7 && indices.x - 1 >= 0)
			{
				sf::Vector2i pos{ indices.x - 1, indices.y + 1 };
				if (PIECE->indices == pos && PIECE->color == 'W')
				{
					legalMoves.emplace_back(pos);
				}
			}
			if (indices.y + 1 <= 7 && indices.x - 1 <= 7)
			{
				sf::Vector2i pos{ indices.x + 1, indices.y + 1 };
				if (PIECE->indices == pos && PIECE->color == 'W')
				{
					legalMoves.emplace_back(pos);
				}
			}
		}

		//en passant
		if (color == 'W' && indices.y - 1 >= 0)
		{
			if (indices.x + 1 <= 7 && PIECE->indices.x == indices.x + 1 && PIECE->isPawn && PIECE->canBeEnPassantEd && enPassantMoveCounter == 0 && PIECE->color == 'B' && PIECE->indices.y == indices.y)
			{
				legalMoves.emplace_back(indices.x + 1, indices.y - 1);
			}
			if (indices.x - 1 >= 0 && PIECE->indices.x == indices.x - 1 && PIECE->isPawn && PIECE->canBeEnPassantEd && enPassantMoveCounter == 0 && PIECE->color == 'B' && PIECE->indices.y == indices.y)
			{
				legalMoves.emplace_back(indices.x - 1, indices.y - 1);
			}
		}
		else if (color == 'B' && indices.y + 1 <= 7)
		{
			if (indices.x + 1 <= 7 && PIECE->indices.x == indices.x + 1 && PIECE->isPawn && PIECE->canBeEnPassantEd && enPassantMoveCounter == 0 && PIECE->color == 'W' && PIECE->indices.y == indices.y)
			{
				legalMoves.emplace_back(indices.x + 1, indices.y + 1);
			}
			if (indices.x - 1 >= 0 && PIECE->indices.x == indices.x - 1 && PIECE->isPawn && PIECE->canBeEnPassantEd && enPassantMoveCounter == 0 && PIECE->color == 'W' && PIECE->indices.y == indices.y)
			{
				legalMoves.emplace_back(indices.x - 1, indices.y + 1);
			}
		}

	}

	if (canMove1Space)
	{
		switch (color)
		{
		case 'W':
			legalMoves.emplace_back(indices.x, indices.y - 1);
			break;
		case 'B':
			legalMoves.emplace_back(indices.x, indices.y + 1);
			break;
		}
	}
	if (canMove2Spaces)
	{
		switch (color)
		{
		case 'W':
			legalMoves.emplace_back(indices.x, indices.y - 2);
			break;
		case 'B':
			legalMoves.emplace_back(indices.x, indices.y + 2);
			break;
		}
	}
}