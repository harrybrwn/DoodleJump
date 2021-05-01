#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <random>
#include <ctime>

class PlatformCtrl {
    int n;
    sf::RectangleShape* platforms;

    struct {
        int w, h;
    } screen;

    struct {
        std::uniform_int_distribution<int> x;
        std::uniform_int_distribution<int> y;
    } rng;
    std::default_random_engine rng_engine;

public:
    PlatformCtrl(sf::RectangleShape platforms[], int n)
      : platforms(platforms), n(n), rng_engine(time(0))
    {
        sf::Vector2f size(120, 20);
        for (int i = 0; i < n; i++)
        {
            platforms[i].setSize(size);
            platforms[i].setPosition(0, 0);
            platforms[i].setFillColor(sf::Color::Black);
        }
    }

    void set_bounds(int w, int h)
    {
        screen.w = w;
        screen.h = h;
        rng = {
            std::uniform_int_distribution<int>(0, w),
            std::uniform_int_distribution<int>(0, h)
        };
    }

    void randomize(int w, int h)
    {
        set_bounds(w, h);
        randomize();
    }

    void randomize()
    {
        for (int i = 0; i < n; i++)
            platforms[i].setPosition(rng.x(rng_engine), rng.y(rng_engine));
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
        //      one goes out of bounds.
    }
};
