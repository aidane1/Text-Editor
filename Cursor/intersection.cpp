#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "../Rope/rope.cpp"
#include "../Render_Line/line.cpp"
#include "./cursor.cpp"
#include "../Gap_Buffer/buffer.cpp"

void check_intersection(sf::Vector2i mouse, std::vector<Render_Line> &lines, std::vector<Rope *> &text, sf::Font font, Cursor_Position &cursor, int mouseDelta)
{
    // loop over all the lines until you find the one the click was in
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].text.getGlobalBounds().contains(mouse.x, mouse.y))
        {

            // Find the fraction of the width and height of the box the click was in
            sf::FloatRect bounds = lines[i].text.getGlobalBounds();
            float percent_top = (mouse.y - bounds.top) / bounds.height;
            float percent_left = (mouse.x - bounds.left) / bounds.width;

            // get the raw string that is being rendered
            std::string text_data = text[i]->serialize_display(font);
            // std::cout << text_data << "\n";

            //find how many lines are in that string
            int numBreaks = 0;
            for (int j = 0; j < text_data.length(); j++)
            {
                if (text_data[j] == '\n')
                    numBreaks++;
            }

            // find what line the click was on
            int line = std::ceil((numBreaks + 1) * percent_top);
            int breaks_found = 0;
            int line_start_index = 0;
            int line_end_index = line == numBreaks + 1 ? text_data.length() : 0;
            // find the character index of the first and last character of that line
            for (int j = 0; j < text_data.length(); j++)
            {
                if (text_data[j] == '\n')
                {
                    breaks_found++;
                    if (breaks_found == line - 1)
                        line_start_index = j + 1;
                    else if (breaks_found == line)
                        line_end_index = j;
                }
            }
            // std::cout << "length: " << breaks_found << "\n";
            //make a guess as to what character was clicked based on the fractions
            int check_index = std::ceil((line_end_index - line_start_index) * percent_left);

            //calculate the width of that guess to see how it compares to the actual mouse click
            sf::Text text;
            text.setFont(font);
            text.setPosition(SCREEN_WIDTH - WIDTH, 0);
            text.setString(text_data.substr(line_start_index, check_index + 1));
            text.setCharacterSize(FONT_SIZE);
            text.setLetterSpacing(LETTER_SPACING);
            text.setLineSpacing(LINE_SPACING);

            sf::FloatRect text_dimensions = text.getGlobalBounds();

            bool guess_is_larger = text_dimensions.width + SCREEN_WIDTH - WIDTH > mouse.x;

            bool found = false;

            //while the character that was clicked hasn't been found, move the current checked index closer to
            //it and re-calculate width
            while (!found)
            {
                text.setString(text_data.substr(line_start_index, check_index + 1));
                text_dimensions = text.getGlobalBounds();

                if (guess_is_larger)
                {
                    if (text_dimensions.width + SCREEN_WIDTH - WIDTH < mouse.x)
                    {
                        found = true;
                    }
                    else if (check_index-- < 0)
                        break;
                }
                else
                {
                    if (text_dimensions.width + SCREEN_WIDTH - WIDTH > mouse.x)
                    {
                        found = true;
                    }
                    if (check_index++ > line_end_index - line_start_index)
                        break;
                }
            }
            // normalize
            if (guess_is_larger)
            {
                check_index++;
            }
            else
            {
                check_index--;
            }

            // calculate final width
            text.setString(text_data.substr(line_start_index, check_index + 1));
            text_dimensions = text.getGlobalBounds();

            //update the cursor with the new info
            cursor.updatePosition(i, line_start_index + check_index, SCREEN_WIDTH - WIDTH + text_dimensions.width, lines[i].text.getGlobalBounds().top + std::floor(lines[i].text.getGlobalBounds().height / (numBreaks + 1) * (line - 1)), mouseDelta);
        }
    }
}