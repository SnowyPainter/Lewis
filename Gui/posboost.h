#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <tuple>

namespace boost {
	static sf::Vector2f bounds2vec(sf::FloatRect r) {
		return sf::Vector2f(r.width, r.height);
	}
	enum class StackOption {
		Horizontal,
		Vertical
	};
	enum class Horizontal {
		Left, Right, Center
	};
	enum class Vertical {
		Top, Bottom, Middle
	};

	class Stacker {
	private:
		std::function<sf::Vector2f(sf::Vector2f, sf::Vector2f)> allocate;
		std::vector<sf::Text*> stacked;
		int currp = 0;
	public:
		Stacker() {}
		Stacker(StackOption opt, int spacing) {
			if (opt == StackOption::Horizontal)
				allocate = [&p = currp, s = spacing](sf::Vector2f pos, sf::Vector2f size)->sf::Vector2f {
				pos = sf::Vector2f(p, pos.y);
				p = pos.x + size.x + s;
				return pos;
			};
			else
				allocate = [&p = currp, s = spacing](sf::Vector2f pos, sf::Vector2f size)->sf::Vector2f {
				pos = sf::Vector2f(pos.x, p);
				p = pos.y + size.y + s;
				return pos;
			};
		}
		void Stack(sf::Text* object) {			
			object->setPosition(allocate(object->getPosition(), bounds2vec(object->getGlobalBounds())));
			stacked.push_back(object);
		}
		std::vector<sf::Text*> GetAll() {
			return stacked;
		}
	};
}