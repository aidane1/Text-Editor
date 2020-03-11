#pragma once

#include <fstream>
#include <iostream>
#include "../Definitions/defines.cpp"

char *openFile(const char *file)
{
    std::streampos begin, end;
    std::ifstream fileStream;
    fileStream.open(file);
    begin = fileStream.tellg();
    fileStream.seekg(0, std::ios::end);
    end = fileStream.tellg();
    int size = (end - begin);
    char *memblock = new char[size + 1];
    fileStream.seekg(0, std::ios::beg);
    fileStream.read(memblock, size);
    fileStream.close();
    memblock[size] = '\0';
    return memblock;
}