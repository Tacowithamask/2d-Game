#include "Enemy.h"

Enemy::Enemy() {
        
    if (!texture_idle.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Idle.png")) {
        // Handle texture failure 
	}
    if (!texture_run.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Run.png")) {
        // Handle texture failure 
	}
    if (!texture_attack.loadFromFile("assets/Units/Purple Units/Warrior/Warrior_Attack1.png")) {
        // Handle texture failure 
	}   

    // SFML 3.1 syntax: {position}, {size}
    current_frame = sf::IntRect({ 0, 0 }, { 192, 192 });

    enemy_shape.setRadius(size_of_enemy);
	enemy_shape.setOrigin(sf::Vector2f{ size_of_enemy, size_of_enemy });
    enemy_shape.setFillColor(sf::Color::White);
    enemy_shape.setPosition(sf::Vector2f(constants::screen_width / 2, constants::screen_height / 2));

    enemy_shape.setTexture(&texture_idle);
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

sf::Vector2f Enemy::getPosition()
{
	return sf::Vector2f(enemy_shape.getPosition());
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
		has_attacked = false;   // Reset the attack flag if not attacking
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
		has_attacked = false; // Reset the attack flag after the attack animation duration
	}
}

void Enemy::setAnimationState(int state) {
    // Don't reload the texture if we are already in this state!
    if (current_state == state) return;

    current_state = state;

    if (state == 0) {
        enemy_shape.setTexture(&texture_idle);
        max_frames = 8; // Adjust this if Idle has a different number of frames
    }
    else if (state == 1) {
        enemy_shape.setTexture(&texture_run);
        max_frames = 6; // Adjust this if Run has a different number of frames
    }
    else if (state == 2) {
        enemy_shape.setTexture(&texture_attack);
        max_frames = 4; // Adjust this if Attack has a different number of frames
	}
    // Reset the frame position back to the start whenever the animation changes
    current_frame.position.x = 0;
    enemy_shape.setTextureRect(current_frame);
}

bool Enemy::attackPlayer() {

    if (attack_clock.getElapsedTime().asSeconds()>= 0.3f && !has_attacked) {
        has_attacked = true;
        return true;
	}

    return false;
}

void Enemy::facePlayer(const sf::Vector2f& player_position) {
    sf::Vector2f enemy_position = enemy_shape.getPosition();
    sf::Vector2f direction = player_position - enemy_position;
    if ( direction.x < 0) {
        enemy_shape.setScale({ -1.0f, 1.0f }); // Flip horizontally
    }
    else {
        enemy_shape.setScale({ 1.0f, 1.0f }); // Normal orientation
	}
}

void Enemy::moveTowardsPlayer(const sf::Vector2f& player_position, const std::vector<std::unique_ptr<Enemy>>& enemies) {
	// give weight to the direction vector based on the distance to the player, and also avoid overlapping with other enemies
	sf::Vector2f direction_to_player = player_position - enemy_shape.getPosition();
	float direction_length = std::sqrt(direction_to_player.x * direction_to_player.x + direction_to_player.y * direction_to_player.y);

    if (direction_length != 0) {
        direction_to_player /= direction_length; // Normalize the direction
    }

	sf::Vector2f seperation_vector{ 0.0f, 0.0f };
	int nearby_count = 0;
	float seperation_distance = 50.0f; // Minimum distance to maintain from other enemies 

    for ( const auto& other_enemy : enemies) {
        if (other_enemy.get() != this) { // Don't compare with itself
            sf::Vector2f to_other = other_enemy->getPosition() - enemy_shape.getPosition();
            float distance_to_other = std::sqrt(to_other.x * to_other.x + to_other.y * to_other.y);
            if (distance_to_other < seperation_distance && distance_to_other > 0) {
                seperation_vector -= (to_other / distance_to_other); // Move away from the other enemy
                nearby_count++;
            }
        }
	}

    if (nearby_count > 0) {
        seperation_vector /= static_cast<float>(nearby_count); // Average the separation vector
	}

	float seperation_length = std::sqrt(seperation_vector.x * seperation_vector.x + seperation_vector.y * seperation_vector.y);
    if (seperation_length != 0) {
        seperation_vector /= seperation_length; // Normalize the separation vector
	}

	float weight_to_player = 1.3f; // Weight for moving towards the player
	sf::Vector2f final_direction = (direction_to_player * weight_to_player) + seperation_vector;
    float final_length = std::sqrt(final_direction.x * final_direction.x + final_direction.y * final_direction.y);
    if (final_length != 0) {
		this->setAnimationState(1); // Set to moving state
        final_direction /= final_length; // Normalize the final direction
    }
    else {
		this->setAnimationState(0); // Set to idle state if no movement
    }
	enemy_shape.move(final_direction * enemy_speed);

}

int Enemy::getHealth() const {
    return enemy_health;
}

int Enemy::getDamage() const {
    return attack_damage;
}

void Enemy::takeDamage(int damage) {
    enemy_health -= damage;
    if (enemy_health < 0) enemy_health = 0;
}

bool Enemy::isAlive() const {
    return enemy_health > 0;
}