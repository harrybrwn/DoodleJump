#pragma once

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Text.hpp>

#include <math.h>

void center_origin(sf::Transformable& obj, sf::FloatRect bounds);
void center_text(sf::Text& text);

inline float vec_distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    float dx = std::abs(a.x - b.x);
    float dy = std::abs(a.y - b.y);
    return std::sqrt((dx*dx) + (dy*dy));
}