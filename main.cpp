#include <SFML/Graphics.hpp>

#include <random>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "Game.hpp"
#include "PlatformCtrl.h"
#include "Player.h"
#include "util.hpp"

#define DEBUG

using namespace std;

static bool should_close(sf::RenderWindow& win);
static void debug_hitbox(sf::FloatRect hitbox, sf::RenderWindow& win);
void end_screen(sf::RenderWindow& win, sf::Font& font);

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

    int playerDistTraveled = 0;

    Player p("assets/PixelThing.png");
    p.x = vw / 2;
    p.y = vh - (vh/4);

    // platform stuff
    static const int nplatforms = 13;
    sf::RectangleShape platforms[nplatforms];
    PlatformCtrl platform_ctrl(platforms, nplatforms);
    platform_ctrl.set_bounds(vw, vh);
    platform_ctrl.randomize();
    // guarantee that a platform is under the player at spawn
    platforms[nplatforms-1].setPosition(
        p.x-(PlatformCtrl::PlatformWidth/2),
        p.y+(p.height*2));

    // Create the window
    sf::VideoMode video(vw, vh);
    sf::RenderWindow win(video, "Not Doodle Jump ;)", sf::Style::Close);
    Game game(win);
    game.score.setPosition(vw-175, 50);

    sf::Text debugtext("", game.font, 18);
    debugtext.setFillColor(sf::Color::Black);

    win.draw(background);
    if (game.show_opening_screen() < 0)
        return 0;

    win.clear();
    while (win.isOpen())
    {
        // Handle window events
        if (should_close(win))
        {
            return 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            p.sprite.setScale(-1, 1);
            p.dx = -Game::MoveSpeed; // Moves Left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            p.look_right();
            p.dx = Game::MoveSpeed; // Moves Right
        }

    #ifdef DEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            p.jump(-Game::Jump);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            p.y += 1; // for debugging
    #endif

        // if hitting a platform while falling
        if (p.dy >= 0 && platform_ctrl.hit_platform(p.jump_hitbox()))
        {
            cout << "platform hit: " << p.dx << ", " << p.dy << std::endl;
            p.dy = -Game::Jump;
        }
        else
        {
            p.dy += Game::Gravity;
        }

        if (p.y < 200 && p.dy < 0)
        {
            platform_ctrl.shift_up(-p.dy);
            p.y = 200;
            playerDistTraveled -= (int)p.dy; // update score
        }

        // Bottom edge detection
        if (p.y > vh-p.height)
        {
            // TODO move all platforms up until there are no more.
            break; // break out of main game loop
        }

        p.dx *= Game::Friction; // friction/wind-resistance... something like that
        p.x  += p.dx;
        p.y  += p.dy;

        // Update score
        if (playerDistTraveled <= (p.y - vh+p.height) * -1)
        {
            playerDistTraveled = (p.y-vh+p.height) * -1;
        }
        game.set_score(playerDistTraveled);

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
        char buf[500];
        snprintf(buf, sizeof(buf),
            "velocity: <%f, %f>\nposition: <%f, %f>\n",
            p.dx, p.dy, p.x, p.y);
        debugtext.setString(buf);
    #endif

        p.width_wrap(vw);
        p.update();

        win.draw(background);
        for (int i = 0; i < nplatforms; i++)
            win.draw(platforms[i]);
        win.draw(p.sprite);
        win.draw(debugtext);
        game.draw(game.score);
        win.display();
    }

    win.clear();
    win.draw(background);
    game.end_screen();
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