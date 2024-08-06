#include "Engine.h"

Engine::Engine() : m_system(new ParticleSystem())
{
}

Engine::~Engine()
{
	delete m_system;
}

void Engine::Run()
{
	std::cout << "Welcome to the Physis (f-EYE-sis) Engine." << '\n';
	auto particle = m_system->GetParticles();

	/*std::cout << "rx\try\tvx\tvy\tax\tay\n";
	particle->PrintState();*/
}
