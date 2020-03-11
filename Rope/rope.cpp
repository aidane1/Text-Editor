#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Definitions/defines.cpp"
#include "../Wrapping/wrapping.cpp"

class Rope
{

private:
    Rope *left;
    Rope *right;
    bool is_rope;
    Rope *parent;
    int sum;
    std::string string_data;

public:
    //initializer to initialize a rope from a string of unknown length
    Rope(bool is_rope, std::string data, Rope *parent_rope) : is_rope(is_rope)
    {

        parent = parent_rope;

        if (is_rope)
        {
            bool child_is_rope = data.length() / 2 >= 3;

            left = new Rope(child_is_rope, data.substr(0, data.length() / 2), this);
            right = new Rope(child_is_rope, data.substr(data.length() / 2, data.length()), this);

            sum = data.length() / 2;
        }
        else
        {
            string_data = data;
        }
    }
    //initializer to initialize a leaf from data
    Rope(std::string data, Rope *parent_rope)
    {
        is_rope = false;
        string_data = data;
        parent = parent_rope;
    }
    //initializer to split a leaf in to a node with two leaves
    Rope(std::string left_leaf, std::string right_leaf, Rope *parent_rope)
    {
        is_rope = true;
        left = new Rope(left_leaf, this);
        right = new Rope(right_leaf, this);
        parent = parent_rope;
        sum = left_leaf.length();
    }
    //initializer to create a rope from two smaller ropes
    Rope(Rope *left_child, Rope *right_child, Rope *parent_rope)
    {
        is_rope = true;
        parent = parent_rope;
        left = left_child;
        right = right_child;
        sum = left->total_weight();
    }
    ~Rope()
    {
        if (is_rope)
        {
            if (left)
                delete left;
            if (right)
                delete right;
        }
    }
    char index(int index)
    {
        if (!is_rope)
        {
            return string_data[index];
        }
        else if (index >= sum)
        {
            if (right != nullptr)
            {
                return right->index(index - sum);
            }
            return char();
        }
        else
        {
            if (left != nullptr)
            {
                return left->index(index);
            }
            return char();
        }
    }
    Rope *index_node(int index)
    {
        if (!is_rope)
        {
            return this;
        }
        else if (index >= sum)
        {
            if (right != nullptr)
            {
                return right->index_node(index - sum);
            }
            return nullptr;
        }
        else
        {
            if (left != nullptr)
            {
                return left->index_node(index);
            }
            return nullptr;
        }
    }
    int remainder_at_index(int index)
    {
        if (!is_rope)
        {
            return index;
        }
        else if (index >= sum)
        {
            if (right != nullptr)
            {
                return right->remainder_at_index(index - sum);
            }
            return 0;
        }
        else
        {
            if (left != nullptr)
            {
                return left->remainder_at_index(index);
            }
            return 0;
        }
    }
    void append_chars(int index, std::string characters)
    {
        Rope *node = index_node(index);
        int remainder = remainder_at_index(index);
        node->string_data = node->string_data.substr(0, remainder + 1) + characters + node->string_data.substr(remainder + 1, node->string_data.length() - remainder - 1);
        Rope *nodeCopy = node;
        while (nodeCopy->parent != nullptr)
        {
            if (nodeCopy->parent->left == nodeCopy)
            {
                nodeCopy->parent->sum += characters.length();
            }
            nodeCopy = nodeCopy->parent;
        }
    }
    Rope *split(int index)
    {
        // std::cout << "Index: " << index << "\n";
        Rope *node = index_node(index + 1);
        // std::cout << node->serialize_data() << "\n";
        int remainder = remainder_at_index(index + 1);
        // std::cout << "Remainder: " << remainder << "\n";
        if (remainder != 0)
        {
            Rope *nodeParent = node->parent;
            if (nodeParent->left == node)
            {
                nodeParent->left = new Rope(node->string_data.substr(0, remainder), node->string_data.substr(remainder, node->string_data.length() - remainder), nodeParent);
                node = nodeParent->left->right;
            }
            else
            {
                nodeParent->right = new Rope(node->string_data.substr(0, remainder), node->string_data.substr(remainder, node->string_data.length() - remainder), nodeParent);
                node = nodeParent->right->right;
            }
            remainder = 0;
        }
        // std::cout << node->serialize_data() << "\n";
        // std::cout << node->smallest_index_contained() << "\n";
        // std::cout << node->parent->right->smallest_index_contained() << "\n";
        // std::cout << node->parent->serialize_data() << "\n";

        Rope *cutTree = nullptr;

        while (node->parent != nullptr)
        {
            Rope *nodeParent = node->parent;
            if (node->parent->right != nullptr)
            {
                if (nodeParent->right->smallest_index_contained() > index)
                {
                    if (cutTree == nullptr)
                    {
                        cutTree = nodeParent->right;
                    }
                    else
                    {
                        if (nodeParent->left == node)
                        {
                            cutTree = Rope::concat(cutTree, nodeParent->right);
                        }
                        else
                        {
                            cutTree = Rope::concat(nodeParent->right, cutTree);
                        }
                    }
                    nodeParent->right = nullptr;
                }
            }
            node = nodeParent;
        }
        this->rebalance();
        return cutTree;
    }
    void append(int index, std::string data)
    {
    }
    static Rope *concat(Rope *s1, Rope *s2);
    static Rope *insert(Rope *s1, int index, std::string data);
    static Rope *delete_substring(Rope *s1, int index, int length);
    int smallest_index_contained()
    {
        if (this->parent == nullptr)
        {
            return 0;
        }
        if (this->parent->left == this)
        {
            return this->parent->smallest_index_contained();
        }
        else
        {
            return this->parent->sum + this->parent->smallest_index_contained();
        }
    }
    int rebalance()
    {
        if (this->is_rope)
        {
            if (this->left != nullptr)
            {
                sum = this->left->rebalance();
                if (this->right != nullptr)
                {
                    return sum + this->right->rebalance();
                }
                return sum;
            }
            if (this->right != nullptr)
            {
                return this->right->rebalance();
            }
            return 0;
        }
        else
        {
            return this->string_data.length();
        }
    }
    int tree_weight()
    {
        Rope *root = this;
        while (root->parent != nullptr)
        {
            root = root->parent;
        }
        return root->total_weight();
    }
    int total_weight()
    {
        if (is_rope)
        {
            if (right != nullptr)
            {
                return sum + right->total_weight();
            }
            return sum;
        }
        else
        {
            return string_data.length();
        }
    }
    std::string serialize_data()
    {
        if (is_rope)
        {
            if (left != nullptr && right != nullptr)
            {
                return left->serialize_data() + right->serialize_data();
            }
            else if (left != nullptr)
            {
                return left->serialize_data();
            }
            else if (right != nullptr)
            {
                return right->serialize_data();
            }
            return "";
        }
        else
        {
            return string_data;
        }
    }
    std::string serialize_display(sf::Font font)
    {
        return insert_text_breaks(this->serialize_data(), font);
    }
};
Rope *Rope::concat(Rope *s1, Rope *s2)
{
    Rope *newRope = new Rope(s1, s2, nullptr);
    s1->parent = newRope;
    s2->parent = newRope;
    newRope->rebalance();
    return newRope;
}
Rope *Rope::insert(Rope *s1, int index, std::string data)
{
    // s1->rebalance();
    Rope *new_rope;
    Rope *first_join;

    Rope *second_cut = s1->split(index);
    Rope *insert_rope;

    if (data.length() > 3)
    {
        insert_rope = new Rope(true, data, nullptr);
    }
    else
    {
        insert_rope = new Rope(data, nullptr);
    }
    first_join = Rope::concat(s1, insert_rope);
    new_rope = Rope::concat(first_join, second_cut);
    new_rope->rebalance();

    return new_rope;
}
Rope *Rope::delete_substring(Rope *s1, int index, int length)
{
    if ((index != 0) && (length != 0))
    {
        if (index != s1->serialize_data().length()-1)
        {
            Rope *second_cut = s1->split(index - 1);

            second_cut->rebalance();

            Rope *saved_section = second_cut->split(length - 1);

            Rope *new_rope = Rope::concat(s1, saved_section);

            return new_rope;
        }
        else
        {
            s1->split(index - 1);

            return s1;
        }
    }
    else
    {
        return nullptr;
    }
}