#pragma once
#include <iostream>
#include "../../../vendor/vendor.h"
#include "../piece.h"

class Knight : public Piece
{
public:
	Knight(const int X, const int Y, const char COLOR);
	void generateLegalMoves() override;
private:
};