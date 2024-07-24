#include <iostream>
#include <string>
#include <thread>
#include "ParticleConfig.h"
#include "Particle.h"
#include "ParticleSystem.h"

int main()
{
    std::cout << "Welcome to the physics simulator.\n";
    std::cout << "Please provide the starting parameters for the system:\n";
    
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

    ParticleConfig config = ParticleConfig(x0, v0, a0);
    ParticleSystem* system = new ParticleSystem(config, t_total, dt);
    float* px = system->x_buffer;
    float* pv = system->v_buffer;

    // std::cout << "\nExecuting particle system on a separate thread...\n";
    // std::thread worker([&]() 
    //             { 
    //                 system->execute();
    //             });
    // worker.join();
    // std::cout << "Execution complete.\n";

    std::cout << "\nBeginning system execution...\n";
    while (system->is_running())
    {
        system->step(dt);
    }

    std::cout << "\nFinal system parameters:"
        << "\n\txf:\t" << system->get_x()
        << "\n\tvf:\t" << system->get_v()
        << "\n\tdx:\t" << system->displacement(x0)
        << "\n\tdv:\t" << system->delta_v(v0)
        << "\n\tvAvg:\t" << system->average_v(x0) << '\n';

    std::cout << "\nSystem log (t, x, v):\n\n";
    for (float t = 0; t < t_total; t += dt)
    {
        std::cout << "\t" << t << "\t" << *px << "\t" << *pv << "\n";
        px++, pv++;
    }
    delete system;
}