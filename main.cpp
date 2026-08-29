#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "CONSTANTS_H.h"
#include "Enemy.h"
#include "pointer.h"
#include <iostream>

int main()
{

    sf::RenderWindow window(sf::VideoMode({constants::screen_width, constants::screen_height}), "");
	window.setFramerateLimit(60);

    Enemy enemy;
	Pointer pointer;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (pointer.gethealth() <= 0)
        {
            std::cout << "Game Over! Pointer Health: " << pointer.gethealth() << std::endl;
            window.close();
		}

		sf::Vector2i m_position = pointer.getPosition(window);
		 /* std::cout << "Mouse Position: (" << m_position.x << ", " << m_position.y << ")" << std::endl;
		 std::cout << "Enemy Position: (" << enemy.getPosition(sf::Vector2f()).x << ", " << enemy.getPosition(sf::Vector2f()).y << ")" << std::endl; */
         if (m_position.x >= 0 && m_position.x <= 720 && m_position.y >= 0 && m_position.y <= 720)
        {
             pointer.setPosition(sf::Vector2f(float(m_position.x)- 4.0f, float(m_position.y)- 4.0f));
	       } 

        sf::Vector2f r_vector = {
            static_cast<float>(m_position.x) - enemy.getPosition(sf::Vector2f()).x,
            static_cast<float>(m_position.y) - enemy.getPosition(sf::Vector2f()).y
		};
        if (r_vector.length() > enemy.getAttackRange())
        {

            enemy.move(r_vector.normalized());
            enemy.setAnimationState(1); // 1 = RUN
        }
        else
        {
            enemy.setAnimationState(2); // 2 = ATTACK
           
        }

        enemy.facePlayer(sf::Vector2f(float(m_position.x), float(m_position.y)));
        enemy.updateAnimation();

        if (enemy.attackPlayer())
        {
            pointer.updatehealth(0);
            std::cout << "Pointer Health: " << pointer.gethealth() << std::endl;
        }

        window.clear();

		pointer.draw(window);  

		
        if (r_vector.length() > 0.0f)
        enemy.move(r_vector.normalized());
		
        enemy.draw(window);

        window.display();
    }
}