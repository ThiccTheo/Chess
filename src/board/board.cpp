#include "board.h"
#include "../game/game.h"

const float Board::SPRITE_SIZE{ 80.f };
std::vector<Board> Board::tiles;

Board::Board(const int X, const int Y, const char COLOR)
{
	indices = sf::Vector2i(X, Y);
	shape.setSize(sf::Vector2f(SPRITE_SIZE, SPRITE_SIZE));
	shape.setPosition(static_cast<float>(X * SPRITE_SIZE), static_cast<float>(Y * SPRITE_SIZE));
	
	characterColor = COLOR;

	switch (characterColor)
	{
		case 'W': 
			color = sf::Color(245, 230, 190);
			break;
		case 'B': 
			color = sf::Color(102, 68, 58);
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

			tiles.push_back(Board(rank, file, color));
		}
	}
}

void Board::draw()
{
	for (const auto& TILE : tiles)
	{
		Game::window.draw(TILE.shape);
	}
}