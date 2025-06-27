#include "bigint.hpp" // or appropriate header
#include <chrono>
#include <iostream>

int main()
{

	std::string s1;
	std::string s2;
	std::cout << "Enter string s1: ";
	std::cin >> s1;
	std::cout << "Enter string s2: ";
	std::cin >> s2;
	BigInt i1(s1);
	BigInt i2(s2);
	std::cout << "Multiplication: " << i1 * i2 << std::endl;
	std::cout << "Addition: " << i1 + i2 << std::endl;
	std::cout << "Subtraction: " << i1 - i2 << std::endl;
	std::cout << "Divison: " << i1 / i2 << std::endl;
	std::cout << "Modulo: " << i1 % i2 << std::endl;
}