#include "Game.hpp"
#include <cstdlib>

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "04 - Composition & Game Class"),
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

void Game::update(float dt) {
    // Спавн ворогів
    updateEnemySpawn(dt);

    // Оновлення
    m_player.update(dt, WINDOW_WIDTH, WINDOW_HEIGHT);

    for (auto& bullet : m_bullets) {
        bullet.update(dt);
    }

    for (auto& enemy : m_enemies) {
        enemy.update(dt, m_player.getPosition(), m_bullets);
    }

    // Очищення неактивних сутностей
    std::erase_if(m_bullets, [](const Bullet& b) {
        sf::Vector2f p = b.getPosition();
        return p.x < 0 || p.x > WINDOW_WIDTH || p.y < 0 || p.y > WINDOW_HEIGHT;
    });

    std::erase_if(m_enemies, [](const Enemy& e) {
        return !e.isAlive();
    });
}

void Game::handleCollisions() {
    for (Bullet& bullet : m_bullets) 
    {
        if (bullet.isEnemy()) 
        {
            if (m_player.getBounds().findIntersection(bullet.getBounds())) 
            {
                m_player.takeDamage(15);
                bullet.deactivate();
            }
        } 
        else 
        {
            for (Enemy& enemy : m_enemies) {
                if (enemy.getBounds().findIntersection(bullet.getBounds())) 
                {
                    enemy.takeDamage(25);
                    bullet.deactivate();
                }
            }
        }
    }

    for (Enemy& enemy : m_enemies) {
        if (enemy.getBounds().findIntersection(m_player.getBounds()))
        {
            m_player.takeDamage(0.5f);
        }
    }

    if (!m_player.isAlive()) 
    {
        resetGame();
    }

}

void Game::resetGame() {
    m_player.reset({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    m_enemies.clear();
    m_bullets.clear();
    m_enemySpawnTimer = 0.f;
}

void Game::updateEnemySpawn(float dt)
{
    m_enemySpawnTimer += dt;
    if (m_enemySpawnTimer >= EnemySpawnFrequencySec) {
        m_enemySpawnTimer = 0.f;
        EnemyType type = (std::rand() % 2 == 0) ? EnemyType::Chaser : EnemyType::Shooter;
        m_enemies.emplace_back(sf::Vector2f{ static_cast<float>(std::rand() % WINDOW_WIDTH), 10.f }, type);
    }
}

void Game::render() {
    m_window.clear(sf::Color(25, 25, 30));

    m_player.draw(m_window);
    for (const Bullet& bullet : m_bullets) bullet.draw(m_window);
    for (const Enemy& enemy : m_enemies) enemy.draw(m_window);

    m_window.display();
}