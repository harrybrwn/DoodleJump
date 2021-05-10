#include "util.hpp"

void center_origin(sf::Transformable& obj, sf::FloatRect bounds)
{
    obj.setOrigin(bounds.left + bounds.width/2.0f,
                  bounds.top + bounds.height/2.0f);
}

void center_text(sf::Text& text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    center_origin(text, bounds);
}
