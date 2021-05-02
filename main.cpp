#include <SFML/Graphics.hpp>

#include <random>
#include <iostream>
#include <stdlib.h>

#include "PlatformCtrl.h"

// Gravity
static const float G = 0.3;
// Jump speed
static const float jump = 12;
static const float move_speed = 5;

struct Player {
    unsigned int height, width;
    float x, y;
    float dx, dy;

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
};

static bool handle_close_events(sf::RenderWindow& win);

int main(void)
{
    int vw, vh;
    sf::Sprite background;
    sf::Texture backgroundtex;

    sf::Sprite player;
    sf::Texture playertex;

    // background stuff
    backgroundtex.loadFromFile("assets/bck@2x.png");
    background.setTexture(backgroundtex);
    vw = backgroundtex.getSize().x;
    vh = backgroundtex.getSize().y;

    // platfor stuff
    static const int nplatforms = 12;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl platform_ctrl(platforms, nplatforms);
    platform_ctrl.set_bounds(vw, vh);
    platform_ctrl.randomize();

    // player stuff
    playertex.loadFromFile("assets/doodler.png");
    player.setTexture(playertex);
    sf::Vector2u pSize = playertex.getSize();
    Player p( pSize, vw / 2, vh-pSize.y );

    // Create the window
    sf::VideoMode video(vw, vh);
    sf::RenderWindow win(video, "Not Doodle Jump ;)", sf::Style::Close);
    win.setFramerateLimit(60);

    while (win.isOpen())
    {
        // Handle window events
        if (handle_close_events(win))
            return 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            p.dx = -move_speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            p.dx =  move_speed;

        p.dy += G;   // Gravity
        p.dx *= 0.85; // friction/wind-resistance... something like that
        p.x  += p.dx;
        p.y  += p.dy;

        // Bottom edge detection
        if (p.y > vh-p.height)
        {
            // TODO this is where the player looses
            p.y = vh-p.height;
            p.dy = 0;
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