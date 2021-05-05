#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <random>
#include <ctime>

/**
 * Platfor controller for managing and generating platforms
 */
class PlatformCtrl {
    static const int PlatformWidth = 120;

    int n;
    struct { int w, h; } screen;
    sf::RectangleShape* platforms;
    bool hit_check = false;
    std::default_random_engine rng_engine;
    struct {
        std::uniform_int_distribution<int> x;
        std::uniform_int_distribution<int> y;
    } rng;

public:
    PlatformCtrl(sf::RectangleShape platforms[], int n)
      : platforms(platforms), n(n), rng_engine(time(0))
    {
        sf::Vector2f size(PlatformWidth, 20);
        for (int i = 0; i < n; i++)
        {
            platforms[i].setSize(size);
            platforms[i].setPosition(0, 0);
            platforms[i].setFillColor(sf::Color::Black);
        }
    }

    void set_bounds(int w, int h)
    {
        // TODO Make this function an option as a contructor
        screen.w = w;
        screen.h = h;
        rng.x = std::uniform_int_distribution<int>(PlatformWidth/2, w-PlatformWidth);
        rng.y = std::uniform_int_distribution<int>(0, w / n);
    }

    void randomize(int w, int h)
    {
        // TODO This overload is dumb and i hate it, probably get rid of it.
        set_bounds(w, h);
        randomize();
    }

    void randomize()
    {
        int x, y;
        // Divide the screen vertically into "n" regions
        // and generate a random height inside each region
        // so the platform heigh feels random but really isn't
        // that random because players still need to be able
        // to get to the top.
        const int section_width = screen.h / n;

        for (int i = 0; i < n; i++)
        {
            x = rng.x(rng_engine);
            y = rng.y(rng_engine);
            platforms[i].setPosition(x, y+(i*section_width));
        }
    }

    bool hit_platform(sf::FloatRect box)
    {
        sf::FloatRect bounds;
        for (int i = 0; i < n; i++)
        {
            bounds = platforms[i].getGlobalBounds();
            if (bounds.intersects(box))
            {
                hit_check = true;
                return true;
            }
        }
        return false;
    }

    bool hit_platform(float x, float y)
    {
        //sf::FloatRect bounds;
        for (int i = 0; i < n; i++)
        {
            sf::Vector2f bounds = platforms[i].getPosition();
            if(y < bounds.y && y > bounds.y - 7)
            {
                if (x > bounds.x && x < bounds.x + PlatformWidth)
                {
                    hit_check = true;
                    return true;
                }
            }
        }
        return false;
    }

    bool landed_on()
    {
        if(hit_check)
        {
            return true;
        }

        return false;

    }

    int width_getter()
    {
        return PlatformWidth;
    }

    void shift_up(int y);
};

void PlatformCtrl::shift_up(int y)
{
    sf::Vector2f pos;
    for (int i = 0; i < n; i++)
    {
        pos = platforms[i].getPosition();
        if (pos.y > screen.h)
        {
            // generate a new position for the platform
            // make sure its off-screen
            platforms[i].setPosition(rng.x(rng_engine), -50);
        }
        else
        {
            pos.y += y;
            platforms[i].setPosition(pos);
        }
    }
}
