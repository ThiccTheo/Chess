#pragma once
#include <iostream>
#include <vector>
#include "../../vendor/vendor.h"

class Board
{
public:
	static void init();
	static void draw();

	sf::RectangleShape shape;
	static std::vector<Board> tiles;
	sf::Color color;
	static sf::Color selectionColor;
	static sf::Color legalColor;
	bool isLegal;
	char characterColor;
	sf::Vector2i indices;
	Board(const int X, const int Y, const char COLOR);
private:
	static const float SPRITE_SIZE;
};