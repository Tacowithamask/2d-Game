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

	int getHealth() const;
	int getDamage() const;
	void takeDamage(int damage);
	bool isAlive() const;
	bool attackEnemy();

	bool isFacingRight() const;
	constants::borderCollision isOutOfWindow();

	float attack_range = 70.0f;
	float size_of_player = 45.0f;
	float player_speed = 3.0f;

private:
	sf::CircleShape player_shape;
	sf::Texture texture_idle, texture_run, texture_attack;
	int player_health = 100;
	int player_damage = 25;
	sf::Clock animation_clock;
	sf::Clock attack_clock;
	sf::IntRect current_frame;
	int frame_width = 192;
	int max_frames = 6;
	int current_state = -1;
	bool has_attacked = false;
	
};


