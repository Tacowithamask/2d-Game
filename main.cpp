#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "CONSTANTS_H.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ constants::screen_width, constants::screen_height }), "SFML Game");
    window.setFramerateLimit(60);

    Map map;
    Player player;

    // 1. Create a Vector of Enemies
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 2. Spawn 3 enemies at different starting locations
    for (int i = 0; i < 3; ++i) {
        auto new_enemy = std::make_unique<Enemy>();
        // Space them out so they don't spawn on top of each other
        new_enemy->setPosition(sf::Vector2f(400.0f + (i * 80.0f), 300.0f + (i * 40.0f)));
        enemies.push_back(std::move(new_enemy));
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // -----------------------------------------------------------------
        // PLAYER LOGIC
        // -----------------------------------------------------------------

        if (player.isAlive())
        {
            player.moveWithKeyboard();
            player.updateAnimation();

            bool isPlayerAttacking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            if (isPlayerAttacking && player.getAnimationState() != 2)
            {
                player.setAnimationState(2);
            }

            // If the player lands a swing, check WHICH enemies get hit
            if (player.getAnimationState() == 2 && player.attackEnemy())
            {
				// Loop through EVERY enemy in the game, can probably be optimized later to only check nearby enemies
                for (auto& enemy : enemies)
                {
                    sf::Vector2f dist_vec = enemy->getPosition() - player.getPosition();

                    if (dist_vec.length() <= player.attack_range)
                    {
                        bool valid_direction = (player.isFacingRight() && dist_vec.x >= 0) ||
                            (!player.isFacingRight() && dist_vec.x <= 0);

                        if (valid_direction) {
                            enemy->takeDamage(player.getDamage());
                            std::cout << "Enemy hit! HP: " << enemy->getHealth() << std::endl;
                        }
                    }
                }
            }
        }

        // -----------------------------------------------------------------
        // ENEMY LOGIC
        // -----------------------------------------------------------------
     
        // Loop through and update every enemy independently
        for (auto& enemy : enemies)
        {
            if (player.isAlive())
            {
                sf::Vector2f enemy_to_player = player.getPosition() - enemy->getPosition();
                float enemy_distance = enemy_to_player.length();

                if (enemy_distance <= enemy->getAttackRange())
                {
                    enemy->setAnimationState(2); // ATTACK
                    if (enemy->attackPlayer())
                    {
                        player.takeDamage(enemy->getDamage());
                        std::cout << "Player hit! HP: " << player.getHealth() << std::endl;
                    }
                }
                else
                {
                    enemy->moveTowardsPlayer(player.getPosition());
                }

                enemy->facePlayer(player.getPosition());
            }

            enemy->updateAnimation();
        }

        // -----------------------------------------------------------------
        // GARBAGE COLLECTION (Remove Dead Enemies)
        // -----------------------------------------------------------------

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& e) {
                return !e->isAlive();
                }),
            enemies.end()
        );

        // -----------------------------------------------------------------
        // RENDER
        // -----------------------------------------------------------------

        window.clear();

        map.draw(window);

        if (player.isAlive()) {
            player.draw(window);
        }

        // Draw all remaining alive enemies
        for (const auto& enemy : enemies) {
            enemy->draw(window);
        }

        window.display();
    }

    return 0;
}