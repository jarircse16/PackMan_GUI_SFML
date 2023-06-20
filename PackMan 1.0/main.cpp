#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int DOT_RADIUS = 10;
const int GHOST_RADIUS = 15;
const int PACMAN_RADIUS = 20;

struct Dot {
    sf::CircleShape shape;

    Dot(float x, float y) {
        shape.setPosition(x, y);
        shape.setRadius(DOT_RADIUS);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin(DOT_RADIUS, DOT_RADIUS);
    }
};

class Pacman {
private:
    sf::CircleShape shape;
    sf::Vector2f direction;

public:
    Pacman(float x, float y) {
        shape.setPosition(x, y);
        shape.setRadius(PACMAN_RADIUS);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin(PACMAN_RADIUS, PACMAN_RADIUS);
        direction = sf::Vector2f(0.0f, 0.0f);
    }

    void setDirection(sf::Vector2f dir) {
        direction = dir;
    }

    void update(float deltaTime) {
        shape.move(direction * deltaTime);
    }

    const sf::CircleShape& getShape() const {
        return shape;
    }
};

class Ghost {
private:
    sf::CircleShape shape;
    sf::Vector2f direction;

public:
    Ghost(float x, float y) {
        shape.setPosition(x, y);
        shape.setRadius(GHOST_RADIUS);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(GHOST_RADIUS, GHOST_RADIUS);
        direction = sf::Vector2f(0.0f, 0.0f);
    }

    void setDirection(sf::Vector2f dir) {
        direction = dir;
    }

    void update(float deltaTime) {
        shape.move(direction * deltaTime);
    }

    const sf::CircleShape& getShape() const {
        return shape;
    }
};

bool checkCollision(const sf::CircleShape& shape1, const sf::CircleShape& shape2) {
    sf::Vector2f position1 = shape1.getPosition();
    sf::Vector2f position2 = shape2.getPosition();
    float radius1 = shape1.getRadius();
    float radius2 = shape2.getRadius();

    float dx = position1.x - position2.x;
    float dy = position1.y - position2.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < (radius1 + radius2);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman Game");

    std::vector<Dot> dots;
    std::vector<Ghost> ghosts;

    // Generate random dots
    for (int i = 0; i < 50; i++) {
        float x = static_cast<float>(std::rand() % (WINDOW_WIDTH - 2 * DOT_RADIUS) + DOT_RADIUS);
        float y = static_cast<float>(std::rand() % (WINDOW_HEIGHT - 2 * DOT_RADIUS) + DOT_RADIUS);
        dots.emplace_back(x, y);
    }

    // Generate random ghosts
    for (int i = 0; i < 3; i++) {
        float x = static_cast<float>(std::rand() % (WINDOW_WIDTH - 2 * GHOST_RADIUS) + GHOST_RADIUS);
        float y = static_cast<float>(std::rand() % (WINDOW_HEIGHT - 2 * GHOST_RADIUS) + GHOST_RADIUS);
        ghosts.emplace_back(x, y);
    }

    Pacman pacman(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    pacman.setDirection(sf::Vector2f(0.0f, -100.0f));
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    pacman.setDirection(sf::Vector2f(0.0f, 100.0f));
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    pacman.setDirection(sf::Vector2f(-100.0f, 0.0f));
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    pacman.setDirection(sf::Vector2f(100.0f, 0.0f));
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        pacman.update(deltaTime);

        // Check collision with dots
        for (auto it = dots.begin(); it != dots.end();) {
            if (checkCollision(pacman.getShape(), it->shape)) {
                it = dots.erase(it);
            }
            else {
                ++it;
            }
        }

        // Update ghost positions
        for (auto& ghost : ghosts) {
            sf::Vector2f dir(static_cast<float>(std::rand() % 3 - 1), static_cast<float>(std::rand() % 3 - 1));
            ghost.setDirection(dir);
            ghost.update(deltaTime);
        }

        // Check collision with ghosts
        for (const auto& ghost : ghosts) {
            if (checkCollision(pacman.getShape(), ghost.getShape())) {
                std::cout << "Game Over!" << std::endl;
                window.close();
            }
        }

        window.clear();

        // Draw dots
        for (const auto& dot : dots) {
            window.draw(dot.shape);
        }

        // Draw ghosts
        for (const auto& ghost : ghosts) {
            window.draw(ghost.getShape());
        }

        // Draw pacman
        window.draw(pacman.getShape());

        window.display();
    }

    return 0;
}
