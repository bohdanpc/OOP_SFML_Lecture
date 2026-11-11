// ShooterEnemy.cpp
#include "ShooterEnemy.hpp"
#include "Player.hpp"
#include <cmath>

ShooterEnemy::ShooterEnemy(sf::Vector2f startPos)
{
    m_speed = 70.0f;
    m_health = 50;

    m_shape.setRadius(22.f);
    m_shape.setFillColor(sf::Color::Magenta);
    m_shape.setOrigin({22.f, 22.f});
    m_shape.setPosition(startPos);
}

void ShooterEnemy::takeDamage(int amount) {
    m_health -= amount;
    m_hitFlashTimer = 0.1f;
    if (m_health < 0) m_health = 0;
}

void ShooterEnemy::updateColor() {
    m_shape.setFillColor(m_hitFlashTimer > 0.f ? sf::Color::White : sf::Color::Magenta);
}

void ShooterEnemy::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) {
    if (m_hitFlashTimer > 0.f) m_hitFlashTimer -= dt;
    updateColor();

    sf::Vector2f dir = playerPos - m_shape.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist != 0.f) dir /= dist;

    if (dist > 250.f) {
        m_shape.move(dir * m_speed * dt);
    }

    m_shootCooldown -= dt;
    if (m_shootCooldown <= 0.f) {
        outBullets.emplace_back(m_shape.getPosition(), playerPos, true);
        m_shootCooldown = 2.0f;
    }
}

void ShooterEnemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

void ShooterEnemy::onPlayerCollision(Player& player) {
    player.takeDamage(5);
    m_health = 0;
}