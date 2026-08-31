#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "CONSTANTS_H.h"
class Pointer
{
public:
	Pointer();
	void draw(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	sf::Vector2i getPosition(sf::RenderWindow& window);
	void updatehealth(int new_health);
	uint16_t gethealth() const;
	void moveCursor(sf::RenderWindow& window);

private:
	const float radius_of_pointer = 4.0f;
	sf::CircleShape pointer_shape;
	sf::Color pointer_color = sf::Color::Blue;
	uint16_t pointer_health;

};


