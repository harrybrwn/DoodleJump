#include <SFML/Graphics.hpp>

#include <random>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "PlatformCtrl.h"
#include "Player.h"

#define DEBUG

using namespace std;

// Gravity
static float G = 0.3;
// Jump speed
static const float jump = 10;
static const float move_speed = 5;

static bool handle_close_events(sf::RenderWindow& win);
static void debug_hitbox(sf::FloatRect hitbox, sf::RenderWindow& win);
static bool scrolledUp = false;

int main(void)
{
    int vw, vh;
    sf::Sprite background;
    sf::Texture backgroundtex;

    // background stuff
    backgroundtex.loadFromFile("assets/bck@2x.png");
    background.setTexture(backgroundtex);
    vw = backgroundtex.getSize().x;
    vh = backgroundtex.getSize().y;

    sf::Font font;
    font.loadFromFile("assets/arial.ttf");
    sf::Text debugtext;
    debugtext.setFont(font);
    debugtext.setCharacterSize(18);
    debugtext.setFillColor(sf::Color::Black);

    Player p("assets/PixelThing.png");
    p.x = vw / 2;
    p.y = vh-100;

    // platform stuff
    static const int nplatforms = 13;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl platform_ctrl(platforms, nplatforms);
    platform_ctrl.set_bounds(vw, vh);
    platform_ctrl.randomize();

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
            p.sprite.setScale(-1, 1);
            p.dx = -move_speed; // Moves Left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            p.look_right();
            p.dx =  move_speed; // Moves Right
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            p.jump(-jump);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            p.y += 1; // for debugging

        // if hitting a platform while falling
        if (p.dy >= 0 && platform_ctrl.hit_platform(p.jump_hitbox()))
        {
            cout << "platform hit: " << p.dx << ", " << p.dy << std::endl;
            p.dy = -jump;
        }
        else
        {
            p.dy += G;
        }

        if (p.y < 200 && p.dy < 0)
        {
            platform_ctrl.shift_up(-p.dy);
            p.y = 200;
        }

        // Bottom edge detection
        if (p.y > vh-p.height)
        {
            // TODO this is where the player looses
            p.y = vh-p.height;
            p.dy = 0;
        }

        p.dx *= 0.85; // friction/wind-resistance... something like that
        p.x  += p.dx;
        p.y  += p.dy;

    #ifdef DEBUG
        // For debugging only: place player with mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i pos = sf::Mouse::getPosition(win);
            p.x = pos.x;
            p.y = pos.y;
            p.dy = 0;
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            p.dy = -40; // jump really high
        if (scrolledUp) platform_ctrl.shift_up(20);
    #endif

        p.width_wrap(vw);
        p.update();

        char buf[500];
        snprintf(buf, sizeof(buf),
            "velocity: <%f, %f>\nposition: <%f, %f>\n",
            p.dx, p.dy, p.x, p.y);
        debugtext.setString(buf);

        win.draw(background);
        for (int i = 0; i < nplatforms; i++)
            win.draw(platforms[i]);
        win.draw(p.sprite);
        win.draw(debugtext);
        win.display();
    }
    return 0;
}

static bool handle_close_events(sf::RenderWindow& win)
{
    sf::Event event;
    scrolledUp = false;
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
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheel.x > 0)
                scrolledUp = true;
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