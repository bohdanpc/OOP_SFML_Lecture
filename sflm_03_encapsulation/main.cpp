#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;


void updateCollision(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, Player& player)
{
    for (Bullet& bullet : bullets) {
        if (bullet.isEnemy()) {
            if (player.getBounds().findIntersection(bullet.getBounds())) {
                player.takeDamage(15);
                bullet.deactivate();
            }
        }
        else {
            for (auto& enemy : enemies) {
                if (enemy.getBounds().findIntersection(bullet.getBounds())) {
                    enemy.takeDamage(25);
                    bullet.deactivate();
                }
            }
        }
    }

    if (!player.isAlive()) {
        player.reset({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
        enemies.clear();
        bullets.clear();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "03 - Encapsulation");
    window.setFramerateLimit(60);

    Player player({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    sf::Clock clock;
    float spawnTimer = 0.f;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // 1. Події
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos{static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y)};
                    bullets.emplace_back(player.getPosition(), mousePos, false);
                }
            }
        }

        // 2. Введення та спавн
        player.handleInput();


        spawnTimer += dt;
        if (spawnTimer >= 1.5f) {
            spawnTimer = 0.f;
            EnemyType type = (std::rand() % 2 == 0) ? EnemyType::Chaser : EnemyType::Shooter;
            enemies.emplace_back(sf::Vector2f{static_cast<float>(std::rand() % WINDOW_WIDTH), 10.f}, type);
        }

        // 3. Оновлення
        player.update(dt, WINDOW_WIDTH, WINDOW_HEIGHT);
        for (auto& bullet : bullets) bullet.update(dt);
        for (auto& enemy : enemies) enemy.update(dt, player.getPosition(), bullets);

        // 4. Колізії — виклики методів замість прямого доступу до полів
        updateCollision(bullets, enemies, player);

        // 4. Очищення
        std::erase_if(bullets, [](const Bullet& b) {
            sf::Vector2f p = b.getPosition();
            return p.x < 0 || p.x > WINDOW_WIDTH || p.y < 0 || p.y > WINDOW_HEIGHT;
        });
        std::erase_if(enemies, [](const Enemy& e) { return !e.isAlive(); });

        // 5. Рендер
        window.clear(sf::Color(25, 25, 30));
        player.draw(window);
        for (const Bullet& b : bullets) b.draw(window);
        for (const Enemy& e : enemies) e.draw(window);
        window.display();
    }

    return 0;
}