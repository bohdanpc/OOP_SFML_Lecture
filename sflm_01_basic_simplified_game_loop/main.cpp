#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

int main()
{
    const unsigned int WINDOW_WIDTH = 800;
    const unsigned int WINDOW_HEIGHT = 600;

    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "01 - SFML Game Loop");
    window.setFramerateLimit(60);

    sf::CircleShape player(20.f);
    player.setFillColor(sf::Color::Cyan);

    player.setOrigin({ 20.f, 20.f });
    player.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });

    const float playerSpeed = 10.f;
    sf::Clock clock;

    //GAME LOOP
    while (window.isOpen())
    {
        //Обробка подій ігрового вікна
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        //(Input Processing) Обробка інпут девайсів
        sf::Vector2f movement{ 0.f, 0.f };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += 1.f;

        //Game Update (оновлення ігрової логіки)
        player.move(movement * playerSpeed);

        window.clear(sf::Color(25, 25, 30));

        //Рендер
        window.draw(player);
        window.display();
    }

    return 0;
}