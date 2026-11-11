#include "Bullet.hpp"
#include <cmath>

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, bool isEnemy)
    : m_isEnemy(isEnemy)
{
    float radius = isEnemy ? 5.f : 4.f;
    m_shape.setRadius(radius);
    m_shape.setOrigin({radius, radius});
    m_shape.setPosition(startPos);
    m_shape.setFillColor(isEnemy ? sf::Color::Yellow : sf::Color::White);

    sf::Vector2f dir = targetPos - startPos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f) dir /= len;

    float speed = isEnemy ? 250.f : 500.f;
    m_velocity = dir * speed;
}

void Bullet::update(float dt) {
    m_shape.move(m_velocity * dt);
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}