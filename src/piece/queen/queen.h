#pragma once
#include <iostream>
#include "../../../vendor/vendor.h"
#include "../piece.h"

class Queen : public Piece
{
public:
	Queen(const int X, const int Y, const char COLOR);
	void generateLegalMoves() override;
private:
};

