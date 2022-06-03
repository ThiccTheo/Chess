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
sf::SoundBuffer Piece::promotionSoundBuffer;
sf::Sound Piece::moveSound;
sf::SoundBuffer Piece::castleSoundBuffer;
sf::Sound Piece::castleSound;
sf::Sound Piece::captureSound;
sf::Sound Piece::promotionSound;
int Piece::enPassantMoveCounter;

const bool Piece::LOAD()
{
	if (
		texture.loadFromFile("src/piece/spritesheet.png") &&
		moveSoundBuffer.loadFromFile("src/piece/move.wav") &&
		captureSoundBuffer.loadFromFile("src/piece/capture.wav") &&
		promotionSoundBuffer.loadFromFile("src/piece/promotion.wav") &&
		castleSoundBuffer.loadFromFile("src/piece/castle.wav")

		)
	{
		texture.setSmooth(true);
		moveSound.setBuffer(moveSoundBuffer);
		captureSound.setBuffer(captureSoundBuffer);
		promotionSound.setBuffer(promotionSoundBuffer);
		castleSound.setBuffer(castleSoundBuffer);
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
	shouldDelete = false;
	checkCounter = 0;
}

Piece::~Piece() {}

void Piece::draw()
{
	for (auto& piece : pieces)
	{
		if (piece->shouldDelete)
		{
			piece.reset();
			pieces.erase(std::remove(pieces.begin(), pieces.end(), piece), pieces.end());
			break;
		}
		Game::window.draw(piece->sprite);
	}
}

void Piece::generateLegalMoves() {}

void Piece::validateLegalMoves()
{
	for (const auto& PIECE : pieces)
	{
		//remove legal moves that collide with pieces
		legalMoves.erase(std::remove_if(legalMoves.begin(), legalMoves.end(),
		[&PIECE, this](const sf::Vector2i& LEGAL_MOVE)
		{
			/*if (PIECE->indices == LEGAL_MOVE && PIECE->name == "king" && PIECE->color != color)
			{
				PIECE->checkCounter++;
			}*/
			return (LEGAL_MOVE == PIECE->indices && color == PIECE->color);
		}),
		legalMoves.end());
	}
}

void Piece::promotePawn()
{
	bool flag{ false };
	std::vector<sf::RectangleShape> squares;
	squares.emplace_back(sf::Vector2f(static_cast<float>(Game::RESOLUTION.x / 2), static_cast<float>(Game::RESOLUTION.y / 2)));
	squares.emplace_back(sf::Vector2f(static_cast<float>(Game::RESOLUTION.x / 2), static_cast<float>(Game::RESOLUTION.y / 2)));
	squares.emplace_back(sf::Vector2f(static_cast<float>(Game::RESOLUTION.x / 2), static_cast<float>(Game::RESOLUTION.y / 2)));
	squares.emplace_back(sf::Vector2f(static_cast<float>(Game::RESOLUTION.x / 2), static_cast<float>(Game::RESOLUTION.y / 2)));

	//queen, knight, rook, bishop
	sf::Sprite queen;
	queen.setTexture(texture);

	sf::Sprite knight;
	knight.setTexture(texture);

	sf::Sprite rook;
	rook.setTexture(texture);

	sf::Sprite bishop;
	bishop.setTexture(texture);

	sf::Vector2f scale{ static_cast<float>(Game::RESOLUTION.x / 80 / 2), static_cast<float>(Game::RESOLUTION.y / 80 / 2) };
	switch (color)
	{
		case 'W':
			queen.setTextureRect(sf::IntRect(80, 0, 80, 80));
			knight.setTextureRect(sf::IntRect(240, 0, 80, 80));
			rook.setTextureRect(sf::IntRect(320, 0, 80, 80));
			bishop.setTextureRect(sf::IntRect(160, 0, 80, 80));
			break;
		case 'B':
			queen.setTextureRect(sf::IntRect(80, 80, 80, 80));
			knight.setTextureRect(sf::IntRect(240, 80, 80, 80));
			rook.setTextureRect(sf::IntRect(320, 80, 80, 80));
			bishop.setTextureRect(sf::IntRect(160, 80, 80, 80));
			break;
	}

	for (size_t i {0}; i < squares.size(); i++)
	{
		//squares[i].setSize(sf::Vector2f(Game::RESOLUTION.x / 4, Game::RESOLUTION.y / 4));
		switch (i)
		{
			case 0:
				squares[0].setPosition(sf::Vector2f(0.f, 0.f));
				queen.setScale(scale);
				queen.setPosition(squares[0].getPosition());
				break;
			case 1:
				squares[1].setPosition(sf::Vector2f(static_cast<float>(Game::RESOLUTION.x / 2), 0.f));
				knight.setScale(scale);
				knight.setPosition(squares[1].getPosition());
				break;
			case 2:
				squares[2].setPosition(sf::Vector2f(0.f, static_cast<float>(Game::RESOLUTION.y / 2)));
				rook.setScale(scale);
				rook.setPosition(squares[2].getPosition());
				break;
			case 3:
				squares[3].setPosition(sf::Vector2f(static_cast<float>(Game::RESOLUTION.x / 2), static_cast<float>(Game::RESOLUTION.y / 2)));
				bishop.setScale(scale);
				bishop.setPosition(squares[3].getPosition());
				break;
		}
		squares[i].setFillColor(sf::Color::White);
	}
	
	sf::Clock delay;

	while (!flag)
	{
		Game::window.clear(sf::Color::White);
		for (size_t i{ 0 }; i < squares.size(); i++)
		{
			if (squares[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))))
			{
				squares[i].setFillColor(Board::selectionColor);
				if (delay.getElapsedTime().asSeconds() > 0.2 && squares[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Game::window))) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					switch (i)
					{
					case 0:
						pieces.emplace_back(new Queen(indices.x, indices.y, color));
						flag = true;
						break;
					case 1:
						pieces.emplace_back(new Knight(indices.x, indices.y, color));
						flag = true;
						break;
					case 2:
						pieces.emplace_back(new Rook(indices.x, indices.y, color));
						flag = true;
						break;
					case 3:
						pieces.emplace_back(new Bishop(indices.x, indices.y, color));
						flag = true;
						break;
					}
					shouldDelete = true;
				}
			}
			else
			{
				squares[i].setFillColor(sf::Color::White);
			}
			Game::window.draw(squares[i]);
		}
		Game::window.draw(queen);
		Game::window.draw(knight);
		Game::window.draw(rook);
		Game::window.draw(bishop);
		Game::window.display();
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
							std::string soundType{ "move" };

							if (piece->name == "king")
							{
								//castling long
								if (tempIndices.x - 2 == piece->indices.x)
								{
									for (auto& castlePiece : pieces)
									{
										if (castlePiece->color == piece->color && castlePiece->name == "rook" && castlePiece->onSpawnTile == true && castlePiece->indices.x == 0)
										{
											castlePiece->indices.x += 3;
											castlePiece->sprite.setPosition(castlePiece->indices.x * SPRITE_SIZE, castlePiece->indices.y * SPRITE_SIZE);
											soundType = "castle";
											break;
										}
									}
								}
								//castling short
								if (tempIndices.x + 2 == piece->indices.x)
								{
									for (auto& castlePiece : pieces)
									{
										if (castlePiece->color == piece->color && castlePiece->name == "rook" && castlePiece->onSpawnTile == true && castlePiece->indices.x == 7)
										{
											castlePiece->indices.x -= 2;
											castlePiece->sprite.setPosition(castlePiece->indices.x * SPRITE_SIZE, castlePiece->indices.y * SPRITE_SIZE);
											soundType = "castle";
											break;
										}
									}
								}
							}

							if (piece->name == "pawn")
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
								switch (piece->color)
								{
								case 'W':
									if (piece->indices.y == 0)
									{
										soundType = "promotion";
										bool captureFlag{ false };
										for (size_t i{ 0 }; i < pieces.size(); i++)
										{
											if (piece->indices == pieces[i]->indices && piece->color != pieces[i]->color)
											{
												captureFlag = true;
												break;
											}
										}
										if (captureFlag)
										{
											captureSound.play();
										}
										else
										{
											moveSound.play();
										}
										piece->promotePawn();
										sf::Clock tempTimer;
										while (tempTimer.getElapsedTime().asSeconds() < 0.1);
									}
									break;
								case 'B':
									if (piece->indices.y == 7)
									{
										soundType = "promotion";
										bool captureFlag{ false };
										for (size_t i{ 0 }; i < pieces.size(); i++)
										{
											if (piece->indices == pieces[i]->indices && piece->color != pieces[i]->color)
											{
												captureFlag = true;
												break;
											}
										}
										if (captureFlag)
										{
											captureSound.play();
										}
										else
										{
											moveSound.play();
										}
										piece->promotePawn();
										sf::Clock tempTimer;
										while (tempTimer.getElapsedTime().asSeconds() < 0.1);
									}
									break;
								}
							}
							/*std::cout << piece->indices.x << ", " << piece->indices.y << '\n';*/

							for (size_t i{ 0 }; i < pieces.size(); i++)
							{
								if (piece != pieces[i] && piece->indices == pieces[i]->indices && piece->color != pieces[i]->color)
								{
									pieces.erase(pieces.begin() + i);
									if (soundType != "promotion")
									{
										soundType = "capture";
									}
									break;

								}
								if (pieces[i]->name == "pawn" && pieces[i]->canBeEnPassantEd)
								{
									if (piece->color == 'W' && pieces[i]->color == 'B' && piece->indices.x == pieces[i]->indices.x && piece->indices.y == pieces[i]->indices.y - 1)
									{
										pieces.erase(pieces.begin() + i);
										if (soundType != "promotion")
										{
											soundType = "capture";
										}
										break;
									}
									if (piece->color == 'B' && pieces[i]->color == 'W' && piece->indices.x == pieces[i]->indices.x && piece->indices.y == pieces[i]->indices.y + 1)
									{
										pieces.erase(pieces.begin() + i);
										if (soundType != "promotion")
										{
											soundType = "capture";
										}
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
							else if (soundType == "promotion")
							{
								promotionSound.play();
							}
							else if (soundType == "castle")
							{
								castleSound.play();
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