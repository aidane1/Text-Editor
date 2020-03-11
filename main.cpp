#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Wrapping/wrapping.cpp"
#include "Definitions/defines.cpp"
#include "Rope/rope.cpp"
#include "Disk_Access/access.cpp"
#include "Render_Line/line.cpp"
#include "Render_Line/marker.cpp"
#include "Cursor/cursor.cpp"
#include "Cursor/intersection.cpp"
#include "Gap_Buffer/buffer.cpp"

// void updateLines(std::vector<Render_Line> &lines, std::vector<Line_Marker> &markers, int mouseDelta)
void updateLines(std::vector<Render_Line> &lines, std::vector<Line_Marker> &markers, int mouseDelta)
{
    int base = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i].text.setPosition(sf::Vector2f(SCREEN_WIDTH - WIDTH, base - mouseDelta));
        markers[i].marker.setPosition(sf::Vector2f(SCREEN_WIDTH - WIDTH - MARKER_MARGIN, base - mouseDelta));
        base += lines[i].height + LINE_SPACING * FONT_SIZE;
    }
}

int main(int argc, char *argv[])
{

    char *fileData;
    if (argc > 1)
    {
        fileData = openFile(argv[1]);
    }
    int mouseDelta = 0;

    Cursor_Position cursor(0, -1, SCREEN_WIDTH - WIDTH, 10, 9);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Text Editor", sf::Style::Close);

    sf::Font font;
    // if (!font.loadFromFile("Fonts/Roboto-Medium.ttf"))
    // {
    // };
    if (!font.loadFromFile("Fonts/FiraCode-Retina.ttf"))
    {
    }

    char *token = std::strtok(fileData, "\n");

    std::vector<Rope *> lines;

    while (token != nullptr)
    {
        lines.push_back(new Rope(true, token, nullptr));
        token = std::strtok(nullptr, "\n");
    }

    std::vector<Render_Line> text_lines;
    std::vector<Line_Marker> line_markers;

    for (int i = 0; i < lines.size(); i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(lines[i]->serialize_display(font));
        text.setCharacterSize(FONT_SIZE);
        text.setFillColor(sf::Color::White);
        text.setLineSpacing(LINE_SPACING);
        text.setLetterSpacing(LETTER_SPACING);
        text_lines.push_back(Render_Line(text));

        sf::Text marker_text;
        marker_text.setFont(font);
        marker_text.setString(std::to_string(i + 1));
        marker_text.setCharacterSize(FONT_SIZE);
        marker_text.setFillColor(sf::Color::White);
        marker_text.setLetterSpacing(LETTER_SPACING);
        marker_text.setLineSpacing(LINE_SPACING);
        line_markers.push_back(Line_Marker(i + 1, marker_text));
    }

    updateLines(text_lines, line_markers, mouseDelta);

    sf::RectangleShape marker_border;
    marker_border.setSize(sf::Vector2f(2, SCREEN_HEIGHT));
    marker_border.setPosition(sf::Vector2f(SCREEN_WIDTH - WIDTH - 18, 0));
    marker_border.setFillColor(sf::Color::Red);

    sf::Clock clock;

    sf::Clock framerate_clock;

    bool is_left_pressed = false;

    while (window.isOpen())
    {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                mouseDelta -= event.mouseWheel.delta * 3;
                mouseDelta = mouseDelta >= 0 ? mouseDelta : 0;
                updateLines(text_lines, line_markers, mouseDelta);
                cursor.scroll(mouseDelta);
                cursor.is_shown = true;
            }
            else if (event.type == sf::Event::TextEntered)
            {
                // std::cout << event.text.unicode << "\n";
                switch (event.text.unicode)
                {
                case 8:
                {
                    lines[cursor.line] = Rope::delete_substring(lines[cursor.line], cursor.character, 1);
                    text_lines[cursor.line].text.setString(lines[cursor.line]->serialize_display(font));
                    text_lines[cursor.line].height = text_lines[cursor.line].text.getGlobalBounds().height;
                    cursor.pushForward(-1, lines[cursor.line], font, text_lines[cursor.line]);
                    updateLines(text_lines, line_markers, mouseDelta);
                    break;
                }

                case 10:
                {
                    break;
                }

                default:
                {
                    if (cursor.character != -1)
                    {
                        std::string string_data;
                        string_data.push_back(event.text.unicode);
                        lines[cursor.line] = Rope::insert(lines[cursor.line], cursor.character, string_data);
                        text_lines[cursor.line].text.setString(lines[cursor.line]->serialize_display(font));
                        text_lines[cursor.line].height = text_lines[cursor.line].text.getGlobalBounds().height;
                        cursor.pushForward(1, lines[cursor.line], font, text_lines[cursor.line]);
                        updateLines(text_lines, line_markers, mouseDelta);
                        break;
                    }
                    else
                    {
                        std::string string_data;
                        string_data.push_back(event.text.unicode);
                        lines[cursor.line] = Rope::concat(new Rope(string_data, nullptr), lines[cursor.line]);

                        // lines[cursor.line] = Rope::insert(lines[cursor.line], cursor.character, string_data);

                        text_lines[cursor.line].text.setString(lines[cursor.line]->serialize_display(font));
                        text_lines[cursor.line].height = text_lines[cursor.line].text.getGlobalBounds().height;

                        cursor.pushForward(1, lines[cursor.line], font, text_lines[cursor.line]);
                        updateLines(text_lines, line_markers, mouseDelta);

                        break;
                    }
                }
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {

                switch (event.key.code)
                {
                case 58:
                {
                    Rope *new_line = lines[cursor.line]->split(cursor.character);
                    lines.push_back(new_line);

                    text_lines[cursor.line].text.setString(lines[cursor.line]->serialize_display(font));
                    text_lines[cursor.line].height = text_lines[cursor.line].text.getGlobalBounds().height;

                    sf::Text text;
                    text.setFont(font);
                    text.setString(new_line->serialize_display(font));
                    text.setCharacterSize(FONT_SIZE);
                    text.setFillColor(sf::Color::White);
                    text.setLineSpacing(LINE_SPACING);
                    text.setLetterSpacing(LETTER_SPACING);
                    text_lines.insert(text_lines.begin() + cursor.line + 1, Render_Line(text));

                    sf::Text marker_text;
                    marker_text.setFont(font);
                    marker_text.setString(std::to_string(cursor.line + 2));
                    marker_text.setCharacterSize(FONT_SIZE);
                    marker_text.setFillColor(sf::Color::White);
                    marker_text.setLetterSpacing(LETTER_SPACING);
                    marker_text.setLineSpacing(LINE_SPACING);
                    line_markers.insert(line_markers.begin() + cursor.line + 1, Line_Marker(cursor.line + 2, marker_text));

                    updateLines(text_lines, line_markers, mouseDelta);

                    std::cout << text.getGlobalBounds().top << "\n";

                    cursor.updatePosition(cursor.line + 1, -1, SCREEN_WIDTH - WIDTH, text.getGlobalBounds().top, mouseDelta);

                    break;
                }
                case 18:
                {
                    if (event.key.system)
                    {
                        if (argc > 1)
                        {
                            std::string data = "";
                            for (int i = 0; i < lines.size(); i++)
                            {
                                data += lines[i]->serialize_data();
                                data += i != lines.size() - 1 ? "\n" : "";
                            }
                            saveFile(argv[1], data);
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!is_left_pressed)
            {
                is_left_pressed = true;

                check_intersection(sf::Mouse::getPosition(window), text_lines, lines, font, cursor, mouseDelta);

                std::cout << cursor.character << "\n";
            }
        }
        else if (is_left_pressed)
        {
            is_left_pressed = false;
        }

        for (int i = 0; i < text_lines.size(); i++)
        {
            window.draw(text_lines[i].text);
        }
        for (int i = 0; i < line_markers.size(); i++)
        {
            window.draw(line_markers[i].marker);
        }
        if (cursor.is_shown)
        {
            window.draw(cursor.rendered_position);
        }
        window.draw(marker_border);

        window.display();

        sf::Time elapsed = clock.getElapsedTime();
        // std::cout << "Framerate: " << 1.0/elapsed.asSeconds() << "\n";

        if (elapsed.asMilliseconds() > 500)
        {
            cursor.switchShown();
            clock.restart();
        }

        // std::cout << 1.0/framerate_clock.getElapsedTime().asSeconds() << "\n";
        // framerate_clock.restart();

        // if (elapsed.asMicroseconds() < 1000000.0/60) {
        //     sf::Time sleep;
        //     sleep.asMicroseconds()
        //     sf::sleep(sleep);
        // }
    }

    delete fileData;

    return 0;
}