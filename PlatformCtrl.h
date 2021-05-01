#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <random>
#include <ctime>

#include <unordered_set>

/**
 * Platfor controller for managing and generating platforms
 */
class PlatformCtrl {
    int n;
    sf::RectangleShape* platforms;

    struct { int w, h; } screen;

    struct {
        std::uniform_int_distribution<int> x;
        std::uniform_int_distribution<int> y;
    } rng;
    std::default_random_engine rng_engine;

    // set of y values
    std::unordered_set<int> set;

public:
    PlatformCtrl(sf::RectangleShape platforms[], int n)
      : platforms(platforms), n(n), rng_engine(time(0))
    {
        sf::Vector2f size(120, 20);
        set.reserve(n);
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
        rng = {
            std::uniform_int_distribution<int>(0, w-10),
            std::uniform_int_distribution<int>(100, h)
        };
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
        for (int i = 0; i < n; i++)
        {
            x = rng.x(rng_engine);
            y = rng.y(rng_engine);
            platforms[i].setPosition(x, y);
            set.insert(y);
        }
    }

    bool hit_platform(sf::FloatRect box)
    {
        sf::FloatRect bounds;
        for (int i = 0; i < n; i++)
        {
            bounds = platforms[i].getGlobalBounds();
            if (bounds.intersects(box))
                return true;
        }
        return false;
    }

    bool hit_platform(float x, float y)
    {
        sf::FloatRect bounds;
        for (int i = 0; i < n; i++)
        {
            bounds = platforms[i].getGlobalBounds();
            if (bounds.contains(x, y))
                return true;
        }
        return false;
    }

    void shift(int y)
    {
        // TODO shift all platforms up by y
        //      and move platforms to the top if
        //      they go through the floor.
    }
};
