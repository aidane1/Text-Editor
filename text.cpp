#include <iostream>

void test(std::string &data) {
    std::cout << "test function number 1 data: " << data << "\n";
    (data)[0] = 'B';
    std::cout << "test function number 2 data: " << data << "\n";
}

int main() {
    // std::string my_string = "Hello!";
    // std::cout << "Main function number 1 data: " << my_string << "\n";
    // test(my_string);
    // std::cout << "Main function number 2 data: " << my_string << "\n";
    std::string my_string = "Hello!";
    std::string my_string_2 = my_string;
    std::cout << "1: " << my_string << "\n";
    std::cout << "2: " << my_string_2 << "\n";
    my_string[0] = 'B';
    std::cout << "3: " << my_string << "\n";
    std::cout << "4: " << my_string_2 << "\n";
    return 0;
}