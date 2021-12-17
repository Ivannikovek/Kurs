#include "pch.h"
#include "CppUnitTest.h"

#include "../../CourseWork/CourseWork/RPN.h"
#include "../../CourseWork/CourseWork/RPN_private_methods.cpp"
#include "../../CourseWork/CourseWork/RPN_public_methods.cpp"

#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestBoss {

	//std::vector of wrong expressions:
	std::vector<std::string> wrong_test = {
		"sqrt(3",			//closing bracket is absent
		"sqrt(3 + 5 ))",	//extra bracket
		")sqrt(3) + (5)) ", //mismatch of brackets positions
		"6 div 8",			//unknown command
		"6 8",				//missed operator
		"6 + 8 (5)",		//missed operator
		"6 sin(pi)",		//missed operator
		"6 + - 8",			//extra operator
		"3..5",				//extra point
		"3.pi",				//extra operand
		"e12",				//incorrect command
		"sin pi",			//missed bracket for operand
	};
	
	//structure for saving infix expression, postfix and result of counting
	struct solved_expression {
		std::string infix;
		std::string postfix;
		double result;
	};

	//std::vector of right expressions and results:
	std::vector<solved_expression> correct_test = {
		//infix							//postfix							//result
		{"pi",							"p ",								M_PI},
		{"e",							"e ",								exp(1)},
		{"2",							"2 ",								2},
		{"9.",							"9. ",								9.},
		{".1",							".1 ",								.1},
		{"0.333",						"0.333 ",							0.333},
		{"    1234.5  ",				"1234.5 ",							1234.5},
		{" -  pi",						"p _ ",								-M_PI},
		{" -  e",						"e _ ",								-exp(1)},
		{"-3.003",						"3.003 _ ",							-3.003},
		{"pi + pi",						"p p + ",							M_PI + M_PI},
		{"pi+pi",						"p p + ",							M_PI + M_PI},
		{"2.+.98",						"2. .98 + ",						2.98},
		{"2.0+0.98",					"2.0 0.98 + ",						2.98},
		{"2*pi/3",						"2 p * 3 / ",						2 * M_PI / 3},
		{"2^3 + 2^2 + 2^1 + 2^ 0",		"2 3 ^ 2 2 ^ + 2 1 ^ + 2 0 ^ + ",	8 + 4 + 2 + 1},
		{"pi - 3.14",					"p 3.14 - ",						M_PI - 3.14},	
		{"cos(pi/2)",					"p 2 / c ",							cos(M_PI / 2)},
		{"sin(pi/2)",					"p 2 / s ",							sin(M_PI / 2)},
		{"cos(100)^2 + sin(100)^2",		"100 c 2 ^ 100 s 2 ^ + ",			pow(cos(100), 2) + pow(sin(100), 2)},
		{"sqrt(3^2)",					"3 2 ^ q ",							sqrt(pow(3, 2))},
		{"sqrt(100)",					"100 q ",							sqrt(100)},
		{"log(100)",					"100 o ",							log10(100)},
		{"ln(e)",						"e l ",								log(exp(1))},
		{"tg(pi/4)",					"p 4 / t ",							tan(M_PI / 4)},
		{"ctg(-pi/4)",					"p _ 4 / g ",						1 / (tan(-M_PI / 4))},
		{"tg(100)*ctg(100)",			"100 t 100 g * ",					tan(100) / tan(100)},
		{"abs(pi - 3.14)",				"p 3.14 - a ",						fabs(M_PI - 3.14)},
		{"abs(e - 2.7)",				"e 2.7 - a ",						fabs(exp(1) - 2.7)},
		{"abs(-123)",					"123 _ a ",							fabs(-123)},
	};

	//RPN - Reverse Polish Notation
	TEST_CLASS(UnitTestBoss) {
		TEST_METHOD(RPN_defaultConstructor) {
			bool isCatched = false;

			for (size_t i = 0u; i < 100u; i++)
				try {
				RPN empty_object;
			}
			catch (std::exception e) {
				isCatched = true;
			}

			//every empty object is created without exception:
			Assert::IsFalse(isCatched);
		}

		TEST_METHOD(RPN_constructorByCstyledString_wrongCases) {
			for (size_t i = 0u; i < wrong_test.size(); i++) {
				//functor capture 'i' for creating objects by test[i]:
				auto functor = [i] { RPN object(wrong_test[i].data()); }; //get C-styled string for creating object

				//expected std::invalid_argument exception in each test:
				Assert::ExpectException<std::invalid_argument>(functor);
			}
		}

		TEST_METHOD(RPN_constructorByCstyledString_rightCases) {
			size_t counter = 0;

			for (size_t i = 0u; i < correct_test.size(); i++) {
				try {
					RPN object(correct_test[i].infix.data()); //get C-styled string for creating object
				}
				catch (std::exception) {
					counter++;
				}
			}

			//expected zero catched exceptions from correct test objects:
			Assert::AreEqual(counter, 0u);
		}

		TEST_METHOD(RPN_constructorByString_wrongCases) {
			for (size_t i = 0u; i < wrong_test.size(); i++) {
				//functor capture 'i' for creating objects by test[i]:
				auto functor = [i] { RPN object(wrong_test[i]); };

				//expected std::invalid_argument exception in each test:
				Assert::ExpectException<std::invalid_argument>(functor);
			}
		}

		TEST_METHOD(RPN_constructorByString_rightCases) {
			size_t counter = 0;

			for (size_t i = 0u; i < correct_test.size(); i++) {
				try {
					RPN object(correct_test[i].infix);
				}
				catch (std::exception) {
					counter++;
				}
			}

			//expected zero catched exceptions from correct test objects:
			Assert::AreEqual(counter, 0u);
		}

		TEST_METHOD(RPN_isCorrect) {

			//expected succsesfull checking all correct tests:
			for (size_t i = 0u; i < correct_test.size(); i++)
				Assert::IsTrue(RPN::isCorrect(correct_test[i].infix));

			//expected fail checking all wrong tests:
			for (size_t i = 0u; i < wrong_test.size(); i++)
				Assert::IsFalse(RPN::isCorrect(wrong_test[i]));
		}

		TEST_METHOD(RPN_isCorrectWithMessages)
		{
			//expected succsesfull checking all correct tests:
			for (size_t i = 0u; i < correct_test.size(); i++)
				Assert::IsTrue(RPN::isCorrect(correct_test[i].infix));

			//expected fail checking all wrong tests:
			for (size_t i = 0u; i < wrong_test.size(); i++)
				Assert::IsFalse(RPN::isCorrect(wrong_test[i]));

			//messages of positions are not checking because of difficulty
			//it is printing in 'main'
		}

		TEST_METHOD(RPN_getPostfixForm)
		{
			//expected equality each postfix form to saved for test
			for (size_t i = 0u; i < correct_test.size(); i++)
				Assert::AreEqual(RPN(correct_test[i].infix).getPostfixForm(), correct_test[i].postfix);
		}

		TEST_METHOD(RPN_getResult)
		{
			//expected equality each value to saved for test
			for (size_t i = 0u; i < correct_test.size(); i++)
				Assert::AreEqual(RPN(correct_test[i].infix).getResult(), correct_test[i].result);
		}
	};
}
