#include <iostream>
#include <thread>
#include "../include/ParticleConfig.h"
#include "../include/Particle.h"
#include "../include/ParticleSystem.h"

int main()
{
    std::cout << "Welcome to the physics simulator.\n";
    std::cout << "Please provide the starting parameters when prompted to do so.\n";
    
    float x0, v0, a0, t_total, dt;
    std::cout << "(1) Initial position (x):\t";
    std::cin >> x0;

    std::cout << "(2) Initial velocity (x):\t";
    std::cin >> v0;

    std::cout << "(3) Acceleration (constant, x):\t";
    std::cin >> a0;

    std::cout << "(5) Total Time (s):\t\t";
    std::cin >> t_total;

    std::cout << "(6) Time Step (s):\t\t";
    std::cin >> dt;

    std::cout << "\nYou entered:"
        << "\n\tx0:\t" << x0
        << "\n\tv0:\t" << v0
        << "\n\ta0:\t" << a0
        << "\n\tt:\t"  << t_total
        << "\n\tdt:\t" << dt << '\n';

    ParticleConfig config = ParticleConfig(x0, v0, a0);
    ParticleSystem* system = new ParticleSystem(config, t_total, dt);

    std::cout << "\nExecuting particle system on a separate thread...\n";
    std::thread worker([&]() 
                { 
                    system->Execute();
                });
    worker.join();
    std::cout << "Execution complete.\n";

    std::cout << "\nFinal system parameters:"
        << "\n\txf:\t" << system->get_x()
        << "\n\tvf:\t" << system->get_v()
        << "\n\tdx:\t" << system->displacement(x0)
        << "\n\tdv:\t" << system->delta_v(v0)
        << "\n\tvAvg:\t" << system->average_v(x0) << '\n';

    delete system;
}