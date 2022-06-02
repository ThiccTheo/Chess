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
sf::SoundBuffer Piece::moveSoundBuffer;
sf::SoundBuffer Piece::captureSoundBuffer;
sf::Sound Piece::moveSound;
sf::Sound Piece::captureSound;
int Piece::enPassantMoveCounter;

const bool Piece::LOAD()
{
	if (
		texture.loadFromFile("src/piece/spritesheet.png") &&
		moveSoundBuffer.loadFromFile("src/piece/move.wav") &&
		captureSoundBuffer.loadFromFile("src/piece/capture.wav")

		)
	{
		moveSound.setBuffer(moveSoundBuffer);
		captureSound.setBuffer(captureSoundBuffer);
		return true;
	}
	else
	{
		return false;
	}
}

void Piece::init(const std::string& FEN)
{
	enPassantMoveCounter = 0;
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
	onSpawnTile = true;
	isPawn = false;
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

void Piece::validateLegalMoves()
{
	for (size_t i{ 0 }; i < legalMoves.size(); i++)
	{
		for (const auto& PIECE : pieces)
		{
			legalMoves.erase(std::remove_if(legalMoves.begin(), legalMoves.end(),
			[&PIECE, this](const sf::Vector2i& LEGAL_MOVE)
			{
				return (LEGAL_MOVE == PIECE->indices && color == PIECE->color);
			}),
			legalMoves.end());
		}
	}
}

void Piece::generateDiagonalMoves()
{
	sf::Vector2i pos;
	bool flag;

	//northeast
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + ((pos.y - 1) * 8) + 1) >= 0 && (pos.x + ((pos.y - 1) * 8) + 1) <= 63 && Board::tiles[pos.x + ((pos.y - 1) * 8) + 1].indices.x > pos.x)
		{
			pos.x++;
			const Board& TILE = Board::tiles[pos.x + (--pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}

	//northwest
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + ((pos.y - 1) * 8) - 1) >= 0 && (pos.x + ((pos.y - 1) * 8) - 1) <= 63 && Board::tiles[pos.x + ((pos.y - 1) * 8) - 1].indices.x < pos.x)
		{
			pos.x--;
			const Board& TILE = Board::tiles[pos.x + (--pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}

	//southeast
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + ((pos.y + 1) * 8) + 1) >= 0 && (pos.x + ((pos.y + 1) * 8) + 1) <= 63 && Board::tiles[pos.x + ((pos.y + 1) * 8) + 1].indices.x > pos.x)
		{
			pos.x++;
			const Board& TILE = Board::tiles[pos.x + (++pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}

	//southwest
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + ((pos.y + 1) * 8) - 1) >= 0 && (pos.x + ((pos.y + 1) * 8) - 1) <= 63 && Board::tiles[pos.x + ((pos.y + 1) * 8) - 1].indices.x < pos.x)
		{
			pos.x--;
			const Board& TILE = Board::tiles[pos.x + (++pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}
}

void Piece::generateOrthogonalMoves()
{
	sf::Vector2i pos;
	bool flag;

	//north
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + ((pos.y - 1) * 8)) >= 0 && (pos.x + ((pos.y - 1) * 8)) <= 63)
		{
			const Board& TILE = Board::tiles[pos.x + (--pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}

	//south
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + ((pos.y + 1) * 8)) <= 63 && (pos.x + ((pos.y + 1) * 8)) >= 0)
		{
			const Board& TILE = Board::tiles[pos.x + (++pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}

	//east
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + (pos.y * 8) + 1) % 8 != 0 && (pos.x + (pos.y * 8) + 1) <= 63 && (pos.x + (pos.y * 8) + 1) >= 0)
		{
			pos.x++;
			const Board& TILE = Board::tiles[pos.x + (pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}
	}

	//west
	flag = false;
	pos = indices;
	while (!flag)
	{
		if ((pos.x + (pos.y * 8) - 1) >= 0 && (pos.x + (pos.y * 8) - 1) <= 63 && Board::tiles[pos.x + (pos.y * 8) - 1].indices.y == pos.y)
		{
			pos.x--;
			const Board& TILE = Board::tiles[pos.x + (pos.y * 8)];

			for (const auto& PIECE : pieces)
			{
				if (PIECE->indices == TILE.indices)
				{
					legalMoves.emplace_back(TILE.indices);
					flag = true;
					break;
				}
				else
				{
					legalMoves.emplace_back(TILE.indices);
				}
			}
		}
		else
		{
			break;
		}

	}
}

void Piece::update()
{
	for (auto& piece : pieces)
	{
		for (auto& tile : Board::tiles)
		{
			tile.shape.setFillColor(tile.color);
			tile.isLegal = false;
		}

		if (piece->sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && piece->color == colorToMove && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Board::tiles[piece->indices.x + (piece->indices.y * 8)].shape.setFillColor(Board::selectionColor);

			piece->legalMoves.clear();
			piece->generateLegalMoves();
			piece->validateLegalMoves();

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
						while (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && tile.shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && tile.isLegal);

						if (tile.shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && tile.isLegal)
						{
							enPassantMoveCounter++;

							sf::Vector2i tempIndices = piece->indices;

							piece->indices = tile.indices;
							piece->sprite.setPosition(piece->indices.x * SPRITE_SIZE, piece->indices.y * SPRITE_SIZE);
							piece->onSpawnTile = false;

							if (piece->isPawn)
							{
								if (piece->canBeEnPassantEd == true && enPassantMoveCounter > 0)
								{
									piece->canBeEnPassantEd = false;
								}
								if ((tempIndices.y - 2 == piece->indices.y) || (tempIndices.y + 2 == piece->indices.y))
								{
									piece->canBeEnPassantEd = true;
									enPassantMoveCounter = 0;
								}
							}
							/*std::cout << piece->indices.x << ", " << piece->indices.y << '\n';*/

							std::string soundType{ "move" };

							for (size_t i{ 0 }; i < pieces.size(); i++)
							{
								if (piece != pieces[i] && piece->indices == pieces[i]->indices)
								{
									pieces.erase(pieces.begin() + i);
									soundType = "capture";
									captureSound.play();
									break;

								}
								if (pieces[i]->isPawn && pieces[i]->canBeEnPassantEd)
								{
									if (piece->color == 'W' && pieces[i]->color == 'B' && piece->indices.x == pieces[i]->indices.x && piece->indices.y == pieces[i]->indices.y - 1)
									{
										pieces.erase(pieces.begin() + i);
										soundType = "capture";
										captureSound.play();
										break;
									}
									if (piece->color == 'B' && pieces[i]->color == 'W' && piece->indices.x == pieces[i]->indices.x && piece->indices.y == pieces[i]->indices.y + 1)
									{
										pieces.erase(pieces.begin() + i);
										soundType = "capture";
										captureSound.play();
										break;
									}
								}
							}

							if (soundType == "move")
							{
								moveSound.play();
							}
							else if (soundType == "capture")
							{
								captureSound.play();
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