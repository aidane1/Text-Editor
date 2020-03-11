#pragma once

#include <fstream>
#include <iostream>
#include "../Definitions/defines.cpp"
#include "./open.cpp"

char *saveFile(const char *file, std::string data)
{
    std::ofstream fileStream;
    fileStream.open(file, std::ios_base::out);
    if (fileStream.is_open())
    {
        fileStream << data;
    }
    fileStream.close();
    return openFile(file);
}