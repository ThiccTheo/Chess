#include <iostream>
#include "game.h"
#include "../piece/piece.h"

int main()
{
	if (Piece::LOAD())
	{
		Game::run();
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}