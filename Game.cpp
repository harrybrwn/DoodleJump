#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.hpp"
#include "util.hpp"

class Button;

static Game::Action handle_game_screen_buttons(sf::RenderWindow& win, Button& btn);

/**
 * Displayable is a thin wrapper around the sprite class that loads it's
 * own texture automatically.
 */
class Displayable : public sf::Sprite {
protected:
    sf::Texture tx;

public:
    Displayable(std::string texture_file)
    {
        tx.loadFromFile(texture_file);
        setTexture(tx);
    }
};

/**
 * Button is a button class that loads it's own
 * texture and handles click/press state.
 */
class Button : public Displayable {
    sf::FloatRect bounds;
    bool pressed;

public:
    Button(std::string texture_file)
      : Displayable(texture_file)
    {
        pressed = false;
        bounds = getLocalBounds();
    }

    bool clicked(sf::Event& event)
    {
        bounds = getGlobalBounds();
        return (
            event.mouseButton.button == sf::Mouse::Left &&
            bounds.contains(event.mouseButton.x, event.mouseButton.y)
        );
    }

    bool clicked(int x, int y)
    {
        bounds = getGlobalBounds();
        return bounds.contains(x, y);
    }

    bool is_pressed() { return pressed; }

    void set_pressed(bool v)
    {
        pressed = v;
        if (v)
            setColor(sf::Color(0x000000c0));
        else
            setColor(sf::Color(0xffffffff));
    }
};

void Game::draw()
{
    win.draw(background);
    plts.draw_with(win);
    win.draw(p.sprite);
    win.draw(score);
}


Game::Action Game::show_opening_screen()
{
    sf::Vector2f view = win.getView().getSize();
    sf::Text text("Not", font, 50);
    Button play("./assets/play.png");
    Displayable title("./assets/doodle-jump.png");

    title.setPosition((view.x/2)-50, (view.y/2)-120);
    title.scale(0.65, 0.65);

    text.setFillColor(sf::Color::Black);
    text.setPosition((view.x / 2)-100, (view.y / 2) - 100);
    text.setRotation(-20);
    center_text(text);

    play.setPosition(view.x / 2, (view.y / 2)+100);
    play.scale(0.75, 0.75);
    center_origin(play, play.getLocalBounds());
    sf::FloatRect btn = play.getGlobalBounds();

    win.draw(text);
    win.draw(title);
    win.draw(play);

    Game::Action action = handle_game_screen_buttons(win, play);
    if (action == Game::ActionNoOp)
        return Game::Start;
    return action;
}

Game::Action Game::end_screen()
{
    sf::Vector2f view = win.getView().getSize();
    sf::Text endscreen("Game Over", font, 50);
    endscreen.setFillColor(sf::Color::Black);
    endscreen.setPosition(view.x / 2, view.y / 2);
    center_text(endscreen);

    Button again("assets/play-again-on@2x.png");
    again.setPosition(view.x / 2, (view.y / 2)+100);
    again.scale(0.75, 0.75);
    center_origin(again, again.getLocalBounds());

    win.draw(endscreen);
    win.draw(again);

    int rot = 0;
    float theta = 1.0;
    float scale = 1.0;
    auto btn = again;
    // Game::Action action = handle_game_screen_buttons(win, again);
    sf::Event event;
    while (win.isOpen())
    {
        while (win.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                // Handle the "x" button
                win.close();
                return Game::Done;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Q:
                case sf::Keyboard::Key::Escape:
                    // Close on "Esc"
                    win.close();
                    return Game::Done;
                case sf::Keyboard::Enter:
                    btn.set_pressed(true);
                    win.draw(btn);
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Enter && btn.is_pressed())
                    return Game::Again;
                break;
            case sf::Event::MouseButtonPressed:
                if (btn.clicked(event) && !btn.is_pressed())
                {
                    btn.set_pressed(true);
                    win.draw(btn);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (btn.clicked(event) && btn.is_pressed())
                    return Game::Again;
                break;
            }
        }
        theta += 0.03;
        rot = (rot + 1) % 360;
        scale = std::sin(theta);
        endscreen.setRotation(rot);
        endscreen.setScale(scale, scale);
        win.draw(background);
        win.draw(endscreen);
        win.draw(again);
        win.display();
    }
    return Game::Done;
    // if (action == Game::ActionNoOp)
    //     return Game::Done;
    // return action;
}

static Game::Action handle_game_screen_buttons(sf::RenderWindow& win, Button& btn)
{
    sf::Event event;
    while (win.isOpen())
    {
        while (win.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                // Handle the "x" button
                win.close();
                return Game::Done;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Q:
                case sf::Keyboard::Key::Escape:
                    // Close on "Esc"
                    win.close();
                    return Game::Done;
                case sf::Keyboard::Enter:
                    btn.set_pressed(true);
                    win.draw(btn);
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Enter && btn.is_pressed())
                    return Game::Again;
                break;
            case sf::Event::MouseButtonPressed:
                if (btn.clicked(event) && !btn.is_pressed())
                {
                    btn.set_pressed(true);
                    win.draw(btn);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (btn.clicked(event) && btn.is_pressed())
                    return Game::Again;
                break;
            }
        }
        win.display();
    }
    return Game::ActionNoOp;
}

Game::Action Game::pause()
{
    sf::Texture tx;
    sf::Sprite bg;
    sf::Event event;
    if (paused)
        return Game::Start;
    paused = true;

    tx.loadFromFile("assets/pause.png");
    bg.setTexture(tx);
    win.draw(bg);

    while (win.isOpen())
    {
        while (win.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                win.close();
                return Game::Done;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Q:
                    win.close();
                    return Game::Done;
                case sf::Keyboard::Key::Escape:
                case sf::Keyboard::P:
                    paused = false;
                    return Game::Start;
                }
                break;
            }
            win.display();
        }
    }
    return Game::ActionNoOp;
}


void Game::setup_game_over()
{
    int a = -10;
    int nplatforms = plts.count();
    auto platforms = plts.get_platforms();

    while (win.isOpen())
    {
        p.update();
        p.dx *= Game::Friction;
        p.x += p.dx;
        plts.shift_up(a);
        for (int i = 0; i < nplatforms; i++)
            if (platforms[i].getPosition().y > 0)
                goto keepShifting;
        return;
    keepShifting:
        draw();
        win.display();
        a -= 1;
    }
}

void Game::apply_debug_controlls()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i pos = sf::Mouse::getPosition(win);
        if (pos.y < Game::MaxPlayerHeight)
        {
            plts.shift_up(Game::MaxPlayerHeight-pos.y);
            p.distTraveled += Game::MaxPlayerHeight-pos.y;
        }
        p.x = pos.x;
        p.y = pos.y;
        p.dy = 0;
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        p.dy = -40; // jump really high
}