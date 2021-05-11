#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>

#include "Player.h"
#include "PlatformCtrl.h"

class Game {
    sf::RenderWindow& win;
    sf::Sprite background;
    Player& p;
    PlatformCtrl& plts;
    bool paused;

public:
    static constexpr int   MaxPlayerHeight = 200;
    static constexpr float Gravity         = 0.30f,
                           Jump            = 10.0f,
                           MoveSpeed       = 6.00f,
                           Friction        = 0.85f;

    enum Action {
        ActionNoOp,
        Done,
        Again,
        Start,
    };

    sf::Font font;
    sf::Text score;

    Game(sf::RenderWindow& win, Player& player, PlatformCtrl& plts)
      : win(win), p(player), plts(plts)
    {
        win.setFramerateLimit(60);
        font.loadFromFile("./assets/arial.ttf");

        score.setFont(font);
        score.setFillColor(sf::Color::Black);
        score.setCharacterSize(25);
        score.setString("Score: 0");
        paused = false;
    }

    void draw();
    void draw(const sf::Drawable& drawable, const sf::RenderStates& states)
    {
        win.draw(drawable, states);
    }

    void draw(const sf::Drawable& drawable)
    {
        win.draw(drawable, sf::RenderStates::Default);
    }

    void set_score(int value)
    {
        score.setString("Score: "+std::to_string(value));
    }

    Action show_opening_screen();
    Action end_screen();
    Action pause();

    void set_background(sf::Sprite bg) { background = bg; }
    void setup_game_over();

    void apply_debug_controlls();
};
