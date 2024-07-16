#include <iostream>
#include "SystemConfig.h"
#include "System.h"

int main()
{
    std::cout << "Welcome to the physics simulator.\n";
    std::cout << "Please provide the starting parameters when prompted to do so.\n";
    
    float x0, v0, a, t;
    std::cout << "(1) Initial position (x):\t";
    std::cin >> x0;

    std::cout << "(2) Initial velocity (x):\t";
    std::cin >> v0;

    std::cout << "(3) Acceleration (constant, x):\t";
    std::cin >> a;

    std::cout << "(5) Time (s):\t\t\t";
    std::cin >> t;

    std::cout << "\nYou entered:"
        << "\n\tx0:\t" << x0
        << "\n\tv0:\t" << v0
        << "\n\ta:\t" << a
        << "\n\tt:\t" << t << '\n';

    SystemConfig config = SystemConfig(x0, v0, a);
    System system = System(config);

    float xf = system.step_x(t);
    float vf = system.step_v(t);
    float dx = xf - x0;
    float dv = vf - v0;
    float vAvg = dv / t;

    std::cout << "\nFinal system parameters:"
        << "\n\txf:\t" << xf
        << "\n\tvf:\t" << vf
        << "\n\tdx:\t" << dx
        << "\n\tdv:\t" << dv
        << "\n\tvAvg:\t" << vAvg << '\n';
}