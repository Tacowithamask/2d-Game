#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "CONSTANTS_H.h"
#include <iostream>
#include <vector>
#include <memory>

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
	sf::Vector2f getPosition();
	bool attackPlayer(); 
	void moveTowardsPlayer(const sf::Vector2f& player_position, const std::vector<std::unique_ptr<Enemy>>& enemies);
	void facePlayer(const sf::Vector2f& player_position);

	int getHealth() const;
	int getDamage() const;
	void takeDamage(int damage);
	bool isAlive() const;

	float enemy_speed = 1.0f;

private:
	float size_of_enemy = 45.0f;
	int enemy_health = 50;
	int attack_damage = 10;
	sf::CircleShape enemy_shape;
	sf::Texture texture_idle, texture_run, texture_attack;
	sf::Clock animation_clock;
	sf::Clock attack_clock;
	sf::IntRect current_frame;
	int frame_width = 192;
	int max_frames = 6;
	int current_state = -1;
	float attack_range = 40.0f;
	bool has_attacked = false;
};


