#include <SFML/Graphics.hpp>
#include <string.h>

#include <iostream>
#include "doodle_png.i"
#include "background.i"

struct Player {
    unsigned int height, width;
    float x, y;
};

int main(void)
{
    sf::VideoMode video(500, 800);
    sf::RenderWindow win(video, "Doodle Jump", sf::Style::Close);

    sf::Texture backgroundtex;
    sf::Texture playertex;
    sf::Sprite background;
    sf::Sprite player;
    Player p;

    playertex.loadFromMemory(doodle_png, doodle_png_len);
    player.setTexture(playertex);
    p.height = playertex.getSize().y;
    p.width = playertex.getSize().x;
    p.x = 250;
    p.y = 800 - p.height - 1;
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
            p.y -= 20;
        }

        p.y += 2; // gravity

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
        win.draw(player);
        win.display();
    }
    return 0;
}