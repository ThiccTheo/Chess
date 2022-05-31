#include "game.h"
#include "../board/board.h"
#include "../piece/piece.h"

sf::RenderWindow Game::window;
char Game::winner{ ' ' };
const sf::Vector2u Game::RESOLUTION{ 640, 640 };

void Game::init()
{
	winner = ' ';
	window.create(sf::VideoMode(RESOLUTION.x, RESOLUTION.y), "Chess", sf::Style::Close);
	window.setFramerateLimit(60);
}

void Game::run()
{
	init();

	Board::tiles.clear();
	Board::init();

	Piece::pieces.clear();
	Piece::init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::White);
		Board::draw();
		Piece::draw();
		window.display();
		Piece::update();
	}

	//if winner == 'w' or 'b'
}