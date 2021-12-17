#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "RPN.h"

int main() {
	char c;
	std::string str;

	size_t N;

	std::cout << "Enter count of expressions:" << std::endl;
	std::cin >> N;
	std::cin.get(); //read extra '\n'
	std::cout << std::endl;

	for (size_t i = 0; i < N; i++) {
		std::cout << "Enter infix expression:" << "\t";
		std::getline(std::cin, str);

		std::ostringstream error;

		std::cout << "Expression is correct:" << "\t";
		if (RPN::isCorrect(str, error)) {
			std::cout << "YES" << std::endl;
			std::cout << "Postfix form:" << "\t\t";
			std::cout << RPN(str).getPostfixForm() << std::endl;
			std::cout << "Result:" << "\t\t\t";
			std::cout << RPN(str).getResult() << std::endl;
		}
		else {
			std::cout << "NO" << std::endl;
			std::cout << error.str();
		}
		std::cout << std::endl;
	}

	return 0;
}
