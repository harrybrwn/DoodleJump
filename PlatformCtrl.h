#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <random>
#include <ctime>

#include "util.hpp"

/**
 * Platfor controller for managing and generating platforms
 */
class PlatformCtrl {

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
    static const int PlatformWidth = 100;
    static const int PlatformHeight = 15;

    PlatformCtrl(sf::RectangleShape platforms[], int n)
      : platforms(platforms), n(n), rng_engine(time(0))
    {
        sf::Vector2f size(PlatformWidth, PlatformHeight);
        for (int i = 0; i < n; i++)
        {
            platforms[i].setSize(size);
            platforms[i].setPosition(0, 0);

            platforms[i].setFillColor(sf::Color::Black);
        }
    }

    int count() { return n; }

    sf::RectangleShape* get_platforms() { return platforms; }

    void set_bounds(int w, int h)
    {
        // TODO Make this function an option as a contructor
        screen.w = w;
        screen.h = h;
        rng.x = std::uniform_int_distribution<int>(PlatformWidth/2, w-PlatformWidth);
        // rng.x = std::uniform_int_distribution<int>((w/2), (w/2)+1);
        rng.y = std::uniform_int_distribution<int>(0, (w / n) - PlatformHeight);
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
            // making sure players dont get stuck mid-platform
            //bounds.height = (int)bounds.height / 2;
            if (box.intersects(bounds))
                return true;
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

    void shift_up(int y)
    {
        sf::Vector2f pos;
        for (int i = 0; i < n; i++)
        {
            pos = platforms[i].getPosition();
            if (pos.y > screen.h)
            {
                // generate a new position for the platform
                // make sure its off-screen
                int x, y;
                x = rng.x(rng_engine);
                y = rng.y(rng_engine) - 45;
                platforms[i].setPosition(x, y);
            }
            else
            {
                pos.y += y;
                platforms[i].setPosition(pos);
            }
        }
    }

    sf::RectangleShape* closest_platform(const sf::Vector2f& v)
    {
        sf::Vector2f pos;
        int ix = 0;
        float min;
        float dist;

        min = vec_distance(v, platforms[0].getPosition());
        for (int i = 1; i < n; i++)
        {
            dist = vec_distance(v, platforms[i].getPosition());
            if (dist < min)
            {
                min = dist;
                ix = i;
            }
        }
        return &platforms[ix];
    }

    sf::RectangleShape* closest_platform(float x, float y)
    {
        return closest_platform(sf::Vector2f(x, y));
    }

    void draw_with(sf::RenderTarget& target)
    {
        for (int i = 0; i < n; i++)
        {
            target.draw(platforms[i]);
        }
    }
};
