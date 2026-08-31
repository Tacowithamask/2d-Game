#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "CONSTANTS_H.h"
#include "Enemy.h"
#include "Pointer.h"
#include "Player.h"
#include <iostream>

int main()
{

    sf::RenderWindow window(sf::VideoMode({constants::screen_width, constants::screen_height}), "");
	window.setFramerateLimit(60);

    Enemy enemy;
    Player player;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Calculate the distance between the enemy and the player
        sf::Vector2f enemy_to_player = player.getPosition() - enemy.getPosition();
        float enemy_distance = std::sqrt(enemy_to_player.x * enemy_to_player.x + enemy_to_player.y * enemy_to_player.y);

        // Check if the distance is within the enemy's attack range (40.0f)
        if (enemy_distance <= enemy.getAttackRange()) {
            enemy.setAnimationState(2); // 2 = ATTACK

            // Trigger the attack damage logic
            if (enemy.attackPlayer()) {
                std::cout << "Player hit by enemy!" << std::endl;
            }
        }
        else {
            // Move if out of range
            enemy.moveTowardsPlayer(player.getPosition());
        }

        enemy.facePlayer(player.getPosition());
        enemy.updateAnimation();

        bool isPlayerAttacking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        if (isPlayerAttacking && player.getAnimationState() != 2) {

			player.setAnimationState(2); // 2 = ATTACK

			sf::Vector2f dist_vec = enemy.getPosition() - player.getPosition();
			float distance = dist_vec.length();

            if (distance <= player.attack_range ){
				std::cout << "Enemy player hit " << std::endl;
            }
        }
        
        player.updateAnimation();
        player.moveWithKeyboard();

        window.clear();

        player.draw(window);
        enemy.draw(window);

        window.display();
    }
}