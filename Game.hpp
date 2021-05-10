#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>

class Game {
    sf::RenderWindow& win;

public:
    static constexpr float Gravity   = 0.3f;
    static constexpr float Jump      = 10.0f;
    static constexpr float MoveSpeed = 5.0f;
    static constexpr float Friction  = 0.85f;

    sf::Font font;
    sf::Text score;

    Game(sf::RenderWindow& win)
      : win(win)
    {
        win.setFramerateLimit(60);
        font.loadFromFile("./assets/arial.ttf");

        score.setFont(font);
        score.setFillColor(sf::Color::Black);
        score.setCharacterSize(25);
        score.setString("Score: 0");
    }

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states)
    {
        win.draw(drawable, states);
    }

    void draw(const sf::Drawable& drawable)
    {
        win.draw(drawable, sf::RenderStates::Default);
    }

    int show_opening_screen();
    int end_screen();

    void set_score(int value)
    {
        score.setString("Score: " + std::to_string(value));
    }
};
