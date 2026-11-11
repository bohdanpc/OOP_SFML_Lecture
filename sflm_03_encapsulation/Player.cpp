#include "Player.hpp"
#include <cmath>

Player::Player(sf::Vector2f startPos) {
    m_shape.setRadius(20.f);
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setOrigin({20.f, 20.f});
    m_shape.setPosition(startPos);
}

void Player::handleInput() {
    m_movement = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))  m_movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))  m_movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))  m_movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))  m_movement.x += 1.f;

    const float len = std::sqrt(m_movement.x * m_movement.x + m_movement.y * m_movement.y);
    if (len != 0.f) m_movement /= len;
}

// Інкапсуляція: гравець сам керує своїм HP та візуальною реакцією
void Player::takeDamage(int amount) {
    m_health -= amount;
    if (m_health < 0) m_health = 0;
    m_hitFlashTimer = 0.15f; // Спалах при отриманні шкоди
}

void Player::reset(sf::Vector2f startPos) {
    m_health = 100;
    m_hitFlashTimer = 0.f;
    m_shape.setPosition(startPos);
    updateColor();
}

void Player::updateColor() {
    if (m_hitFlashTimer > 0.f) {
        m_shape.setFillColor(sf::Color::Red);
    } else {
        float hpRatio = static_cast<float>(m_health) / 100.f;
        m_shape.setFillColor(sf::Color(
            static_cast<std::uint8_t>(255 * (1.f - hpRatio * 0.7f)),
            static_cast<std::uint8_t>(255 * hpRatio),
            static_cast<std::uint8_t>(255 * hpRatio)
        ));
    }
}

void Player::update(float dt, unsigned int windowWidth, unsigned int windowHeight) {
    if (m_hitFlashTimer > 0.f) {
        m_hitFlashTimer -= dt;
    }
    updateColor();

    m_shape.move(m_movement * m_speed * dt);

    //Перевірка виходу за межі екрану
    sf::Vector2f pos = m_shape.getPosition();
    if (pos.x < 20.f) pos.x = 20.f;
    if (pos.x > windowWidth - 20.f) pos.x = windowWidth - 20.f;
    if (pos.y < 20.f) pos.y = 20.f;
    if (pos.y > windowHeight - 20.f) pos.y = windowHeight - 20.f;
    m_shape.setPosition(pos);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}