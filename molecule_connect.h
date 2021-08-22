#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "atom.h"
class molecule_connect {
private:
	std::vector<std::vector<sf::Atom*>> connections;

	sf::Vertex getVtxFromPos(sf::Vector2f pos) {
		return sf::Vertex(pos, sf::Color::Red);
	}
public:
	molecule_connect() {
		connections.push_back(std::vector<sf::Atom*>());
	}
	void AddPair(sf::Atom* at1, sf::Atom* at2) {
		bool paired = false;
		for (auto &conn : connections) {
			for (auto atom : conn) {
				if (atom == at1) {
					paired = true;
					conn.push_back(at2);
					return;
				}
				else if (atom == at2) {
					paired = true;
					conn.push_back(at1);
					return;
				}
			}
		}
		if (!paired) {
			connections.push_back(std::vector<sf::Atom*>());
			connections[connections.size() - 1].push_back(at1);
			connections[connections.size() - 1].push_back(at2);
			return;
		}
	}

	void Draw(sf::RenderWindow* app) {

		for (auto& conn : connections) {
			const int size = conn.size();
			sf::Vertex* line = new sf::Vertex[size];
			for (int i = 0; i < size;i++) {
				line[i] = getVtxFromPos(conn[i]->CenterPos());
			}
			app->draw(line, size, sf::LinesStrip);
		}
	}
};