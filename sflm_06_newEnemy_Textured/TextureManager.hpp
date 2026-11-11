#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

// Генератор процедурних текстур у пам'яті (без потреби завантажувати зовнішні PNG)
class TextureManager {
public:
    static TextureManager& instance() {
        static TextureManager inst;
        return inst;
    }

    const sf::Texture& getPlayerTexture() const { return m_playerTex; }
    const sf::Texture& getChaserTexture() const { return m_chaserTex; }
    const sf::Texture& getShooterTexture() const { return m_shooterTex; }

private:
    TextureManager() {
        m_playerTex = createPlayerTexture();
        m_chaserTex = createChaserTexture();
        m_shooterTex = createShooterTexture();
    }

    sf::Texture m_playerTex;
    sf::Texture m_chaserTex;
    sf::Texture m_shooterTex;

    // Смайлик із великими очима для гравця
    sf::Texture createPlayerTexture() {
        const unsigned int S = 48;
        sf::Image img(sf::Vector2u{S, S}, sf::Color::Transparent);

        for (unsigned int y = 0; y < S; ++y) {
            for (unsigned int x = 0; x < S; ++x) {
                float dx = (float)x - S / 2.f;
                float dy = (float)y - S / 2.f;
                if (dx * dx + dy * dy <= (S / 2.f - 2) * (S / 2.f - 2)) {
                    img.setPixel({x, y}, sf::Color(0, 220, 255)); // Блакитна основа
                }
            }
        }
        // Великі здивовані очі
        drawCircle(img, 16, 18, 5, sf::Color::White);
        drawCircle(img, 32, 18, 5, sf::Color::White);
        drawCircle(img, 17, 18, 2, sf::Color::Black);
        drawCircle(img, 33, 18, 2, sf::Color::Black);
        // Відкритий рот "O"
        drawCircle(img, 24, 32, 4, sf::Color::Black);

        sf::Texture tex;
        (void)tex.loadFromImage(img);
        return tex;
    }

    // Злий червоний колобок із зубами (Chaser)
    sf::Texture createChaserTexture() {
        const unsigned int S = 48;
        sf::Image img(sf::Vector2u{S, S}, sf::Color::Transparent);

        for (unsigned int y = 0; y < S; ++y) {
            for (unsigned int x = 0; x < S; ++x) {
                float dx = (float)x - S / 2.f;
                float dy = (float)y - S / 2.f;
                if (dx * dx + dy * dy <= (S / 2.f - 2) * (S / 2.f - 2)) {
                    img.setPixel({x, y}, sf::Color(255, 60, 60)); // Червона основа
                }
            }
        }
        // Злі очі
        drawCircle(img, 15, 20, 4, sf::Color::White);
        drawCircle(img, 33, 20, 4, sf::Color::White);
        drawCircle(img, 17, 21, 2, sf::Color::Black);
        drawCircle(img, 31, 21, 2, sf::Color::Black);
        // Зубата посмішка
        for (unsigned int x = 16; x <= 32; ++x) {
            img.setPixel({x, 34}, sf::Color::Black);
            if (x % 3 == 0) img.setPixel({x, 33}, sf::Color::White);
        }

        sf::Texture tex;
        (void)tex.loadFromImage(img);
        return tex;
    }

    // Фіолетовий ворог в окулярах (Shooter)
    sf::Texture createShooterTexture() {
        const unsigned int S = 48;
        sf::Image img(sf::Vector2u{S, S}, sf::Color::Transparent);

        for (unsigned int y = 0; y < S; ++y) {
            for (unsigned int x = 0; x < S; ++x) {
                float dx = (float)x - S / 2.f;
                float dy = (float)y - S / 2.f;
                if (dx * dx + dy * dy <= (S / 2.f - 2) * (S / 2.f - 2)) {
                    img.setPixel({x, y}, sf::Color(200, 50, 230)); // Фіолетова основа
                }
            }
        }
        // Окуляри
        drawCircle(img, 16, 20, 6, sf::Color::Yellow);
        drawCircle(img, 32, 20, 6, sf::Color::Yellow);
        drawCircle(img, 16, 20, 3, sf::Color::Black);
        drawCircle(img, 32, 20, 3, sf::Color::Black);
        for (unsigned int x = 20; x <= 28; ++x) {
            img.setPixel({x, 20}, sf::Color::Yellow);
        }

        sf::Texture tex;
        (void)tex.loadFromImage(img);
        return tex;
    }

    void drawCircle(sf::Image& img, int cx, int cy, int r, sf::Color col) {
        for (int y = -r; y <= r; ++y) {
            for (int x = -r; x <= r; ++x) {
                if (x * x + y * y <= r * r) {
                    img.setPixel({static_cast<unsigned int>(cx + x), static_cast<unsigned int>(cy + y)}, col);
                }
            }
        }
    }
};