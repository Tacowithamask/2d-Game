#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "CONSTANTS_H.h"

class Player
{
public:
	Player();

	void draw(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition();
	void move(const sf::Vector2f& direction);
	void moveWithKeyboard();

	void updateAnimation();
	void setAnimationState(int state);
	int getAnimationState() const;




	float attack_range = 70.0f;
	const float size_of_player = 45.0f;
	const float player_speed = 3.0f;

private:
	sf::CircleShape player_shape;
	sf::Texture player_texture;
	uint16_t player_health;
	sf::Clock animation_clock;
	sf::Clock attack_clock;
	sf::IntRect current_frame;
	int frame_width = 192;
	int max_frames = 6;
	int current_state = -1;

	
};


