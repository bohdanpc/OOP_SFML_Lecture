#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.hpp"

enum class EnemyType { Chaser, Shooter };

class Enemy {
public:
    Enemy(sf::Vector2f startPos, EnemyType type);

    void update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets);
    void draw(sf::RenderWindow& window) const;

    void takeDamage(int amount);
    bool isAlive() const { return m_health > 0; }

    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

private:
    sf::CircleShape m_shape;
    EnemyType m_type;
    float m_speed;
    int m_health;
    float m_shootCooldown = 0.f;
    float m_hitFlashTimer = 0.f;

    void updateColor();
};