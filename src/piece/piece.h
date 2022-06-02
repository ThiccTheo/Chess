#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "../../vendor/vendor.h"

class Piece
{
public:
	virtual void generateLegalMoves();
	void validateLegalMoves();
	void generateDiagonalMoves();
	void generateOrthogonalMoves();
	static void init(const std::string& FEN);
	void constructor(const int X, const int Y, const char COLOR);
	virtual ~Piece();
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
	bool onSpawnTile;
	bool canBeEnPassantEd;
	static int enPassantMoveCounter;
	bool isPawn;
private:
	static sf::SoundBuffer moveSoundBuffer;
	static sf::SoundBuffer captureSoundBuffer;
	static sf::Sound moveSound;
	static sf::Sound captureSound;

};