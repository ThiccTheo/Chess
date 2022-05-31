#pragma once
#include <iostream>
#include "../../../vendor/vendor.h"
#include "../piece.h"

class Bishop : public Piece
{
public:
	Bishop(const int X, const int Y, const char COLOR);
	void generateLegalMoves() override;
private:
};

