// ChaserEnemy.cpp
#include "ChaserEnemy.hpp"
#include "TextureManager.hpp"
#include "Player.hpp"
#include <cmath>

ChaserEnemy::ChaserEnemy(sf::Vector2f startPos)
    : m_sprite(TextureManager::instance().getChaserTexture())
{
    m_sprite.setOrigin({24.f, 24.f});
    m_sprite.setPosition(startPos);

    float m_speed = 120.f;
    int m_health = 30;
}

void ChaserEnemy::takeDamage(int amount) {
    m_health -= amount;
    m_hitFlashTimer = 0.1f;
    if (m_health < 0) m_health = 0;
}

void ChaserEnemy::updateColor() {
    m_sprite.setColor(m_hitFlashTimer > 0.f ? sf::Color::White : sf::Color(255, 255, 255));
}

void ChaserEnemy::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& /*outBullets*/) {
    if (m_hitFlashTimer > 0.f) m_hitFlashTimer -= dt;
    updateColor();

    sf::Vector2f dir = playerPos - m_sprite.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist != 0.f) dir /= dist;

    m_sprite.move(dir * m_speed * dt);

    m_wobbleTimer += dt * 18.f;
    m_sprite.setRotation(sf::degrees(std::sin(m_wobbleTimer) * 16.f));
}

void ChaserEnemy::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void ChaserEnemy::onPlayerCollision(Player& player) {
    player.takeDamage(10); // ѕокусав гравц€, але продовжуЇ б≥гти за ним
}