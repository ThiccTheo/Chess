#pragma once
#include <iostream>
#include "../../../vendor/vendor.h"
#include "../piece.h"

class Rook : public Piece
{
public:
	Rook(const int X, const int Y, const char COLOR);
	void generateLegalMoves() override;
private:
};

