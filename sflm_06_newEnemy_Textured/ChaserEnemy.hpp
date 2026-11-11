// ChaserEnemy.hpp
#pragma once
#include "Enemy.hpp"

class ChaserEnemy : public Enemy {
public:
    ChaserEnemy(sf::Vector2f startPos);

    void update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) override;
    void draw(sf::RenderWindow& window) const override;
    void takeDamage(int amount) override;
    bool isAlive() const override { return m_health > 0; }
    sf::FloatRect getBounds() const override { return m_sprite.getGlobalBounds(); }
    void onPlayerCollision(Player& player) override;

private:
    sf::Sprite m_sprite;
    float m_hitFlashTimer = 0.f;
    float m_wobbleTimer = 0.f;

    void updateColor();
};