// ShooterEnemy.cpp
#include "ShooterEnemy.hpp"
#include "TextureManager.hpp"
#include "Player.hpp"
#include <cmath>


ShooterEnemy::ShooterEnemy(sf::Vector2f startPos)
    : m_sprite(TextureManager::instance().getShooterTexture())
{
    m_sprite.setOrigin({24.f, 24.f});
    m_sprite.setPosition(startPos);

    float m_speed = 70.f;
    int m_health = 50;
}

void ShooterEnemy::takeDamage(int amount) {
    m_health -= amount;
    m_hitFlashTimer = 0.1f;
    if (m_health < 0) m_health = 0;
}

void ShooterEnemy::updateColor() {
    m_sprite.setColor(m_hitFlashTimer > 0.f ? sf::Color::White : sf::Color(255, 255, 255));
}

void ShooterEnemy::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) {
    if (m_hitFlashTimer > 0.f) m_hitFlashTimer -= dt;
    updateColor();

    sf::Vector2f dir = playerPos - m_sprite.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist != 0.f) dir /= dist;

    if (dist > 250.f) {
        m_sprite.move(dir * m_speed * dt);
    }

    m_shootCooldown -= dt;
    if (m_shootCooldown <= 0.f) {
        outBullets.emplace_back(m_sprite.getPosition(), playerPos, true);
        m_shootCooldown = 2.0f;
    }
}

void ShooterEnemy::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void ShooterEnemy::onPlayerCollision(Player& player) {
    player.takeDamage(5); // Слабкий контактний урон
}