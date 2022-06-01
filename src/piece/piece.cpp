#include "piece.h"
#include "../game/game.h"
#include "knight/knight.h"
#include "pawn/pawn.h"
#include "king/king.h"
#include "queen/queen.h"
#include "rook/rook.h"
#include "bishop/bishop.h"
#include "../board/board.h"

char Piece::colorToMove;
const float Piece::SPRITE_SIZE{ 80.f };
sf::Texture Piece::texture;
std::vector<std::unique_ptr<Piece>> Piece::pieces;

const bool Piece::LOAD()
{
	if (texture.loadFromFile("src/piece/spritesheet.png"))
	{
		texture.setSmooth(true);
		return true;
	}
	else
	{
		return false;
	}
}

void Piece::init(const std::string& FEN)
{
	int file{ 0 };
	int rank{ 0 };

	colorToMove = 'W';

	for (const auto& POS : FEN)
	{
		switch (POS)
		{
			case 'r': pieces.emplace_back(new Rook(rank, file, 'B')); rank++; break;
			case 'R': pieces.emplace_back(new Rook(rank, file, 'W')); rank++; break;
			case 'n': pieces.emplace_back(new Knight(rank, file, 'B')); rank++; break;
			case 'N': pieces.emplace_back(new Knight(rank, file, 'W')); rank++; break;
			case 'b': pieces.emplace_back(new Bishop(rank, file, 'B')); rank++; break;
			case 'B': pieces.emplace_back(new Bishop(rank, file, 'W')); rank++; break;
			case 'q': pieces.emplace_back(new Queen(rank, file, 'B')); rank++; break;
			case 'Q': pieces.emplace_back(new Queen(rank, file, 'W')); rank++; break;
			case 'k': pieces.emplace_back(new King(rank, file, 'B')); rank++; break;
			case 'K': pieces.emplace_back(new King(rank, file, 'W')); rank++; break;
			case 'p': pieces.emplace_back(new Pawn(rank, file, 'B')); rank++; break;
			case 'P': pieces.emplace_back(new Pawn(rank, file, 'W')); rank++; break;
			case '/': file++; rank = 0; break;
			case '1': rank += 1; break;
			case '2': rank += 2; break;
			case '3': rank += 3; break;
			case '4': rank += 4; break;
			case '5': rank += 5; break;
			case '6': rank += 6; break;
			case '7': rank += 7; break;
			case '8': rank += 8; break;
		}
	}
}

void Piece::constructor(const int X, const int Y, const char COLOR)
{
	sprite.setTexture(texture);
	indices = sf::Vector2i(X, Y);

	color = COLOR;
	sprite.setPosition(indices.x * SPRITE_SIZE, indices.y * SPRITE_SIZE);
}

Piece::~Piece() {}

void Piece::draw()
{
	for (const auto& PIECE : pieces)
	{
		Game::window.draw(PIECE->sprite);
	}
}

void Piece::generateLegalMoves() {}

void Piece::update()
{
	for (auto& piece : pieces)
	{
		piece->legalMoves.clear();

		for (auto& tile : Board::tiles)
		{
			tile.shape.setFillColor(tile.color);
			tile.isLegal = false;
		}

		if (piece->sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && piece->color == colorToMove && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Board::tiles[piece->indices.x + (piece->indices.y * 8)].shape.setFillColor(Board::selectionColor);

			piece->generateLegalMoves();
			for (const auto& LEGAL_MOVE : piece->legalMoves)
			{
				Board::tiles[LEGAL_MOVE.x + (LEGAL_MOVE.y * 8)].shape.setFillColor(Board::legalColor);
				Board::tiles[LEGAL_MOVE.x + (LEGAL_MOVE.y * 8)].isLegal = true;
			}

			Game::window.clear(sf::Color::White);
			Board::draw();
			Piece::draw();
			Game::window.display();

			bool flag{ false };

			sf::Clock delay;
			bool lock{ false };

			while (!flag)
			{
				for (auto& tile : Board::tiles)
				{
					if (tile.shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && tile.isLegal)
					{
						while (!sf::Mouse::isButtonPressed(sf::Mouse::Left));

						if (tile.shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && tile.isLegal)
						{
							piece->indices = tile.indices;
							piece->sprite.setPosition(piece->indices.x * SPRITE_SIZE, piece->indices.y * SPRITE_SIZE);

							for (size_t i{ 0 }; i < pieces.size(); i++)
							{
								if (piece != pieces[i] && piece->indices == pieces[i]->indices)
								{
									pieces.erase(pieces.begin() + i);
									break;

								}
							}

							switch (colorToMove)
							{
								case 'W':
									colorToMove = 'B';
									break;
								case 'B':
									colorToMove = 'W';
									break;
							}

							flag = true;
							break;
						}
					}
					else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && delay.getElapsedTime().asSeconds() > 0.2)
					{
						flag = true;
						break;
					}
				}
			}

			break;
		}
	}

}