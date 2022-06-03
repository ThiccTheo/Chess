#include "king.h"
#include "../../board/board.h"

King::King(const int X, const int Y, const char COLOR)
{
	constructor(X, Y, COLOR);

	name = "king";

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
	auto [x, y] = indices;

	//north
	if (y - 1 >= 0)
	{
		legalMoves.emplace_back(x, y - 1);
	}

	//south
	if (y + 1 <= 7)
	{
		legalMoves.emplace_back(x, y + 1);
	}

	//east
	if (x + 1 <= 7)
	{
		legalMoves.emplace_back(x + 1, y);
	}

	//west
	if (x - 1 >= 0)
	{
		legalMoves.emplace_back(x - 1, y);
	}

	//northeast
	if (y - 1 >= 0 && x + 1 <= 7)
	{
		legalMoves.emplace_back(x + 1, y - 1);
	}

	//northwest
	if (y - 1 >= 0 && x - 1 >= 0)
	{
		legalMoves.emplace_back(x - 1, y - 1);
	}

	//southeast
	if (y + 1 <= 7 && x + 1 <= 7)
	{
		legalMoves.emplace_back(x + 1, y + 1);
	}

	//southwest
	if (y + 1 <= 7 && x - 1 >= 0)
	{
		legalMoves.emplace_back(x - 1, y + 1);
	}

	//castling
	if (onSpawnTile)
	{
		bool canCastleLong{ true };
		bool canCastleShort{ true };

		for (const auto& PIECE : pieces)
		{
			if (PIECE->indices != indices)
			{
				if (PIECE->indices.y == y)
				{
					//castling short
					if (x + 1 == PIECE->indices.x || x + 2 == PIECE->indices.x)
					{
						canCastleShort = false;
					}
					//castling long
					if (x - 1 == PIECE->indices.x || x - 2 == PIECE->indices.x || x - 3 == PIECE->indices.x)
					{
						canCastleLong = false;
					}
				}
				/*if squares for castling are in check
				if (PIECE->color != color)
				{
					PIECE->validateLegalMoves();

					for (const auto& LEGAL_MOVE : PIECE->legalMoves)
					{
						//castling short
						if (x + 1 == LEGAL_MOVE.x || x + 2 == LEGAL_MOVE.x)
						{
							canCastleShort = false;
						}
						//castling long
						if (x - 1 == LEGAL_MOVE.x || x - 2 == LEGAL_MOVE.x || x - 3 == LEGAL_MOVE.x)
						{
							canCastleLong = false;
						}
					}
				}*/
			}
		}
		for (const auto& PIECE : pieces)
		{
			if (PIECE->color == color && PIECE->name == "rook" && PIECE->onSpawnTile)
			{
				//castling short
				if (PIECE->indices.x == 7 && canCastleShort)
				{
					legalMoves.emplace_back(x + 2, y);
				}

				//castling long
				if (PIECE->indices.x == 0 && canCastleLong)
				{
					legalMoves.emplace_back(x - 2, y);
				}
			}
		}
	}
}