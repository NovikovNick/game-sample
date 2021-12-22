// Include standard headers
#include <iostream>
#include <stdexcept>

class A {
    
};

void foo () {
    throw std::runtime_error("The issue");
}

void boo () {
    foo();
}

int main(void) {


    std::cout << "It works.. " << std::endl;

    try {
        boo();

    } catch (std::runtime_error ex) {

        std::cout << ex.what() << std::endl;
    }

    return 0;
}
