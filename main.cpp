#include <SFML/Graphics.hpp>

#include <random>
#include <iostream>
#include <stdlib.h>

#include "PlatformCtrl.h"

// Static assets
#include "doodle_png.i"
#include "background.i"

// Gravity
static const float G = 0.3;

struct Player {
    unsigned int height, width;
    float x, y;
    float dx, dy;
    int8_t direction;

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
        direction = 1;
    }
};

static bool handle_close_events(sf::RenderWindow& win);

int main(void)
{
    sf::VideoMode video(500, 800);
    sf::RenderWindow win(video, "Not Doodle Jump ;)", sf::Style::Close);
    win.setFramerateLimit(60);

    sf::Sprite background;
    sf::Texture backgroundtex;

    sf::Sprite player;
    sf::Texture playertex;

    // platfor stuff
    static const int nplatforms = 10;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl ctrl(platforms, nplatforms);
    ctrl.set_bounds(500, 800);
    ctrl.randomize();

    // player stuff
    playertex.loadFromMemory(doodle_png, doodle_png_len);
    player.setTexture(playertex);
    sf::Vector2u pSize = playertex.getSize();
    Player p( pSize, 250, 800-pSize.y );
    player.setPosition(p.x, p.y);
    player.scale(-1, 1);              // flip
    player.setOrigin(p.width / 2, 0); // translations happen around middle

    // background stuff
    backgroundtex.loadFromMemory(
        background_png,
        background_png_len
    );
    backgroundtex.setRepeated(true);
    background.setTexture(backgroundtex);

    while (win.isOpen())
    {
        // Handle window events
        if (handle_close_events(win))
            return 0;
        static const float jump = 10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            // flip player image
            if (p.direction == 1)
            {
                player.scale(-1.0, 1.0);
                p.direction = -1;
            }
            p.dx = -5;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // flip player image
            if (p.direction == -1)
            {
                player.scale(-1.0, 1.0);
                p.direction = 1;
            }
            p.dx = 5;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (p.dy <= 0) p.dy = -jump; // just here for debugging
        }

        p.dy += G;   // Gravity
        p.dx *= 0.8; // friction/wind resistance... something like that
        p.x += p.dx;
        p.y += p.dy;

        // Jump we hit a platform
        sf::FloatRect hitbox(p.x, p.y, p.width, p.height);
        if (p.dy > 0 && ctrl.hit_platform(hitbox))
        {
            // TODO prevent jump from triggering when head hits platform
            p.dy = -jump;
        }

        // Bottom edge detection
        if (p.y > 800-p.height)
        {
            p.y = 800-p.height;
            p.dy = 0;
        }

        // Left/Right edge detection
        if (p.x > 500)
        {
            // Teleport to right side
            p.x = 0;
        }
        else if (p.x < 0)
        {
            // Teleport to left side
            p.x = 500;
        }

        player.setPosition(p.x, p.y);
        win.draw(background);
        for (int i = 0; i < nplatforms; i++)
            win.draw(platforms[i]);
        win.draw(player);
        win.display();
    }
    return 0;
}

static bool handle_close_events(sf::RenderWindow& win)
{
    sf::Event event;
    while (win.pollEvent(event))
        switch (event.type)
        {
        case sf::Event::Closed:
            // Handle the "x" button
            win.close();
            return true;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Q:
            case sf::Keyboard::Key::Escape:
                // Close on "Esc"
                win.close();
                return true;
            }
            break;
        }
    return false;
}