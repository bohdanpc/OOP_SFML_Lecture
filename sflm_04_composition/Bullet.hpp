#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, bool isEnemy);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool isEnemy() const { return m_isEnemy; }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    void deactivate() { m_shape.setPosition({-100.f, -100.f}); }

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
    bool m_isEnemy;
};