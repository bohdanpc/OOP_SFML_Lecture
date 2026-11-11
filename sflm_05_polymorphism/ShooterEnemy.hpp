// ShooterEnemy.hpp
#pragma once
#include "Enemy.hpp"

class ShooterEnemy : public Enemy {
public:
    ShooterEnemy(sf::Vector2f startPos);

    void update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) override;
    void draw(sf::RenderWindow& window) const override;
    void takeDamage(int amount) override;
    bool isAlive() const override { return m_health > 0; }
    sf::FloatRect getBounds() const override { return m_shape.getGlobalBounds(); }

    void onPlayerCollision(Player& player) override;

private:
    sf::CircleShape m_shape;
    float m_shootCooldown = 1.0f;
    float m_hitFlashTimer = 0.f;

    void updateColor();
};