#include "Enemy.h"

Enemy::Enemy() {
    // Load the full texture sheet
    if (!enemy_texture.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Run.png")) {
        // Handle texture failure 
    }

    // SFML 3.1 syntax: {position}, {size}
    current_frame = sf::IntRect({ 0, 0 }, { 192, 192 });

    enemy_shape.setRadius(size_of_enemy);
	enemy_shape.setOrigin(sf::Vector2f{ size_of_enemy, size_of_enemy });
    enemy_shape.setFillColor(sf::Color::White);
    enemy_shape.setPosition(sf::Vector2f(constants::screen_width / 2, constants::screen_height / 2));

    enemy_shape.setTexture(&enemy_texture);
    enemy_shape.setTextureRect(current_frame);

    attack_clock.stop();
    attack_clock.reset();

}

void Enemy::draw(sf::RenderWindow& window) const
{
	window.draw(enemy_shape);
}

float Enemy::getAttackRange() const
{
    return attack_range;
}

void Enemy::setPosition(const sf::Vector2f& position)
{
	enemy_shape.setPosition(position);
}

sf::Vector2i Enemy::getPosition()
{
	return sf::Vector2i(enemy_shape.getPosition());
}

void Enemy::move(const sf::Vector2f& direction)
{
	enemy_shape.move(direction);
}

void Enemy::updateAnimation() {

    if (current_state == 2)
	attack_clock.start(); // start the  timer for attack animation
    else {
        attack_clock.stop(); // stop the timer for attack animation
        attack_clock.reset();
    }
    // Advance the frame every 0.1 seconds
    if (animation_clock.getElapsedTime().asSeconds() > 0.1f) {
        // Access 'x' via the new position vector
        current_frame.position.x += 192;

        // Loop back to the start if we reach the end of the sheet
        if (current_frame.position.x >= 192 * max_frames) {
            current_frame.position.x = 0;
        }

        enemy_shape.setTextureRect(current_frame);
        animation_clock.restart();
    }

    if (attack_clock.getElapsedTime().asSeconds() >= 0.35f) {
        attack_clock.restart();
    }
	std::cout << "Attack Clock: " << attack_clock.getElapsedTime().asSeconds() << std::endl;
	std::cout << "Animation Clock: " << animation_clock.getElapsedTime().asSeconds() << std::endl;
}

void Enemy::setAnimationState(int state) {
    // Don't reload the texture if we are already in this state!
    if (current_state == state) return;

    current_state = state;

    if (state == 0) {
        enemy_texture.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Idle.png");
        max_frames = 6; // Adjust this if Idle has a different number of frames
    }
    else if (state == 1) {
        enemy_texture.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Run.png");
        max_frames = 6; // Adjust this if Run has a different number of frames
    }
    else if (state == 2) {
        enemy_texture.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Attack1.png");
        max_frames = 4; // Adjust this if Attack has a different number of frames
	}
    // Reset the frame position back to the start whenever the animation changes
    current_frame.position.x = 0;
    enemy_shape.setTextureRect(current_frame);
}

bool Enemy::attackPlayer() {

    if (attack_clock.getElapsedTime().asSeconds()>= 0.3f) {
        return true;
	}

    return false;
}

void Enemy::facePlayer(const sf::Vector2f& player_position) {
    sf::Vector2f enemy_position = enemy_shape.getPosition();
    sf::Vector2f direction = player_position - enemy_position;
    sf::Angle angle = sf::degrees(std::atan2(direction.y, direction.x) * 180 / constants::pi);

    if (angle.asDegrees() > 90 || angle.asDegrees() < -90) {
        enemy_shape.setScale({ 1.f, -1.f }); // Flip vertically 
    } else {
        enemy_shape.setScale({1.f, 1.f}); // Normal orientation
	}
    enemy_shape.setRotation(angle);
}