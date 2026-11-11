// ChaserEnemy.cpp
#include "ChaserEnemy.hpp"
#include "Player.hpp"
#include <cmath>

ChaserEnemy::ChaserEnemy(sf::Vector2f startPos)
{
    m_health = 30;
    m_speed = 120.0f;

    m_shape.setRadius(18.f);
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setOrigin({18.f, 18.f});
    m_shape.setPosition(startPos);
}

void ChaserEnemy::takeDamage(int amount) {
    m_health -= amount;
    m_hitFlashTimer = 0.1f;
    if (m_health < 0) m_health = 0;
}

void ChaserEnemy::updateColor() {
    m_shape.setFillColor(m_hitFlashTimer > 0.f ? sf::Color::White : sf::Color::Red);
}

void ChaserEnemy::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& /*outBullets*/) {
    if (m_hitFlashTimer > 0.f) m_hitFlashTimer -= dt;
    updateColor();

    sf::Vector2f dir = playerPos - m_shape.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist != 0.f) dir /= dist;

    m_shape.move(dir * m_speed * dt);
}

void ChaserEnemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

// Переслідувач б'є гравця, але продовжує переслідування
void ChaserEnemy::onPlayerCollision(Player& player) {
    player.takeDamage(10);
}