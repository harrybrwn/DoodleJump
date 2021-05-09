#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include <string>

class Player {
    sf::Texture texture;
    bool jumped = false;
    sf::FloatRect hitbox;

public:
    unsigned int height, width;
    float x, y;
    float dx, dy;
    sf::Sprite sprite;

    Player(sf::Vector2u size)
     : Player(size, 0, 0)
    {
    }

    Player(sf::Vector2u size, int x, int y)
      : x(x), y(y)
    {
        height = size.y;
        width = size.x;
        dx = 0;
        dy = 0;
    }

    Player(std::string textureFile)
      : x(0), y(0)
    {
        sf::Vector2u size;
        texture.loadFromFile(textureFile);
        size = texture.getSize();
        sprite.setTexture(texture);

        width = size.x;
        height = size.y;
        sprite.setOrigin(width / 2, 0); // set origin at center of player feet
    }

    void update()
    {
        hitbox.left = x - (width/2);
        hitbox.top = y + height;
        hitbox.width = width;
        hitbox.height = 1;
        sprite.setPosition(x, y);
    }

    void jump(float speed)
    {
        // dont jump twice in a row without ever
        // touching the ground
        if (dy == 0 && !jumped)
        {
            dy = speed;
            jumped = true;
        }
        else
        {
            jumped = false;
        }
    }

    /**
     * jump_hitbox returns a hitbox at the player's feet
     * to detect the player landing on the platform.
     */
    sf::FloatRect jump_hitbox() const
    {
        return hitbox;
    }

    void look_left()  { sprite.setScale(-1, 1); }
    void look_right() { sprite.setScale(1, 1); }
};