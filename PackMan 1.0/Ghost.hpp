#ifndef GHOST_HPP
#define GHOST_HPP

#include <SFML/Graphics.hpp>

class Ghost {
public:
    // Declare member functions here
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    // Declare member variables here
    sf::CircleShape shape;
};

#endif // GHOST_HPP
