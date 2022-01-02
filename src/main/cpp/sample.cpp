// Include standard headers
#include <iostream>
#include <stdexcept>

inline int max(int a, int b)
{
    return a < b ? b : a;
}

int main(void) {


    std::cout << "It works.. " << std::endl;

    std::cout << max(1, 2) << std::endl;
    std::cout << max(3, 2) << std::endl;

    return 0;
}
