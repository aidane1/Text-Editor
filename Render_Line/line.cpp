#pragma once
#include <SFML/Graphics.hpp>

struct Render_Line
{
    sf::Text text;
    int width;
    int height;
    Render_Line(sf::Text text) : text(text)
    {
        width = text.getLocalBounds().width;
        height = text.getLocalBounds().height;
    }
    Render_Line() {
        
    }
};