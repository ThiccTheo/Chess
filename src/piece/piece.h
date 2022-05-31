#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "../../vendor/vendor.h"

class Piece
{
public:
	virtual void generateLegalMoves();
	static void init(const std::string& FEN);
	void constructor(const int X, const int Y, const char COLOR);
	static void draw();
	static void update();
	static const bool LOAD();

	static const float SPRITE_SIZE;
	static std::vector<std::unique_ptr<Piece>> pieces;
	static sf::Texture texture;
	sf::Sprite sprite;
	std::vector<sf::Vector2i> legalMoves;
	sf::Vector2i indices;
	char color;
	static char colorToMove;
};