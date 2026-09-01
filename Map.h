#pragma once
#include <SFML/Graphics.hpp>
#include "CONSTANTS_H.h"

class Map
{

public:
	Map();
	void draw(sf::RenderWindow& window) const;


private:
	sf::IntRect current_frame;
	sf::Texture map_texture;
	sf::RectangleShape map_shape;
};

