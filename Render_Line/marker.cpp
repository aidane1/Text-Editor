#pragma once
#include <SFML/Graphics.hpp>
#include "../Definitions/defines.cpp"
#include <string>

struct Line_Marker
{
    int line;
    sf::Text marker;
    Line_Marker(int line_num, sf::Text text) : marker(text)
    {
        line = line_num;
    }
    Line_Marker()
    {
    }
};