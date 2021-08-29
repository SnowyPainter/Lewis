#pragma warning( disable : 4244)
#pragma warning( disable : 26451)

#include "chemical_formula.h"
#include "atom.h"
#include "molecule_connect.h"
#include "Logger.h"
#include "hsv.h"
#include "dictionary.h"
#include "Gui/posboost.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tuple>
#include <random>
#include <cctype>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

std::string formattedInfo(std::string name, int number, int om, float ng) {
	return "Symbol : " + name + "\nAtomic Number["
		+ std::to_string(number) + "]\nElectronegativity\n"
		+ std::to_string(ng) + "\nOutermosts["
		+ std::to_string(om) + "]";
}

namespace config {
	const sf::VideoMode videoMode(1600, 864);
	sf::String title("Lewis");
	const int style = sf::Style::Close;
	const int standard_raidus = 24;
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

enum class Id {
	AtomDetailText,
	CurrentState,

	Count,
};

int main() {
	Logger l = Logger("main");
	//release : E:/C++/SFML/Lewis/RampartOne-Regular.ttf
	if (!sf::font.loadFromFile("E:/C++/SFML/Lewis/RampartOne-Regular.ttf")) {
		l.Error("There's an error with loading RobotoMono-Light.ttf font");
	}

	bool infoDisplayed = false;
	bool bonding = false;
	int padding = 160;
	std::random_device rseed;
	std::mt19937 gen(rseed());
	std::uniform_int_distribution<int> w(padding, config::videoMode.width - padding);
	std::uniform_int_distribution<int> h(padding, config::videoMode.height - padding);
	std::uniform_int_distribution<int> hue(19, 320);
	
	molecule_connect moleculeConnector;
	sf::Atom* pseudoBondStart = nullptr, * pseudoBondEnd = nullptr;
	std::map<Id,sf::Text> Text;

	for (int i = 0; i < (int)Id::Count; i++) {
		sf::Text t;
		t.setPosition(0, 0);
		t.setFont(sf::font);
		t.setFillColor(sf::Color::Black);
		t.setCharacterSize(15);
		Text[(Id)i] = t;
	}
	Text[Id::CurrentState].setCharacterSize(30);
	Text[Id::CurrentState].setPosition(config::videoMode.width / 2, 0);

	l.Log("This program only supports defined 1~18th atoms");
	l.Log("Please write down a Chemical Formula.");

	std::string input = "O2 + H2"; //Example
	// std::getline(std::cin, c);
	
	l.Log("Initializing RenderWindow and settings ... ");
	config::title += " " + input;
	sf::RenderWindow app(config::videoMode, config::title, config::style);
	app.setFramerateLimit(60);
	app.setKeyRepeatEnabled(false);

	l.Log("Parsing Chemical Formula to some tuples ... ");

	std::map<std::string, std::vector<std::tuple<std::string, int>>> parsed_formulas;
	std::vector<sf::Atom> atoms;
	auto chemical_formulas = split(input, '+');
	for (int i = 0; i < chemical_formulas.size(); i++) {
		chemical_formulas[i] = trim(chemical_formulas[i]);
		parsed_formulas.insert({ chemical_formulas[i], chemical_formula::Parse(chemical_formulas[i]) });
		auto parsed = parsed_formulas[chemical_formulas[i]];
		std::cout << "Parsed chemical formula : " << chemical_formulas[i] << std::endl;

		sf::Color colorTheme = sf::hsv(hue(gen), 100, 100);

		for (int j = 0; j < parsed.size(); j++) {
			std::cout << "Atom named : " << std::get<0>(parsed[j]) << ", Generated " << std::get<1>(parsed[j]) << std::endl;
			for (int k = 0; k < std::get<1>(parsed[j]); k++) {
				auto symbol = std::get<0>(parsed[j]);
				auto atom = sf::Atom(symbol, electron::Negativity[electron::Number[symbol]] * config::standard_raidus, colorTheme);
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
				if (event.key.code == sf::Keyboard::Enter) {
					std::cout << "Currently, Molecules" << std::endl;
					for (auto m : moleculeConnector.GetMolecules()) {
						std::cout << chemical_formula::Frame(m) << std::endl;
					}
				}
				else if (event.key.code == sf::Keyboard::B) {
					Text[Id::CurrentState].setString("Bonding");
					if (!bonding) {
						pseudoBondStart = selectedAtom;
						pseudoBondEnd = nullptr;
					}
					bonding = true;
				}
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
					infoDisplayed = false;
					Text[Id::CurrentState].setString(selectedAtom->GetSymbol());
				}

			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::B) {
					Text[Id::CurrentState].setString("Complete");
					bonding = false;
					pseudoBondEnd = selectedAtom;
					if (pseudoBondStart != nullptr && pseudoBondEnd != nullptr) {
						moleculeConnector.AddPair(pseudoBondStart, pseudoBondEnd);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				auto p = sf::Mouse::getPosition(app);
				selectedAtom->Move(p.x, p.y);
				Text[Id::AtomDetailText].setPosition(selectedAtom->GetDetailTextPosition());
			}

			//Show detail of atom
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::I) {
				if (infoDisplayed)
					infoDisplayed = false;
				else {
					int number = selectedAtom->GetAtomNumber();
					if (number == -1) {
						l.Error("Atom name is unable to use, Program shutdown");
						return -1;
					}
					int outermosts = electron::GetOutermosts(number);
					float negativity = electron::Negativity[number];
					auto text = formattedInfo(selectedAtom->GetSymbol(), number, outermosts, negativity);
					Text[Id::AtomDetailText].setString(text);
					Text[Id::AtomDetailText].setPosition(selectedAtom->GetDetailTextPosition());
					infoDisplayed = true;
				}
			}
		}

		app.clear(sf::Color::White);
		
		moleculeConnector.Draw(&app);

		for (int i = 0; i < atoms.size(); i++)
			atoms[i].Draw(&app);

		if (infoDisplayed)
			app.draw(Text[Id::AtomDetailText]);
		for (int i = 0; i < (int)Id::Count; i++) {
			if ((Id)i == Id::AtomDetailText) continue;

			app.draw(Text[(Id)i]);
		}

		app.display();
	}
	return 0;
}