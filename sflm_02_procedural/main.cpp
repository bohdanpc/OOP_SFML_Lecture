#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;

struct PlayerData {
    sf::CircleShape shape;
    float speed = 300.f;
    float health = 100.0f;
    float hitFlashTimer = 0.f;
};

struct BulletData {
    sf::CircleShape shape;
    sf::Vector2f velocity;  //direction + speed combined
    bool isEnemy = false;
};

enum class EnemyType {
    Chaser,
    Shooter
};

struct EnemyData {
    sf::CircleShape shape;
    float speed;
    int health;
    EnemyType type;
    float shootCooldown = 0.f;
};

sf::Vector2f normalize(const sf::Vector2f& source) {
    float length = std::sqrt(source.x * source.x + source.y * source.y);
    if (length != 0.f)
        return source / length;
    return source;
}

void updatePlayer(PlayerData& player, float dt) {
    // Візуальний фідбек шкоди
    if (player.hitFlashTimer > 0.f) {
        player.hitFlashTimer -= dt;
        player.shape.setFillColor(sf::Color::Red);
    }
    else {
        // Колір плавно змінюється: повне HP - Cyan, критичне HP - темніє до багряного
        float hpRatio = static_cast<float>(player.health) / 100.f;
        if (hpRatio < 0.f) hpRatio = 0.f;
        player.shape.setFillColor(sf::Color(
            static_cast<std::uint8_t>(255 * (1.f - hpRatio * 0.7f)),
            static_cast<std::uint8_t>(255 * hpRatio),
            static_cast<std::uint8_t>(255 * hpRatio)
        ));
    }

    sf::Vector2f movement{ 0.f, 0.f };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1.f;

    movement = normalize(movement);
    player.shape.move(movement * player.speed * dt);

    sf::Vector2f pos = player.shape.getPosition();
    if (pos.x < 20.f) pos.x = 20.f;
    if (pos.x > WINDOW_WIDTH - 20.f) pos.x = WINDOW_WIDTH - 20.f;
    if (pos.y < 20.f) pos.y = 20.f;
    if (pos.y > WINDOW_HEIGHT - 20.f) pos.y = WINDOW_HEIGHT - 20.f;
    player.shape.setPosition(pos);
}

void spawnBullet(std::vector<BulletData>& bullets, sf::Vector2f startPos, sf::Vector2f targetPos, bool isEnemy) {
    BulletData bullet;
    bullet.shape.setRadius(isEnemy ? 5.f : 4.f);
    bullet.shape.setFillColor(isEnemy ? sf::Color::Yellow : sf::Color::White);
    bullet.shape.setOrigin({ bullet.shape.getRadius(), bullet.shape.getRadius() });
    bullet.shape.setPosition(startPos);
    bullet.isEnemy = isEnemy;

    sf::Vector2f dir = normalize(targetPos - startPos);
    float speed = isEnemy ? 250.f : 500.f;
    bullet.velocity = dir * speed;

    bullets.push_back(bullet);
}

void updateEnemies(std::vector<EnemyData>& enemies, std::vector<BulletData>& bullets, const sf::Vector2f& playerPos, float dt) {
    for (EnemyData& enemy : enemies) {
        switch (enemy.type) {
        case EnemyType::Chaser: {
            sf::Vector2f dir = normalize(playerPos - enemy.shape.getPosition());
            enemy.shape.move(dir * enemy.speed * dt);
            break;
        }
        case EnemyType::Shooter: {
            sf::Vector2f toPlayer = playerPos - enemy.shape.getPosition();
            float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

            if (dist > 250.f) {
                enemy.shape.move(normalize(toPlayer) * enemy.speed * dt);
            }

            enemy.shootCooldown -= dt;
            if (enemy.shootCooldown <= 0.f) {
                spawnBullet(bullets, enemy.shape.getPosition(), playerPos, true);
                enemy.shootCooldown = 2.0f;
            }
            break;
        }
        }
    }
}

void updateBullets(std::vector<BulletData>& bullets, float dt) {
    for (auto& bullet : bullets) {
        bullet.shape.move(bullet.velocity * dt);
    }

    std::erase_if(bullets, [](const BulletData& b) {
        sf::Vector2f p = b.shape.getPosition();
        return p.x < 0 || p.x > WINDOW_WIDTH || p.y < 0 || p.y > WINDOW_HEIGHT;
    });
}

void updateCollisions(std::vector<BulletData>& bullets, std::vector<EnemyData>& enemies, PlayerData& player)
{
    for (BulletData& bullet : bullets) {
        if (bullet.isEnemy) {
            //Оновлює внутрішні стани гравця та куль
            if (player.shape.getGlobalBounds()
                .findIntersection(bullet.shape.getGlobalBounds())) {
                player.health -= 20;
                player.hitFlashTimer = 0.15f;
                bullet.shape.setPosition({ -100.f, -100.f });
            }
        }
        else {
            for (EnemyData& enemy : enemies) {
                if (enemy.shape.getGlobalBounds().findIntersection(bullet.shape.getGlobalBounds())) {
                    enemy.health -= 25;
                    bullet.shape.setPosition({ -100.f, -100.f });
                }
            }
        }
    }

    for (EnemyData& enemy : enemies) {
        if (enemy.shape.getGlobalBounds().findIntersection(player.shape.getGlobalBounds())) {
            player.health -= 0.3;
        }
    }

    // Якщо здоров'я закінчилось — рестарт гри
              // Чомусь скидає стан гри???
    if (player.health <= 0) {
        player.health = 100;
        player.shape.setPosition(
            { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
        enemies.clear();
        bullets.clear();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "02 - Procedural Pain");
    window.setFramerateLimit(60);

    PlayerData player;
    player.shape.setRadius(20.f);
    player.shape.setFillColor(sf::Color::Cyan);
    player.shape.setOrigin({ 20.f, 20.f });
    player.shape.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });

    std::vector<BulletData> bullets;
    std::vector<EnemyData> enemies;

    sf::Clock clock;
    float enemySpawnTimer = 0.f;

    //GAME LOOP
    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds();

        // 1. Події
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos{ static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y) };
                    spawnBullet(bullets, player.shape.getPosition(), mousePos, false);
                }
            }
        }

        // 2. Спавн ворогів
        enemySpawnTimer += dt;
        if (enemySpawnTimer >= 2.0f) {
            enemySpawnTimer = 0.f;
            EnemyData enemy;

            if (std::rand() % 2 == 0) {
                enemy.type = EnemyType::Chaser;
                enemy.shape.setRadius(18.f);
                enemy.shape.setFillColor(sf::Color::Red);
                enemy.speed = 120.f;
                enemy.health = 30;
            }
            else {
                enemy.type = EnemyType::Shooter;
                enemy.shape.setRadius(22.f);
                enemy.shape.setFillColor(sf::Color::Magenta);
                enemy.speed = 70.f;
                enemy.health = 50;
                enemy.shootCooldown = 1.0f;
            }

            enemy.shape.setOrigin({ enemy.shape.getRadius(), enemy.shape.getRadius() });
            enemy.shape.setPosition({ static_cast<float>(std::rand() % WINDOW_WIDTH), 10.f });
            enemies.push_back(enemy);
        }

        // 3. Updates
        updatePlayer(player, dt);
        updateEnemies(enemies, bullets, player.shape.getPosition(), dt);
        updateBullets(bullets, dt);

        // 4. Collisions
        updateCollisions(bullets, enemies, player);

        // 5. Clean up dead enemies and used bullets
        std::erase_if(enemies, [](const EnemyData& e) { return e.health <= 0; });
        std::erase_if(bullets, [](const BulletData& b) { return b.shape.getPosition().x < 0; });

        // 6. Rendering
        window.clear(sf::Color(25, 25, 30));

        window.draw(player.shape);
        for (const auto& b : bullets) window.draw(b.shape);
        for (const auto& e : enemies) window.draw(e.shape);

        window.display();
    }

    return 0;
}