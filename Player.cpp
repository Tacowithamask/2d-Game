#include "Player.h"

Player::Player() 
{
    if (!player_texture.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Run.png")) {
        // Handle texture failure 
    }

	// SFML 3.1 syntax: {position}, {size}
	current_frame = sf::IntRect({ 0, 0 }, { 192, 192 });

	player_shape.setRadius(size_of_player);
	player_shape.setOrigin(sf::Vector2f{ size_of_player, size_of_player });
	player_shape.setPosition(sf::Vector2f{0 + size_of_player, 0 + size_of_player});
	player_shape.setTexture(&player_texture);
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
		attack_clock.start();
	}
	else {
		attack_clock.stop();
		attack_clock.reset();
	}
	
	if (animation_clock.getElapsedTime().asSeconds() >= 0.1f) {

		current_frame.position.x += 192; // each texture contains images of 192 width

		if (current_frame.position.x >= 192 * max_frames) {
			current_frame.position.x = 0;
		}

		player_shape.setTextureRect(current_frame);
		animation_clock.restart();

	}

	if (attack_clock.getElapsedTime().asSeconds() >= 0.4f) {
		attack_clock.stop();
		attack_clock.reset();
		this->setAnimationState(0); // 0 = IDLE
	}


}

void Player::setAnimationState(int state) {
	
	if (current_state == state) {
		return;
	}

	current_state = state;

	if (state == 0) {
		player_texture.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Idle.png");
		max_frames = 8;
	}
	else if (state == 1) {
		player_texture.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Run.png");
		max_frames = 6; // number of images in the file
	}
	else if (state == 2) {
		player_texture.loadFromFile("assets/Units/Blue Units/Warrior/Warrior_Attack1.png");
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