#pragma once

#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

template <class T>
class Gap_Buffer
{
public:
    std::vector<T> data;
    std::vector<int> indicies;
    int gap;
    int base_gap;
    int gap_index;

public:
    Gap_Buffer(int base_length, int gap_size)
    {
        data.resize(base_length + gap_size);
        gap_index = 0;
        gap = gap_size;
        base_gap = gap_size;
    }
    int size()
    {
        return indicies.size();
    }

    T &operator[](int index)
    {
        return data[indicies[index]];
    }
    void display()
    {
        for (int i = 0; i < data.size(); i++)
        {
            std::cout << data[i] << "\n";
        }
    }
    void display_index()
    {
        for (int i = 0; i < indicies.size(); i++)
        {
            std::cout << indicies[i] << "\n";
        }
    }
    void move_after(int index)
    {
        std::cout << "index: " << (indicies[index] + 1) << "\n";
        move_gap(indicies[index] + 1);
        std::cout << "Tits 11"
                  << "\n";
    }
    void move_gap(int new_index)
    {
        if (new_index + gap > data.size())
        {
            new_index = data.size() - gap;
        }
        if (new_index > gap_index)
        {

            for (int i = gap_index + gap; i < gap_index + gap + (new_index - gap_index); i++)
            {
                data[i - gap] = data[i];
            }

            for (int i = 0; i < indicies.size(); i++)
            {
                if (indicies[i] > gap_index && indicies[i] < (new_index + gap))
                {
                    indicies[i] -= gap;
                }
            }
        }
        else if (new_index < gap_index)
        {
            std::cout << "tits 6"
                      << "\n";
            for (int i = new_index - 1; i >= gap_index; i--)
            {
                data[i + gap] = data[i];
            }
            std::cout << "tits 7"
                      << "\n";
            for (int i = 0; i < indicies.size(); i++)
            {
                if (indicies[i] >= new_index && indicies[i] < gap_index)
                {
                    indicies[i] += gap;
                }
            }
            std::cout << "tits 8"
                      << "\n";
        }
        std::cout << "tits 9"
                  << "\n";
        gap_index = new_index;
        std::cout
            << "tits 10"
            << "\n";
    }
    void set_gap(int size)
    {
        std::cout << "resize: " << data.size() + size - gap << "\n";
        if (size > gap)
        {
            data.resize(data.size() + size - gap);
            for (int i = data.size() - (size - gap); i > (gap_index + gap - 1); i--)
            {
                data[i + (size - gap)] = data[i];
            }
            for (int i = 0; i < indicies.size(); i++)
            {
                if (indicies[i] >= gap_index + gap)
                {
                    indicies[i] += (size - gap);
                }
            }
            gap = size;
        }
        std::cout << "done" << "\n";
    }
    void insert(T insert_data)
    {
        // display_index();
        // std::cout << "\n\n" << gap_index << "\n\n";
        // std::cout << "Test 1" << "\n";
        std::cout << "gap: " << gap << "\n";
        if (gap > 2)
        {
            data[gap_index] = insert_data;
            gap--;
            indicies.push_back(gap_index);
            sort_indicies();
            gap_index++;
            
        }
        else
        {
            std::cout << "Oh no! out of gap." << "\n";
            set_gap(base_gap);
            insert(insert_data);
        }
        // display_index();
        // std::cout << "\n\n" << gap_index << "\n\n";
    }
    void sort_indicies()
    {
        std::sort(indicies.begin(), indicies.end());
    }
    void bulk_insert(std::vector<T> inserts)
    {
        if (gap > inserts.size())
        {

            // for (int i = 0; i < inserts.size(); i++)
            // {
            //     data[gap_index] = &inserts[i];
            //     indicies.push_back(gap_index++);
            // }
            std::memcpy(&(*(data.begin() + gap_index)), &(*(inserts.begin())), inserts.size() * sizeof(T));
            for (int i = 0; i < inserts.size(); i++)
                indicies.push_back(gap_index++);

            sort_indicies();
            gap = gap - inserts.size();
        }
        else
        {
            set_gap(inserts.size() + base_gap);
            bulk_insert(inserts);
        }
    }
};