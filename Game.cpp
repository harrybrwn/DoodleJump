#include "Game.hpp"
#include "util.hpp"

int Game::show_opening_screen()
{
    sf::Vector2f view = win.getView().getSize();
    sf::Text text("Not", font, 50);

    sf::Texture playtexture;
    sf::Sprite play;

    sf::Texture titletexture;
    sf::Sprite title;

    titletexture.loadFromFile("assets/doodle-jump.png");
    title.setTexture(titletexture);
    title.setPosition((view.x/2)-50, (view.y/2)-120);
    title.scale(0.65, 0.65);

    text.setFillColor(sf::Color::Black);
    text.setPosition((view.x / 2)-100, (view.y / 2) - 100);
    text.setRotation(-20);
    center_text(text);

    playtexture.loadFromFile("./assets/play.png");
    play.setPosition(view.x / 2, (view.y / 2)+100);
    play.setTexture(playtexture);
    play.scale(0.75, 0.75);
    center_origin(play, play.getLocalBounds());
    sf::FloatRect btn = play.getGlobalBounds();

    win.draw(play);
    bool should_start = false;
    sf::Event event;
    while (win.isOpen())
    {
        while (win.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                win.close();
                return -1;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Q:
                case sf::Keyboard::Escape:
                    win.close();
                    return -1;
                case sf::Keyboard::Enter:
                    play.setColor(sf::Color(0x000000c0));
                    should_start = true;
                    win.draw(play);
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Enter && should_start)
                    return 0;
                break;
            case sf::Event::MouseButtonPressed:
                if (
                    event.mouseButton.button == sf::Mouse::Left &&
                    btn.contains(event.mouseButton.x, event.mouseButton.y) &&
                    !should_start)
                {
                    play.setColor(sf::Color(0x000000c0));
                    should_start = true;
                    win.draw(play);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (
                    event.mouseButton.button == sf::Mouse::Left &&
                    should_start &&
                    btn.contains(event.mouseButton.x, event.mouseButton.y)
                ) {
                    return 0;
                }
                should_start = false;
                play.setColor(sf::Color(0xffffffff));
                win.draw(play);
                break;
            }
        }

        win.draw(text);
        win.draw(title);
        win.display();
    }
    return 0;
}

int Game::end_screen()
{
    sf::Vector2f view = win.getView().getSize();
    sf::Text endscreen("Game Over", font, 50);
    endscreen.setFillColor(sf::Color::Black);
    endscreen.setPosition(view.x / 2, view.y / 2);
    center_text(endscreen);

    win.draw(endscreen);
    sf::Event event;
    while (win.isOpen())
    {
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
        win.display();
    }
}