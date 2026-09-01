#include "Map.h"

Map::Map() {
	if (!map_texture.loadFromFile("assets/Terrain/Tileset/Tilemap_color2.png")) {
		// map failed to load 
	}

	current_frame = sf::IntRect({ 15, 15}, { 160, 160 });

	map_shape.setOrigin(sf::Vector2f{ 0, 0 });
	map_shape.setSize(sf::Vector2f(constants::screen_width, constants::screen_height));
	map_shape.setTexture(&map_texture);
	map_shape.setFillColor(sf::Color::White);
	map_shape.setTextureRect(current_frame);
	

}

void Map::draw(sf::RenderWindow& window) const {
	window.draw(map_shape);
}