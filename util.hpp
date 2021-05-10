#pragma once

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Text.hpp>

void center_origin(sf::Transformable& obj, sf::FloatRect bounds);
void center_text(sf::Text& text);