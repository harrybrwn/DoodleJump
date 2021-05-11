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
    unsigned int distTraveled;
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
        distTraveled = 0;
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

    void reset()
    {
        dx = 0;
        dy = 0;
        distTraveled = 0;
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
        // TODO I can't remember why this is like this
        //      an hour after I wrote it and the
        //      comments don't help, it should go away
        //      bc it's hideous. Although, not jumping
        //      when the player is in the air is useful.

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

    void width_wrap(int view_width)
    {
        if (x > view_width + (float)width)
            x = -1.0f * width;
        else if (x < (-1.0f * (float)width))
            x = view_width + (float)width;
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