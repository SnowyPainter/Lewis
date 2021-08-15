#pragma once

#include "Logger.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

namespace AtomicWorld {
	sf::Font font;
	class Atom {
	private:
		sf::CircleShape bohrcircle;
		sf::Text atomNameText;
		sf::Color border;
	public:
		sf::Vector2f CenterPos() {
			float r = bohrcircle.getRadius();
			auto p = bohrcircle.getPosition();
			p.x += r;
			p.y += r;
			return p;
		}
		void Move(float x, float y) {
			float r = bohrcircle.getRadius();
			x -= r;
			y -= r;
			
			bohrcircle.setPosition(x, y);
			auto b = atomNameText.getLocalBounds();
			atomNameText.setPosition(x + r - b.width / 2, y + r - b.height / 2);
		}
		Atom(std::string atom, int radius, sf::Color borderColor) {
			Logger l = Logger("AtomicWorld::Atom");
			border = borderColor;
			atomNameText.setFont(font);
			atomNameText.setCharacterSize(35);
			atomNameText.setStyle(sf::Text::Bold);
			atomNameText.setString(atom);
			atomNameText.setFillColor(sf::Color::Black);

			bohrcircle.setFillColor(sf::Color::White);
			bohrcircle.setOutlineColor(border);
			bohrcircle.setOutlineThickness(7);
			bohrcircle.setFillColor(sf::Color(237, 237, 237)); //#ededed
			bohrcircle.setRadius(radius);

			Move(0, 0);
		}
		void Draw(sf::RenderWindow * app) {
			app->draw(bohrcircle);
			app->draw(atomNameText);
		}

		Atom* Select() {
			bohrcircle.setOutlineColor(sf::Color::Red);
			return this;
		}
		void UnSelect() {
			bohrcircle.setOutlineColor(border);
		}
	};
}