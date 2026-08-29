#include "Pointer.h"

Pointer::Pointer()
{
	pointer_shape.setRadius(radius_of_pointer);
    pointer_shape.setFillColor(pointer_color);
    pointer_shape.setPosition(sf::Vector2f{ 0.0f, 0.0f });
	pointer_shape.setOrigin(sf::Vector2f{ radius_of_pointer, radius_of_pointer });
    pointer_health = 100;
}
    
void Pointer::draw(sf::RenderWindow& window) const
{
    window.draw(pointer_shape);
}

void Pointer::setPosition(const sf::Vector2f& position)
{
    pointer_shape.setPosition(position);
}

sf::Vector2i Pointer::getPosition(sf::RenderWindow& window)
{
	return sf::Mouse::getPosition(window);
}

uint16_t Pointer::gethealth() const
{
    return pointer_health;
}

void Pointer::updatehealth(int new_health)
{
    pointer_health = new_health;
}