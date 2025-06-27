#define MAX_PRECISION 100
#include "bigfloat.hpp"
#include <iostream>


int main()
{


		std::string s1("63.234231243");
		std::string s2("1.23");
		//std::cout << "Enter string s1: ";
		//std::cin >> s1;
		//std::cout << "Enter string s2: ";
		//std::cin >> s2;
		BigFloat i1(s1);
		BigFloat i2(s2);
		std::cout << "Multiplication: " << i1 * i2 << std::endl;
		std::cout << "Addition: " << i1 + i2 << std::endl;
		std::cout << "Subtraction: " << i1 - i2 << std::endl;
		std::cout << "Divison: " << i1 / i2 << std::endl;
		//std::cout << "Modulo: " << i1 % i2 << std::endl;
		std::cout << "\nPress Enter to exit...";
		std::cin.get();  // Wait for Enter
		return 0;
}
