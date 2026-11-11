#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(sf::Vector2f startPos, EnemyType type)
    : m_type(type)
{
    if (m_type == EnemyType::Chaser) {
        m_shape.setRadius(18.f);
        m_speed = 120.f;
        m_health = 30;
    } else {
        m_shape.setRadius(22.f);
        m_speed = 70.f;
        m_health = 50;
        m_shootCooldown = 1.0f;
    }

    m_shape.setOrigin({m_shape.getRadius(), m_shape.getRadius()});
    m_shape.setPosition(startPos);
    updateColor();
}

void Enemy::takeDamage(int amount) {
    m_health -= amount;
    m_hitFlashTimer = 0.1f;
    if (m_health < 0) m_health = 0;
}

void Enemy::updateColor() {
    if (m_hitFlashTimer > 0.f) {
        m_shape.setFillColor(sf::Color::White);
    } else {
        m_shape.setFillColor(m_type == EnemyType::Chaser ? sf::Color::Red : sf::Color::Magenta);
    }
}

void Enemy::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) {
    if (m_hitFlashTimer > 0.f) {
        m_hitFlashTimer -= dt;
    }
    updateColor();

    sf::Vector2f dir = playerPos - m_shape.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist != 0.f) dir /= dist;

    if (m_type == EnemyType::Chaser) {
        m_shape.move(dir * m_speed * dt);
    } else if (m_type == EnemyType::Shooter) {
        if (dist > 250.f) {
            m_shape.move(dir * m_speed * dt);
        }
        m_shootCooldown -= dt;
        if (m_shootCooldown <= 0.f) {
            outBullets.emplace_back(m_shape.getPosition(), playerPos, true);
            m_shootCooldown = 2.0f;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}