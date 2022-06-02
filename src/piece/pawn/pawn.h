#pragma once
#include <iostream>
#include "../../../vendor/vendor.h"
#include "../piece.h"

class Pawn : public Piece
{
public:
	Pawn(const int X, const int Y, const char COLOR);
	Pawn();
	void generateLegalMoves() override;
private:
};

