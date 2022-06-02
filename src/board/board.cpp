#include "board.h"
#include "../game/game.h"

const float Board::SPRITE_SIZE{ 80.f };
std::vector<Board> Board::tiles;
sf::Color Board::selectionColor{ 233,214,107 };
sf::Color Board::legalColor{ 218,44,67 };

Board::Board(const int X, const int Y, const char COLOR)
{
	isLegal = false;
	indices = sf::Vector2i(X, Y);
	shape.setSize(sf::Vector2f(SPRITE_SIZE, SPRITE_SIZE));
	shape.setPosition(static_cast<float>(X * SPRITE_SIZE), static_cast<float>(Y * SPRITE_SIZE));
	characterColor = COLOR;

	switch (characterColor)
	{
		case 'W': 
			color = sf::Color(211, 211, 211);
			break;
		case 'B': 
			color = sf::Color(131, 137, 150);
			break;
	}
	
	shape.setFillColor(color);
}

void Board::init()
{
	for (int file{ 0 }; file < 8; file++)
	{
		for (int rank{ 0 }; rank < 8; rank++)
		{
			char color{ (file + rank) % 2 ? 'B' : 'W' };

			tiles.emplace_back(rank, file, color);
		}
	}
}

void Board::draw()
{
	for (auto& tile : tiles)
	{
		Game::window.draw(tile.shape);
	}
}