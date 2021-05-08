#include <SFML/Graphics.hpp>

#include <random>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "PlatformCtrl.h"
#include "Player.h"

using namespace std;

// Gravity
static float G = 0.3;
// Jump speed
static const float jump = 12;
static const float move_speed = 5;

struct _Player {
    unsigned int height, width;
    float x, y;
    float dx, dy;

    _Player(sf::Vector2u size)
     : _Player(size, 0, 0)
    {
    }

    _Player(sf::Vector2u size, int x, int y)
      : x(x), y(y)
    {
        height = size.y;
        width = size.x;
        dx = 0;
        dy = 0;
    }


    inline sf::FloatRect jump_hitbox()
    {
        sf::FloatRect hitbox(
            x - ( width / 2), y + height, // positioned at player's feet
            width, 1
        );
        return hitbox;
    }
};

static bool handle_close_events(sf::RenderWindow& win);
static void debug_hitbox(sf::FloatRect hitbox, sf::RenderWindow& win);

int main(void)
{
    int vw, vh;
    sf::Sprite background;
    sf::Texture backgroundtex;

    // sf::Sprite player;
    // sf::Texture playertex;

    // background stuff
    backgroundtex.loadFromFile("assets/bck@2x.png");
    background.setTexture(backgroundtex);
    vw = backgroundtex.getSize().x;
    vh = backgroundtex.getSize().y;

    // platform stuff
    static const int nplatforms = 8;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl platform_ctrl(platforms, nplatforms);
    platform_ctrl.set_bounds(vw, vh);
    platform_ctrl.randomize();

    // player stuff
    // playertex.loadFromFile("assets/PixelThing.png");
    // player.setTexture(playertex);
    // sf::Vector2u pSize = playertex.getSize();
    // _Player p( pSize, vw / 2, vh-pSize.y );
    // player.setOrigin(p.width/2, 0);
    Player p("assets/PixelThing.png");
    p.x = vw / 2;
    p.y = vh-100;
    // player = p.sprite;

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
        {
            // player.setScale(sf::Vector2f(-1, 1));
            p.look_left();
            p.dx = -move_speed; // Moves Left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // player.setScale(sf::Vector2f(1, 1));
            p.look_right();
            p.dx =  move_speed; // Moves Right
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            p.jump(-jump);

        // if hitting a platform while falling
        if (p.dy >= 0 && platform_ctrl.hit_platform(p.jump_hitbox()))
            p.dy = 0;
        else
            p.dy += G;

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

        if (p.x > vw + (float)p.width)
        {
            p.x = -1*(float)p.width;
        }
        else if (p.x < (-1*(float)p.width))
        {
            p.x = vw + (float)p.width;
        }

        win.draw(background);
        for (int i = 0; i < nplatforms; i++)
            win.draw(platforms[i]);
        // win.draw(player);
        p.draw_with(win);
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

static void debug_hitbox(sf::FloatRect hitbox, sf::RenderWindow& win)
{
    sf::RectangleShape box;
    sf::Color color = sf::Color::Red;
    box.setSize(sf::Vector2f(hitbox.width, hitbox.height));
    box.setPosition(hitbox.left, hitbox.top);
    box.setFillColor(color);
    box.setOutlineColor(color);
    box.setOutlineThickness(1);
    win.draw(box);
}