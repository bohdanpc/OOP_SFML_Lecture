#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

#pragma optimize("", off)

int main()
{
    const unsigned int WINDOW_WIDTH = 1200;
    const unsigned int WINDOW_HEIGHT = 800;

    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "01 - SFML Game Loop");
    window.setFramerateLimit(60);

    sf::CircleShape player(20.f);
    player.setFillColor(sf::Color::Cyan);

    //player.setOrigin({ 20.f, 20.f });
    player.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });

    const float playerSpeed = 300.f;
    sf::Clock clock;

    while (window.isOpen())
    {
        const float dt = clock.restart().asSeconds();

        //Input and window/system events handling
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        sf::Vector2f movement{ 0.f, 0.f };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            movement.x += 1.f;

        //Normalization process, so the player won't move faster in diagonals
        if (movement.x != 0.f || movement.y != 0.f)
        {
            const float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length; //so the length of the movement vector equals always equals 1
        }

        player.move(movement * playerSpeed * dt);

        //Windows border
        sf::Vector2f pos = player.getPosition();
        if (pos.x < 20.f) pos.x = 20.f;
        if (pos.x > WINDOW_WIDTH - 20.f) pos.x = WINDOW_WIDTH - 20.f;
        if (pos.y < 20.f) pos.y = 20.f;
        if (pos.y > WINDOW_HEIGHT - 20.f) pos.y = WINDOW_HEIGHT - 20.f;
        player.setPosition(pos);

        //RENDER STEP
        window.clear(sf::Color(25, 25, 30));
        window.draw(player);

        window.display();
    }

    return 0;
}