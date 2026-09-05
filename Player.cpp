#include "Player.h"

Player::Player() 
{
	if (!texture_idle.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Idle.png")) {
        // Handle texture failure 
    }

	if (!texture_run.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Run.png")) {
        // Handle texture failure 
    }

	if (!texture_attack.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Attack1.png")) {
        // Handle texture failure 
    }

	// SFML 3.1 syntax: {position}, {size}
	current_frame = sf::IntRect({ 0, 0 }, { 192, 192 });

	player_shape.setRadius(size_of_player);
	player_shape.setOrigin(sf::Vector2f{ size_of_player, size_of_player });
	player_shape.setPosition(sf::Vector2f{0 + size_of_player, 0 + size_of_player});
	player_shape.setTexture(&texture_idle);
	player_shape.setTextureRect(current_frame);

	attack_clock.stop();
	attack_clock.reset();
}

void Player::draw(sf::RenderWindow& window) const
{
	window.draw(player_shape);
}

void Player::setPosition(const sf::Vector2f& position)
{
	player_shape.setPosition(position);
}

sf::Vector2f Player::getPosition()
{
	return sf::Vector2f(player_shape.getPosition());
}

void Player::move(const sf::Vector2f& direction) {
	player_shape.move(direction);
}

void Player::updateAnimation() {

	if (current_state == 2) {
		attack_clock.start(); // start the timer for attack animation
	}
	else {
		attack_clock.stop();
		attack_clock.reset();
		has_attacked = false; // Reset the attack flag if not attacking
	}
	
	if (animation_clock.getElapsedTime().asSeconds() >= 0.1f) {

		current_frame.position.x += 192; // each texture contains images of 192 width

		if (current_frame.position.x >= 192 * max_frames) {
			current_frame.position.x = 0;
		}

		player_shape.setTextureRect(current_frame);
		animation_clock.restart();

	}

	if (attack_clock.getElapsedTime().asSeconds() >= 0.35f) {
		attack_clock.restart();
		has_attacked = false; // Reset the attack flag after the attack animation duration
		this->setAnimationState(0); // Return to idle state after attack
	}


}

void Player::setAnimationState(int state) {
	
	if (current_state == state) {
		return;
	}

	current_state = state;

	if (state == 0) {
		player_shape.setTexture(&texture_idle);
		max_frames = 8;
	}
	else if (state == 1) {
		player_shape.setTexture(&texture_run);
		max_frames = 6; // number of images in the file
	}
	else if (state == 2) {
		player_shape.setTexture(&texture_attack);
		max_frames = 4;
	}

	current_frame.position.x = 0;
	player_shape.setTextureRect(current_frame);

}

void Player::moveWithKeyboard() {

	sf::Vector2f  direction{ 0.0f, 0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		direction.y -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		direction.y += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		direction.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		direction.x += 1.0f;
	}

	if (direction.x != 0.0f || direction.y != 0.0f) {

		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction /= length; // Normalize the direction vector

		if (direction.x < 0) player_shape.setScale({ -1.0f,  1.0f });
		else player_shape.setScale({ 1.0f, 1.0f });

		this->move(direction * player_speed);

		if (current_state != 2) {
			this->setAnimationState(1); // 1 = RUN
		}
	}
	else {

		if (current_state != 2) {
			this->setAnimationState(0); // 0 = IDLE
		}
	}
}

int Player::getAnimationState() const {
	return current_state;
}

int Player::getHealth() const {
	return player_health;
}

int Player::getDamage() const {
	return player_damage;
}	

void Player::takeDamage(int damage) {
	player_health -= damage;
	if (player_health < 0) player_health = 0;
}

bool Player::isAlive() const {
	return player_health > 0;
}

bool Player::attackEnemy() {
	// Deal damage at the 0.20 second mark of the animation
	if (attack_clock.getElapsedTime().asSeconds() >= 0.2f && !has_attacked) {
		has_attacked = true; // Prevent multi-hit bug
		return true;
	}
	return false;
}

bool Player::isFacingRight() const {
	// If the scale is greater than 0, the player is facing right.
	return player_shape.getScale().x > 0.0f;
}