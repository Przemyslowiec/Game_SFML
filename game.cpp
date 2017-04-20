#include "game.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <SFML/Audio.hpp>
#include <algorithm>

Game::Game(void){
	state = END;
	if (!font.loadFromFile("AndadaSC-Bold.otf")) {
		MessageBox(NULL,"Font not found!","ERROR",NULL);
		return;
	}
	state = MENU;
}

Game::~Game(void){}


void Game::runGame()
{
	points_get();
	while (state!=END)
	{
		switch (state)
		{
		case gameState::MENU:
			menu();
			break;
		case gameState::GAME:
			gameWindow();
			break;
		case gameState::GAME_OVER:
			gameOver();
				break;
		case gameState::POINTS:
			points_window();
			break;
		case gameState::SETTINGS:
			break;
		default:
			break;
		}
	}
}


void Game::menu()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "My Game");
	sf::Text title("My Game", font, 80);
	title.setStyle(sf::Text::Bold);

	title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 20);

	//liczba dodatkowych napisów
	const int ile = 4;

	sf::Text tekst[ile];
	//tablica z dodatkowymi napisami 
	std::string str[] = { "Play","Points","Sound on","Exit" };
	for (int i = 0; i<ile; i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(50);
		tekst[i].setString(str[i]);
		tekst[i].setPosition(window.getSize().x / 2 - tekst[i].getGlobalBounds().width / 2, 150 + i * 80);
	}

	while (state == MENU)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (tekst[0].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed &&
				event.key.code == sf::Mouse::Left) {
				state = GAME;
			}
			//Wciœniêcie ESC lub przycisk X
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape) {
				state = END;
			}
			else if (tekst[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed) {
				state = POINTS;
			}
			else if (tekst[2].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed &&
				event.key.code == sf::Mouse::Left) {
				if (s == 1) {
					s = 0;
					tekst[2].setString("Sound off");
					tekst[2].setPosition(window.getSize().x / 2 - tekst[2].getGlobalBounds().width / 2, 150 + 2 * 80);
				}
				else {
					s = 1;
					tekst[2].setString("Sound on");
					tekst[2].setPosition(window.getSize().x / 2 - tekst[2].getGlobalBounds().width / 2, 150 + 2 * 80);
				}
			}
				//klikniêcie EXIT
			else if (tekst[3].getGlobalBounds().contains(mouse) &&	event.type == sf::Event::MouseButtonPressed && 
					event.key.code == sf::Mouse::Left)	{
				state = END;
			}
		}
		//zmiana koloru czcionki po najechaniu kursorem 
		
		for (int i = 0; i < ile; i++) {
			if (tekst[i].getGlobalBounds().contains(mouse)) {
				tekst[i].setFillColor(sf::Color::Blue);
			}
			else {
				tekst[i].setFillColor(sf::Color::White);
			}
		}
		
			window.clear();
			window.draw(title);
			for (int i = 0; i < ile; i++) {
				window.draw(tekst[i]);
			}
			window.display();
	}
}

void Game::gameWindow()
{
	srand(time(NULL));
	//tworzenie okna gry
	sf::RenderWindow GameWindow(sf::VideoMode(480, 480), "Run away!");

	GameWindow.setFramerateLimit(150);

	sf::Texture picture;
	sf::Texture picture1;
	if (!picture.loadFromFile("ucieknier.jpg")) {
		MessageBox(NULL, "Picture not found!", "ERROR", NULL);
		return;
	}
	if (!picture1.loadFromFile("goniec.jpg")) {
		MessageBox(NULL, "Picture not found!", "ERROR", NULL);
		return;
	}

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("oo.wav")) {
		MessageBox(NULL, "Sound not found!", "ERROR", NULL);
		return;
	}
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(100);
	//liczba obiektów

	int x[15], y[15];

	bool a = 0;

	sf::Sprite runner;
	runner.setTexture(picture);

	sf::Sprite deserter[15];
	//tablica goñców
	for (size_t i = 0; i < 5; i++) {
		deserter[i].setTexture(picture1);
		deserter[i].setPosition(100, 100);
	}
	for (size_t i = 0; i < 5; i++) {
		x[i] = (rand() % 3 + 1);
		y[i] = (rand() % 3 + 1);
	}
	
	runner.setPosition(400, 400);

	sound.setLoop(false);
	//przyciœniêcie przycisku -brak
	bool pressed_left_button = 0;
	int moment = 10;
	int dificult = 1;
	double time_start;
	sf::Text po;
	po.setPosition(50, 50);
	po.setFont(font);
	po.setCharacterSize(40);
	sf::Text prepere;
	prepere.setString("Prepare!");
	prepere.setPosition(100, 100);
	prepere.setFont(font);
	prepere.setCharacterSize(40);
	bool pr = 0;
	while (state == GAME)
	{
		while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (size_t ile = 0; ile < dificult; ile++) {
				if (pressed_left_button == 0) {
					time_start = clock();
				}

				//stan przycisku
				pressed_left_button = 1;

				//ruch uciekiniera
				runner.setPosition(sf::Mouse::getPosition(GameWindow).x - runner.getLocalBounds().width / 2, sf::Mouse::getPosition(GameWindow).y - runner.getLocalBounds().width / 2);
				//ruch deserter
				if (((int)(clock() - time_start) / CLOCKS_PER_SEC) % 10 == 0 && ((int)(clock() - time_start) / CLOCKS_PER_SEC) == moment) {
					if (dificult < 15) {
						dificult += 1;
						pr = 0;
					}
					moment += 10;
				}
				//wyœwietlanie ostrze¿enia o nowym klocku
				if ((((int)(clock() - time_start) / CLOCKS_PER_SEC) + 1) % 10== 0 ) {
				if (dificult < 5) {
						pr = 1;
					}
				}

				for (size_t iw = 0; iw < dificult; iw++) {
					deserter[iw].move(x[iw], y[iw]);
				}
				if (deserter[ile].getPosition().x < 0) {
					x[ile] = (x[ile] / x[ile]);
					x[ile] *= (rand() % 3 + 1);
					if (s == 1) {
						sound.play();
					}
				}
				if (deserter[ile].getPosition().x + deserter[ile].getLocalBounds().height > GameWindow.getSize().x) {
					x[ile] = (x[ile] / x[ile]);
					x[ile] *= -1;
					x[ile] *= (rand() % 3 + 1);
					if (s == 1) {
						sound.play();
					}

				}
				if (deserter[ile].getPosition().y < 0) {
					y[ile] = (y[ile] / y[ile]);
					y[ile] *= (rand() % 3 + 1);
					if (s == 1) {
						sound.play();
					}
				}
				if (deserter[ile].getPosition().y + deserter[ile].getLocalBounds().height > GameWindow.getSize().y) {
					y[ile] = (y[ile] / y[ile]);
					y[ile] *= -1;
					y[ile] *= (rand() % 3 + 1);
					if (s == 1) {
						sound.play();
					}
				}
				//zderzenie z drug¹ figur¹
				if (runner.getPosition().x >= deserter[ile].getPosition().x - runner.getLocalBounds().height &&
					runner.getPosition().x <= deserter[ile].getPosition().x + deserter[ile].getLocalBounds().height &&
					runner.getPosition().y >= deserter[ile].getPosition().y - runner.getLocalBounds().height &&
					runner.getPosition().y <= deserter[ile].getPosition().y + deserter[ile].getLocalBounds().height) {
					if (!a) {
						points_list.push_back((int)(clock() - time_start) / CLOCKS_PER_SEC);
						a = 1;
					}
					state = GAME_OVER;
					GameWindow.clear();
					GameWindow.close();
				}
				//zderzenie ze œciank¹ 
				if (runner.getPosition().x<0 ||
					runner.getPosition().y<0 ||
					runner.getPosition().x + runner.getLocalBounds().height>GameWindow.getSize().x ||
					runner.getPosition().y + runner.getLocalBounds().width>GameWindow.getSize().y) {
					if (!a) {
						points_list.push_back((int)(clock() - time_start) / CLOCKS_PER_SEC);
						a = 1;

					}

					state = GAME_OVER;
					GameWindow.clear();
					GameWindow.close();
				}
				po.setString(boost::lexical_cast<std::string>((int)(clock() - time_start) / CLOCKS_PER_SEC));
				//wyœwitlanie

				GameWindow.clear();
				GameWindow.draw(po);
				if (pr) {
					GameWindow.draw(prepere);
				}
				for (size_t ko = 0; ko < dificult; ko++) {
					GameWindow.draw(deserter[ko]);
				}
				GameWindow.draw(runner);
				GameWindow.display();
			}

		}
		//jeœli przycisk zostanie puszczony 
		if (pressed_left_button == 1) {
			if (!a) {
				points_list.push_back((int)(clock() - time_start) / CLOCKS_PER_SEC);
				a = 1;
			}
			state = GAME_OVER;
		}
		//wyœwietlanie 
		GameWindow.clear();
		for (size_t k = 0; k < dificult; k++) {
			GameWindow.draw(deserter[k]);
		}
		GameWindow.draw(runner);
		GameWindow.display();
	}

}


void Game::gameOver()
{
	std::sort(points_list.begin(), points_list.end());
	points_write();
	
	sf::RenderWindow window(sf::VideoMode(500, 500), "My Game");
	sf::Text title("You lose!", font, 80);
	title.setStyle(sf::Text::Bold);

	title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 20);

	//liczba dodatkowych napisów
	const int ile = 3;

	sf::Text tekst[ile];
	//tablica z dodatkowymi napisami 
	std::string str[] = { "Play again","Menu","Exit" };
	for (int i = 0; i<ile; i++)	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(50);
		tekst[i].setString(str[i]);
		tekst[i].setPosition(window.getSize().x / 2 - tekst[i].getGlobalBounds().width / 2, 250 + i * 80);
	}
	sf::Event event;
	while (state == GAME_OVER)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		while (window.pollEvent(event))
		{
			if (tekst[0].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed  &&
				event.key.code == sf::Mouse::Right) {			
					state = GAME;
			}
			if (tekst[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed  &&
				event.key.code == sf::Mouse::Right) {
				state = MENU;
			}			
			//klikniêcie EXIT
			else if (tekst[2].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed &&
				event.key.code == sf::Mouse::Right)
			{
				state = END;
			}
		}
		//zmiana koloru czcionki po najechaniu kursorem 
		for (int i = 0; i < ile; i++) {
			if (tekst[i].getGlobalBounds().contains(mouse)) {
				tekst[i].setFillColor(sf::Color::Blue);
			}
			else {
				tekst[i].setFillColor(sf::Color::White);
			}
		}

		window.clear();
		window.draw(title);
		for (int i = 0; i < ile; i++) {
			window.draw(tekst[i]);
		}
		window.display();
	}
}

void Game::points_get()
{
	std::ifstream binary_file("points.game", std::ios::binary);
	int p;
	if (binary_file) {
		for (size_t i = 0; i < 5; i++) {
			binary_file.read(reinterpret_cast <char*> (&p), sizeof(p));
			points_list.push_back(p);
		}
		binary_file.close();
	}
}

void Game::points_write()
{
	std::ofstream binary_file("points.game", std::ios::binary);
	if (points_list.size() < 5) {
		for (unsigned int i = points_list.size(); i < 5; i++) {
			points_list.push_back(0);
		}
	}

	std::sort(points_list.begin(), points_list.end(), [](int&n, int&b)->bool{return n > b; });
	if (binary_file) {
		for (int i = 0; i < 5; i++) {
			binary_file.write(reinterpret_cast<char*> (&points_list[i]), sizeof(points_list[i]));
		}
		binary_file.close();
	}	
}


void Game::points_window()
{
	sf::RenderWindow window(sf::VideoMode(500, 800), "My Game");
	sf::Text title("Best points", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 20);

	//liczba dodatkowych napisów
	const int ile = 6;

	sf::Text tekst;
	//tablica z dodatkowymi napisami 
	tekst.setFont(font);
	tekst.setCharacterSize(30);
	tekst.setString("Menu");
	tekst.setPosition(window.getSize().x / 2 - tekst.getGlobalBounds().width / 2,700);
	
	sf::Text reset;
	reset.setFont(font);
	reset.setCharacterSize(30);
	reset.setString("Reset points");
	reset.setPosition(window.getSize().x / 2 - reset.getGlobalBounds().width / 2,600);
	//numery pozycji i punkty
	sf::Text ww[5];
	for (unsigned int i = 0; i < 5; i++) {
		ww[i].setFont(font);
		ww[i].setCharacterSize(40);
		ww[i].setString(boost::lexical_cast<std::string>(i+1)+"."+boost::lexical_cast<std::string>(points_list[i]));
		ww[i].setPosition((window.getSize().x / 2 - ww[i].getGlobalBounds().width / 2)-30, 50 + (i+1) * 80);
	}
	while (state == POINTS)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (tekst.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed  &&
				event.key.code == sf::Mouse::Left) {
				state = MENU;
			}
			
			if (reset.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonPressed  &&
				event.key.code == sf::Mouse::Left) {
				points_list.clear();
				for (size_t p = 0; p < 5; p++) {
					points_list.push_back(0);
				}
				points_write();
				for (int i = 0; i<5; i++) {
					ww[i].setFont(font);
					ww[i].setCharacterSize(40);
					ww[i].setString(boost::lexical_cast<std::string>(i + 1) + "." + boost::lexical_cast<std::string>(points_list[i]));
					ww[i].setPosition((window.getSize().x / 2 - ww[i].getGlobalBounds().width / 2) - 30, 50 + (i + 1) * 80);
				}
			}

			window.clear();
			window.draw(reset);
			window.draw(title);
			for (int i = 0; i < ile; i++) {
				window.draw(tekst);
				if (i >= 1 ) {
					window.draw(ww[i - 1]);
				}
			}
			window.display();
		}
		//zmiana koloru czcionki po najechaniu kursorem 
		if (reset.getGlobalBounds().contains(mouse)) {
			reset.setFillColor(sf::Color::Blue);
		}
		else{
			reset.setFillColor(sf::Color::White);
		}
		for (int i = 0; i < ile; i++) {
			if (tekst.getGlobalBounds().contains(mouse)) {
				tekst.setFillColor(sf::Color::Blue);
			}
			else {
				tekst.setFillColor(sf::Color::White);
			}
		}

		window.clear();
		window.draw(title);
		window.draw(reset);
		for (int i = 0; i < ile; i++) {
			window.draw(tekst);
			if (i >= 1 ) {
				window.draw(ww[i - 1]);
			}
		}
		window.display();
	}
}

