#include <iostream>

int main()
{
    std::cout << "Welcome to the physics simulator.\n";
    std::cout << "Please provide the starting parameters when prompted to do so.\n";
    
    float x0, v0, a;
    std::cout << "(1) Initial position (x): ";
    std::cin >> x0;

    std::cout << "(2) Initial velocity (x): ";
    std::cin >> v0;

    std::cout << "(3) Acceleration (constant, x): ";
    std::cin >> a;

    std::cout << "You entered:"
        << "\n\tx0:\t" << x0
        << "\n\tv0:\t" << v0
        << "\n\ta:\t" << a << '\n';
}