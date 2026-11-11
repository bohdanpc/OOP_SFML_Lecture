#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
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
    void updateEnemySpawn(float dt);

    // КОМПОЗИЦІЯ (HAS-A): Game володіє цими компонентами
    sf::RenderWindow m_window;
    Player m_player;
    std::vector<Enemy> m_enemies;
    std::vector<Bullet> m_bullets;

    float m_enemySpawnTimer = 0.f;

    static constexpr unsigned int WINDOW_WIDTH = 1200;
    static constexpr unsigned int WINDOW_HEIGHT = 800;

    static constexpr float EnemySpawnFrequencySec = 1.5;
};