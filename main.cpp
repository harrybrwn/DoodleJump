#include <SFML/Graphics.hpp>

#include <random>
#include <iostream>
#include <stdlib.h>

#include "PlatformCtrl.h"

// Static assets
#include "doodle_png.i"
#include "background.i"

// Gravity
static const float G = 0.4;

struct Player {
    unsigned int height, width;
    float x, y;
    float dx, dy;

    Player(sf::Vector2u size)
    {
        height = size.y;
        width = size.x;
        dx = 0;
        dy = 0;
    }
    Player(sf::Vector2u size, int x, int y)
      : height(size.y), width(size.x), x(x), y(y)
    {
    }
};

int main(void)
{
    sf::VideoMode video(500, 800);
    sf::RenderWindow win(video, "Doodle Jump", sf::Style::Close);

    sf::Texture backgroundtex;
    sf::Texture playertex;
    sf::Sprite background;
    sf::Sprite player;

    static const int nplatforms = 10;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl ctrl(platforms, nplatforms);
    ctrl.set_bounds(500, 800);
    ctrl.randomize();

    playertex.loadFromMemory(doodle_png, doodle_png_len);
    player.setTexture(playertex);
    sf::Vector2u pSize = playertex.getSize();
    Player p( pSize, 250, 800-pSize.y );
    player.setPosition(p.x, p.y);
    backgroundtex.loadFromMemory(
        background_png,
        background_png_len
    );
    backgroundtex.setRepeated(true);
    background.setTexture(backgroundtex);
    win.setFramerateLimit(60);

    sf::Event event;
    while (win.isOpen())
    {
        // Handle window events
        while (win.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                // Handle the "x" button
                win.close();
                return 0;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    // Close on "Esc"
                    win.close();
                    return 0;
                }
                break;
            }
        }

        static const float jump = 10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            p.x -= 5;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            p.x += 5;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            p.dy = -jump;
        }

        p.dy += G; // Gravity

        p.x += p.dx;
        p.y += p.dy;

        // Bottom edge detection
        if (p.y > 800-p.height)
            p.y = 800-p.height;

        // Left/Right edge detection
        if (p.x > 500)
            p.x = 500;
        else if (p.x < 0)
            p.x = 0;

        player.setPosition(p.x, p.y);
        win.draw(background);
        for (int i = 0; i < nplatforms; i++)
            win.draw(platforms[i]);
        win.draw(player);
        win.display();
    }
    return 0;
}