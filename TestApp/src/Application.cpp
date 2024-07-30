#include <iostream>
#include <Engine.h>

int main()
{
	std::cout << "Running the client test app..." << '\n';
	auto engine = new Engine();
	engine->Run();
	delete engine;
}