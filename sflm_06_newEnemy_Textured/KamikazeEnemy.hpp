#pragma once
#include "Enemy.hpp"
#include "SFML/System.hpp"

class KamikazeEnemy : public Enemy {
public:
    KamikazeEnemy(sf::Vector2f startPos);

    void update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) override;
    void draw(sf::RenderWindow& window) const override;
    void takeDamage(int amount) override;
    bool isAlive() const override { return m_health > 0; }
    sf::FloatRect getBounds() const override { return m_shape.getGlobalBounds(); }
    void onPlayerCollision(Player& player) override;

private:
    sf::CircleShape m_shape;
    float m_hitFlashTimer = 0.f;
    float m_rotationSpeed = 400.f;
    
    float m_selfDestructTime = 3.0f;
    float m_selfDestructDistance = 200.0f;

    void updateSelfDestruction(float distanceToPlayer);
    void updateColor();

    sf::Clock m_selfDestructTimer;
};