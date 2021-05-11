#include <SFML/Graphics.hpp>

#include <random>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <random>

#include "Game.hpp"
#include "PlatformCtrl.h"
#include "Player.h"
#include "util.hpp"

#define DEBUG

using namespace std;

static bool should_close(sf::RenderWindow& win);
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

    // Game Window
    sf::VideoMode video(vw, vh);
    sf::RenderWindow win(video, "Not Doodle Jump ;)", sf::Style::Close);
    // platform stuff
    static const int nplatforms = 16;
    // static const int nplatforms = 8;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl platform_ctrl(platforms, nplatforms);
    Player p("assets/PixelThing.png");

    // Create the game controller
    Game game(win, p, platform_ctrl);
    game.score.setPosition(vw-175, 50);
    game.set_background(background);

    sf::Text debugtext("", game.font, 18);
    debugtext.setFillColor(sf::Color::Black);

    win.draw(background);
    if (game.show_opening_screen() == Game::Done)
        return 0;

Gameplay:
    p.reset();
    p.x = vw / 2;
    p.y = vh - (vh/4);

    platform_ctrl.set_bounds(vw, vh);
    platform_ctrl.randomize();
    // Find the platform closest to the player and move it directly under
    // platform_ctrl.shift_up(-p.height);
    sf::RectangleShape* closest_plt = platform_ctrl.closest_platform(p.x, p.y);
    closest_plt->setPosition(
        p.x-(PlatformCtrl::PlatformWidth/2),
        p.y + (p.height*1.25)
    );
    win.clear();

    while (win.isOpen())
    {
        // Handle window events
        if (should_close(win))
            return 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            p.look_left();
            p.dx = -Game::MoveSpeed; // Moves Left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            p.look_right();
            p.dx = Game::MoveSpeed; // Moves Right
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            if (game.pause() != Game::Start)
                return 0;
            continue;
        }

        // if hitting a platform while falling
        if (p.dy >= 0 && platform_ctrl.hit_platform(p.jump_hitbox()))
            p.dy = -Game::Jump;
        else
            p.dy += Game::Gravity;

        // Shift all platforms up
        if (p.y < Game::MaxPlayerHeight && p.dy < 0)
        {
            platform_ctrl.shift_up(-p.dy);
            p.y = Game::MaxPlayerHeight;
            p.distTraveled -= (int)p.dy; // update score
        }

        // Bottom edge detection
        if (p.y > vh-p.height)
        {
            game.setup_game_over(); // shifts all the platforms up
            break; // break out of main game loop
        }

        p.dx *= Game::Friction; // friction/wind-resistance... something like that
        p.x  += p.dx;
        p.y  += p.dy;

        // Update score
        if (p.distTraveled <= (p.y - vh+p.height) * -1)
        {
            p.distTraveled = (p.y-vh+p.height) * -1;
        }
        game.set_score(p.distTraveled);

    #ifdef DEBUG
        game.apply_debug_controlls();
        if (scrolledUp) platform_ctrl.shift_up(20);
        char buf[500];
        snprintf(buf, sizeof(buf),
            "velocity: <%f, %f>\nposition: <%f, %f>\n",
            p.dx, p.dy, p.x, p.y);
        debugtext.setString(buf);
    #endif

        p.width_wrap(vw);
        p.update();

        game.draw();
        win.draw(debugtext);
        win.display();
    }

    win.clear();
    win.draw(background);
    if (game.end_screen() == Game::Again)
    {
        goto Gameplay;
    }
    return 0;
}

static bool should_close(sf::RenderWindow& win)
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