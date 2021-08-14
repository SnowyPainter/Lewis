#include "chfparse.h"
#include "Logger.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tuple>

namespace config {
    const sf::VideoMode videoMode(1248, 864);
    const sf::String title("Lewis");
    const int style = sf::Style::Close;
}

int main() {
    Logger l = Logger("main");
    
    l.Log("Initializing RenderWindow and settings ... ");
    sf::RenderWindow app(config::videoMode, config::title, config::style);
    app.setFramerateLimit(60);

    /* for (auto t : chfparse::Parse("C2H5OH")) {
        std::cout << std::get<0>(t) << ", " << std::get<1>(t) << std::endl;
    } */


    bool prevKeyPressed = false;
    int keycode = 0;
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
        }

        prevKeyPressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keycode);
        
        app.clear(sf::Color::Black);

        app.display();
    }
	return 0;
}