#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void handleCollisions();
    void render();
    void resetGame();

    sf::RenderWindow m_window;
    Player m_player;

    // ПОЛІМОРФНИЙ КОНТЕЙНЕР
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Bullet> m_bullets;

    float m_enemySpawnTimer = 0.f;

    static constexpr unsigned int WINDOW_WIDTH = 1200;
    static constexpr unsigned int WINDOW_HEIGHT = 800;
};