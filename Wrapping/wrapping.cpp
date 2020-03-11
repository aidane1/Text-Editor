#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Definitions/defines.cpp"

int wrap_text(std::string, sf::Font);

int wrap_text(std::string &, std::vector<int> *, int, sf::Font);

int wrap_word(std::string &, int, sf::Font);

std::vector<int> *find_breaks(std::string);

std::string insert_text_breaks(std::string, sf::Font);

std::vector<int> *find_breaks(std::string data)
{
    std::vector<int> *breaks = new std::vector<int>();

    breaks->reserve(data.length() / 5);

    for (int i = 0; i < data.length(); i++)
    {
        if (isspace(data[i]))
        {
            breaks->push_back(i);
        }
    }
    return breaks;
}

//takes a string as an input and returns a new string with line breaks inserted to wrap the text
std::string insert_text_breaks(std::string text, sf::Font font)
{
    //copies the string to a new string so as to not mutate the original string
    auto return_val = text;
    //keeps a base index of where the string is currently being edited
    int base_index = 0;
    //infinitely loops until the break condition is met
    while (1)
    {
        int index = wrap_text(text, font);
        if (index == -1)
            break;
        //removes the current line from the start of the text so as to only wrap the next lines
        text = text.substr(index, text.length());
        base_index += index;
        return_val[base_index] = '\n';
    }
    return return_val;
}

// Base function to calculate the index at which to wrap text
int wrap_text(std::string string_data, sf::Font font)
{

    //assemble a vector of all the indexes of whitespace characters
    std::vector<int> *breaks = find_breaks(string_data);

    // if the line has spaces
    if (breaks->size() > 0)
    {
        //overloaded function that recursively finds the largest index at which the line could be broken
        return wrap_text(string_data, breaks, 0, font);
    }
    else
    {
        return wrap_word(string_data, 1, font);
    }

    //free allocation for indexes vector
    delete breaks;
}

int wrap_word(std::string &word, int index, sf::Font font)
{
    //render the text with the current index to see how large it is
    sf::Text text;
    text.setFont(font);
    text.setString(word.substr(0, index));
    text.setCharacterSize(FONT_SIZE);
    text.setLetterSpacing(LETTER_SPACING);
    text.setLineSpacing(LINE_SPACING);
    sf::FloatRect text_width = text.getLocalBounds();

    if (text_width.width > WIDTH)
    {
        return index - 1;
    }
    else
    {
        if (index >= word.length())
        {
            return -1;
        }
        else
        {
            return wrap_word(word, index + 1, font);
        }
    }
}

//Overloaded wrap_text that minimizes function arguments from main call
int wrap_text(std::string &full_string, std::vector<int> *breaks, int index, sf::Font font)
{

    //render the text with the current index to see how large it is
    sf::Text text;
    text.setFont(font);
    text.setString(full_string.substr(0, (*breaks)[index]));
    text.setCharacterSize(FONT_SIZE);
    text.setLetterSpacing(LETTER_SPACING);
    text.setLineSpacing(LINE_SPACING);
    sf::FloatRect text_width = text.getLocalBounds();

    //if current text width is larger than screen, go to previous index and return
    if (text_width.width > WIDTH)
    {
        return (*breaks)[index - 1];
    }

    //otherwise, run the function again with one more on the index
    else
    {
        //if there doesn't need to be text wrap, return -1
        if (index >= breaks->size())
        {
            return -1;
        }
        return wrap_text(full_string, breaks, index + 1, font);
    }
}
