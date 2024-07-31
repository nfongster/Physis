#include <iostream>
#include "Engine.h"

Engine::Engine() : m_system(new SystemState())
{
}

Engine::~Engine()
{
	delete m_system;
}

void Engine::Run()
{
	std::cout << "Welcome to the Physis (f-EYE-sis) Engine." << '\n';
	auto particle = m_system->GetParticle();
	std::cout << "x:\t" << particle->GetPosition().X << '\n';
	std::cout << "y:\t" << particle->GetPosition().Y << '\n';
	std::cout << "vx:\t" << particle->GetVelocity().X << '\n';
	std::cout << "vy:\t" << particle->GetVelocity().Y << '\n';
	std::cout << "ax:\t" << particle->GetAcceleration().X << '\n';
	std::cout << "ay:\t" << particle->GetAcceleration().Y << '\n';
}
