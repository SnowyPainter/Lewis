#include "chemical_formula.h"
#include "atom.h"
#include "Logger.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tuple>
#include <random>

namespace config {
    const sf::VideoMode videoMode(1248, 864);
    const sf::String title("Lewis");
    const int style = sf::Style::Close;
}

int main() {
    int padding = 160;
    std::default_random_engine gen;
    std::uniform_int_distribution<int> w(padding, config::videoMode.width - padding);
    std::uniform_int_distribution<int> h(padding, config::videoMode.height - padding);

    Logger l = Logger("main");
    bool prevKeyPressed = false;
    int keycode = 0;
    l.Log("Initializing RenderWindow and settings ... ");
    sf::RenderWindow app(config::videoMode, config::title, config::style);
    app.setFramerateLimit(60);
    app.setKeyRepeatEnabled(false);

    if (!AtomicWorld::font.loadFromFile("E:/C++/SFML/Lewis/nmr.ttf")) {
        l.Error("There's an error with loading nmr.ttf font");
    }

    std::string c = "C12H22O11";
    auto parsed = chemical_formula::Parse(c);
    std::vector<AtomicWorld::Atom> atoms;
    for (auto t : parsed) {
        std::cout << "원소 " << std::get<0>(t) << ", " << std::get<1>(t) << "개 생성됨" << std::endl;
        for (int i = 0; i < std::get<1>(t); i++) {
            atoms.push_back(AtomicWorld::Atom(std::get<0>(t), 60));
        }
    }
    for (int i = 0; i < atoms.size(); i++) {
        atoms[i].Move(w(gen), h(gen));
    }
    
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
            else if (event.type == sf::Event::KeyPressed && !prevKeyPressed) {
                keycode = event.key.code;
            }

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
        prevKeyPressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keycode);
        
        app.clear(sf::Color::Black);

        for (int i = 0; i < atoms.size(); i++)
            atoms[i].Draw(&app);

        app.display();
    }
	return 0;
}