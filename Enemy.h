#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "CONSTANTS_H.h"
#include <iostream>

class Enemy
{

public:
	Enemy();
	void draw(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	float getAttackRange() const;
	void move(const sf::Vector2f& direction);
	void updateAnimation();
	void setAnimationState(int state); // 0 not moving, 1 moving
	sf::Vector2i getPosition();
	bool attackPlayer(); 
	void facePlayer(const sf::Vector2f& player_position);

private:
	const float size_of_enemy = 45.0f;
	sf::CircleShape enemy_shape;
	sf::Texture enemy_texture;
	sf::Clock animation_clock;
	sf::Clock attack_clock;
	sf::IntRect current_frame;
	int frame_width = 192;
	int max_frames = 6;
	int current_state = -1;
	float attack_range = 40.0f;
};


