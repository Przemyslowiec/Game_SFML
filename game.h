#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class Game
{
public:
	Game(void);
	~Game(void);
	void runGame();

private:
	sf::Font font;
	void menu();
	void gameWindow();
	void gameOver();
	void points_get();
	void points_write();
	void points_window();
protected:
//	int points = 0;
	bool s = 1;
	std::vector<int> points_list;
	enum gameState
	{
		MENU, GAME, GAME_OVER, END,POINTS,SETTINGS
	};
	gameState state;
};
