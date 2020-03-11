#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "../Rope/rope.cpp"
#include "../Render_Line/line.cpp"
#include "../Definitions/defines.cpp"

struct Cursor_Position
{
    int character;
    int line;
    int offset_x;
    int offset_y;
    sf::Color color;
    sf::RectangleShape rendered_position;
    bool is_shown = true;
    int current_delta;
    Cursor_Position(int line_num, int chara, int x, int y, int delta, sf::Color cursorColor = sf::Color(180, 180, 220)) : rendered_position(sf::Vector2f(5, FONT_SIZE * LINE_SPACING))
    {
        character = chara;
        line = line_num;
        offset_x = x;
        offset_y = y;
        color = cursorColor;
        current_delta = delta;
        rendered_position.setPosition(sf::Vector2f(offset_x, offset_y));
        rendered_position.setFillColor(cursorColor);
        rendered_position.setOutlineColor(sf::Color::Black);
        rendered_position.setOutlineThickness(1.0);
    }
    void updatePosition(int line_num, int chara, int x, int y, int delta)
    {
        offset_x = x;
        offset_y = y;
        line = line_num;
        character = chara;
        current_delta = delta;
        rendered_position.setPosition(sf::Vector2f(offset_x, offset_y));
    }
    void scroll(int mouseDelta)
    {
        rendered_position.setPosition(sf::Vector2f(offset_x, offset_y - mouseDelta + current_delta));
    }
    void switchShown()
    {
        is_shown = !is_shown;
    }
    void pushForward(int num_characters, Rope *line_text, sf::Font font, Render_Line rendered_line)
    {
        std::string text_data = line_text->serialize_display(font);

        int numBreaks = 0;
        for (int j = 0; j < text_data.length(); j++)
        {
            if (text_data[j] == '\n')
                numBreaks++;
        }

        int line_start_index = 0;
        int line_num = 1;
        for (int j = 0; j < text_data.length(); j++)
        {
            if (text_data[j] == '\n')
            {
                if (j > character + num_characters)
                    break;
                line_start_index = j;
                line_num++;
            }
        }
        sf::Text text;
        text.setFont(font);
        text.setString(text_data.substr(line_start_index, character + num_characters - line_start_index + 1));
        text.setCharacterSize(FONT_SIZE);
        text.setLetterSpacing(LETTER_SPACING);
        text.setLineSpacing(LINE_SPACING);
        sf::FloatRect text_dimensions = text.getGlobalBounds();
        character += num_characters;

        updatePosition(line, character, text_dimensions.width + SCREEN_WIDTH - WIDTH, rendered_line.text.getGlobalBounds().top + std::floor(rendered_line.text.getGlobalBounds().height / (numBreaks + 1) * (line_num - 1)), current_delta);
    }
};