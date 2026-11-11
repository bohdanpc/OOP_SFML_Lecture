#include "KamikazeEnemy.hpp"
#include "Player.hpp"
#include <cmath>

#pragma optimize("", off)

KamikazeEnemy::KamikazeEnemy(sf::Vector2f startPos) {
    m_shape.setRadius(14.f);
    m_shape.setPointCount(6); // Смішний шестикутник-пилка
    m_shape.setFillColor(sf::Color(255, 140, 0));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::Yellow);
    m_shape.setOrigin({14.f, 14.f});
    m_shape.setPosition(startPos);

    m_selfDestructTimer.reset();

    m_speed = 220.f;
    m_health = 15;
}

void KamikazeEnemy::takeDamage(int amount) {
    m_health -= amount;
    m_hitFlashTimer = 0.1f;
    if (m_health < 0) m_health = 0;
}

void KamikazeEnemy::updateColor() {
    m_shape.setFillColor(m_hitFlashTimer > 0.f ? sf::Color::White : sf::Color(255, 140, 0));
}

void KamikazeEnemy::updateSelfDestruction(float distanceToPlayer) {
    if (!m_selfDestructTimer.isRunning() && distanceToPlayer < m_selfDestructDistance)
    {
        m_selfDestructTimer.start();
        m_shape.setOutlineThickness(10);
        m_shape.setOutlineColor(sf::Color::Magenta);
        return;
    }


    if (m_selfDestructTimer.getElapsedTime().asSeconds() > m_selfDestructTime)
        takeDamage(999);
}

void KamikazeEnemy::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& /*outBullets*/) {
    if (m_hitFlashTimer > 0.f) 
        m_hitFlashTimer -= dt;
    
    updateColor();

    sf::Vector2f dir = playerPos - m_shape.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    
    if (dist != 0.f) 
        dir /= dist;

    m_shape.move(dir * m_speed * dt);
    m_shape.rotate(sf::degrees(m_rotationSpeed * dt));

    updateSelfDestruction(dist);
}

void KamikazeEnemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

void KamikazeEnemy::onPlayerCollision(Player& player) {
    player.takeDamage(30); // Вибух наносить відчутну шкоду
    takeDamage(999);       // ТІЛЬКИ камікадзе сам себе знищує при тарані!
}