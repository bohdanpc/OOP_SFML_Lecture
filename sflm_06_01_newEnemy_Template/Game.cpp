#include "Game.hpp"
#include "ChaserEnemy.hpp"
#include "ShooterEnemy.hpp"
#include "KamikazeEnemy.hpp"
#include <cstdlib>

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "05 - Polymorphism in Action"),
      m_player({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f})
{
    m_window.setFramerateLimit(60);
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        handleCollisions();
        render();
    }
}

void Game::processEvents() {
    while (const auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos{static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y)};
                m_bullets.emplace_back(m_player.getPosition(), mousePos, false);
            }
        }
    }
    m_player.handleInput();
}


void Game::updateEnemySpawn(float dt)
{
    m_enemySpawnTimer += dt;
    if (m_enemySpawnTimer >= 1.5f) {
        m_enemySpawnTimer = 0.f;
        sf::Vector2f spawnPos{ static_cast<float>(std::rand() % WINDOW_WIDTH), 10.f };

        int dice = std::rand() % 2;
        if (dice == 0) {
            m_enemies.push_back(std::make_unique<ChaserEnemy>(spawnPos));
        }
        else if (dice == 1) {
            m_enemies.push_back(std::make_unique<ShooterEnemy>(spawnPos));
        }
    }
}

void Game::update(float dt) {
   
    updateEnemySpawn(dt);

    m_player.update(dt, WINDOW_WIDTH, WINDOW_HEIGHT);

    for (auto& bullet : m_bullets) {
        bullet.update(dt);
    }

    // ПОЛІМОРФНИЙ ВИКЛИК
    for (auto& enemy : m_enemies) {
        enemy->update(dt, m_player.getPosition(), m_bullets);
    }

    std::erase_if(m_bullets, [](const Bullet& b) {
        sf::Vector2f p = b.getPosition();
        return p.x < 0 || p.x > WINDOW_WIDTH || p.y < 0 || p.y > WINDOW_HEIGHT;
    });

    std::erase_if(m_enemies, [](const std::unique_ptr<Enemy>& e) {
        return !e->isAlive();
    });
}

void Game::handleCollisions() {
    // 1. Кулі
    for (auto& bullet : m_bullets) {
        if (bullet.isEnemy()) {
            if (m_player.getBounds().findIntersection(bullet.getBounds())) {
                m_player.takeDamage(15);
                bullet.deactivate();

                if (!m_player.isAlive()) {
                    resetGame();
                    return;
                }
            }
        }
        else {
            for (auto& enemy : m_enemies) {
                if (enemy->isAlive() && enemy->getBounds().findIntersection(bullet.getBounds())) {
                    enemy->takeDamage(25);
                    bullet.deactivate();
                }
            }
        }
    }

    // 2. Зіткнення сутностей: ЧИСТИЙ ПОЛІМОРФІЗМ!
    // Game взагалі не знає, хто тут камікадзе, а хто ні.
    for (auto& enemy : m_enemies) {
        if (enemy->isAlive() && enemy->getBounds().findIntersection(m_player.getBounds())) {
            enemy->onPlayerCollision(m_player); // Кожен тип ворога реагує по-своєму!

            if (!m_player.isAlive()) {
                resetGame();
                return;
            }
        }
    }
}

void Game::resetGame() {
    m_player.reset({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    m_enemies.clear();
    m_bullets.clear();
    m_enemySpawnTimer = 0.f;
}

void Game::render() {
    m_window.clear(sf::Color(25, 25, 30));

    m_player.draw(m_window);
    for (const auto& bullet : m_bullets) bullet.draw(m_window);

    // ПОЛІМОРФНИЙ ВИКЛИК DRAW
    for (const auto& enemy : m_enemies) enemy->draw(m_window);

    m_window.display();
}