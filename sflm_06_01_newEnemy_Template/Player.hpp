#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

class Player {
public:
    Player(sf::Vector2f startPos);

    void handleInput();
    void update(float dt, unsigned int windowWidth, unsigned int windowHeight);
    void draw(sf::RenderWindow& window) const;

    // Інкапсуляція: гравець сам керує своїм HP та візуальною реакцією
    void takeDamage(int amount);
    bool isAlive() const { return m_health > 0; }
    void reset(sf::Vector2f startPos);

    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    int getHealth() const { return m_health; }

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_movement{0.f, 0.f};
    float m_speed = 300.f;
    int m_health = 200;
    float m_hitFlashTimer = 0.f;

    void updateColor();
};