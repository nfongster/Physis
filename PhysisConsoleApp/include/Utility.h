#pragma once
#include <iostream>

template <typename T>
T GetValueFromPrompt(const char* prompt)
{
	T input{};
	std::cout << prompt;
	while (!(std::cin >> input))
	{
		std::cout << "Invalid input!\n";
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << prompt;
	}
	return input;
}