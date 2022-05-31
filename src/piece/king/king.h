#pragma once
#include <iostream>
#include "../../../vendor/vendor.h"
#include "../piece.h"

class King : public Piece
{
public:
	King(const int X, const int Y, const char COLOR);
	void generateLegalMoves() override;
private:
};

