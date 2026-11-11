#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include "Bullet.hpp"

// Forward declaration, ùîá íå áóëî öèêë³÷íèõ ³íêëóä³â
class Player;

class Enemy {
public:
    virtual ~Enemy() = default;

    virtual void update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& outBullets) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void takeDamage(int amount) = 0;
    virtual bool isAlive() const = 0;
    virtual sf::FloatRect getBounds() const = 0;

    // ÏÎË²ÌÎĞÔÍÀ ĞÅÀÊÖ²ß ÍÀ Ç²ÒÊÍÅÍÍß Ç ÃĞÀÂÖÅÌ
    virtual void onPlayerCollision(Player& player) = 0;

protected:
    float m_health = 100.0f;
    float m_speed = 100.0f;
};