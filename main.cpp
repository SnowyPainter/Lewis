#include "chemical_formula.h"
#include "atom.h"
#include "Logger.h"
#include "hsv.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tuple>
#include <random>
#include <cctype>
#include <locale>
#include <map>

namespace config {
	const sf::VideoMode videoMode(1600, 864);
	const sf::String title("Lewis");
	const int style = sf::Style::Close;
}

std::vector<std::string> split(std::string str, char c) {

	int i = 0, c_count = 0;

	for (int j = 0; j < str.length(); j++)
		if (str[j] == c)
			c_count++;

	std::vector<std::string> result;

	for (int j = 0; j < c_count + 1 && i < str.length(); j++) {
		std::string splited;

		while (i < str.length() && str[i] != c) {
			splited.push_back(str[i]);
			i++;
		}
		i++;

		result.push_back(splited);
	}

	return result;
}

void ltrim(std::string* s) {
	s->erase(s->begin(), std::find_if(s->begin(), s->end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

void rtrim(std::string* s) {
	s->erase(std::find_if(s->rbegin(), s->rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s->end());
}

std::string trim(std::string s) {
	ltrim(&s);
	rtrim(&s);
	return s;
}

int main() {
	int padding = 160;
	std::random_device rseed;
	std::mt19937 gen(rseed());
	std::uniform_int_distribution<int> w(padding, config::videoMode.width - padding);
	std::uniform_int_distribution<int> h(padding, config::videoMode.height - padding);
	
	std::uniform_int_distribution<int> hue(19, 320);

	Logger l = Logger("main");

	l.Log("This program only supports defined 1~18th atoms");

	l.Log("Initializing RenderWindow and settings ... ");
	sf::RenderWindow app(config::videoMode, config::title, config::style);
	app.setFramerateLimit(60);
	app.setKeyRepeatEnabled(false);

	if (!AtomicWorld::font.loadFromFile("E:/C++/SFML/Lewis/nmr.ttf")) {
		l.Error("There's an error with loading nmr.ttf font");
	}
	l.Log("Parsing Chemical Formula to some tuples ... ");
	std::string c = "O2 + H2";
	std::map<std::string, std::vector<std::tuple<std::string, int>>> parsed_formulas;
	std::vector<AtomicWorld::Atom> atoms;
	auto chemical_formulas = split(c, '+');
	for (int i = 0; i < chemical_formulas.size(); i++) {
		chemical_formulas[i] = trim(chemical_formulas[i]);
		parsed_formulas.insert({ chemical_formulas[i], chemical_formula::Parse(chemical_formulas[i]) });
		auto parsed = parsed_formulas[chemical_formulas[i]];
		std::cout << "Parsed chemical formula : " << chemical_formulas[i] << std::endl;
		
		sf::Color colorTheme = sf::hsv(hue(gen), 100, 100);

		for (int i = 0; i < parsed.size(); i++) {
			std::cout << "Atom named : " << std::get<0>(parsed[i]) << ", Generated " << std::get<1>(parsed[i]) << std::endl;
			for (int j = 0; j < std::get<1>(parsed[i]); j++) {
				auto atom = AtomicWorld::Atom(std::get<0>(parsed[i]), 60, colorTheme);
				atom.Move(w(gen), h(gen));
				atoms.push_back(atom);
			}
		}
	}

	l.Log("Done. all initializing & parsing");

	auto selectedAtom = atoms[0].Select();
	float downest = config::videoMode.height;
	float rightest = config::videoMode.width;
	while (app.isOpen())
	{
		sf::Event event;
		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				app.close();
			if (event.type == sf::Event::KeyPressed) {
				if ((event.key.code == sf::Keyboard::Up ||
					event.key.code == sf::Keyboard::Down ||
					event.key.code == sf::Keyboard::Left ||
					event.key.code == sf::Keyboard::Right)) {

					auto nearest = selectedAtom;
					float down_nearest_curr = downest;
					float right_nearest_curr = rightest;

					for (int i = 0; i < atoms.size(); i++) {
						if (atoms[i].CenterPos() == selectedAtom->CenterPos())
							continue;

						if (event.key.code == sf::Keyboard::Up) {
							float d = selectedAtom->CenterPos().y - atoms[i].CenterPos().y;
							if (d > 0 && d < down_nearest_curr) {
								down_nearest_curr = d;
								nearest = &atoms[i];
							}
						}
						else if (event.key.code == sf::Keyboard::Down) {
							float d = selectedAtom->CenterPos().y - atoms[i].CenterPos().y;
							if (d < 0 && abs(d) < abs(down_nearest_curr)) {
								down_nearest_curr = d;
								nearest = &atoms[i];
							}
						}
						else if (event.key.code == sf::Keyboard::Left) {
							float d = selectedAtom->CenterPos().x - atoms[i].CenterPos().x;
							if (d > 0 && d < right_nearest_curr) {
								right_nearest_curr = d;
								nearest = &atoms[i];
							}

						}
						else {
							float d = selectedAtom->CenterPos().x - atoms[i].CenterPos().x;
							if (d < 0 && abs(d) < abs(right_nearest_curr)) {
								right_nearest_curr = d;
								nearest = &atoms[i];
							}
						}
					}
					selectedAtom->UnSelect();
					selectedAtom = nearest->Select();
				}

			}
		}
		app.clear(sf::Color::White);

		for (int i = 0; i < atoms.size(); i++)
			atoms[i].Draw(&app);

		app.display();
	}
	return 0;
}