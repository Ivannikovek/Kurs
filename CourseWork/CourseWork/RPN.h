#ifndef _RPN_H_

#define _RPN_H_
#include <iostream>
#include <string>
#include <sstream>

#include "Stack.hpp"

#include <cmath>
#include <stdexcept>

//for ssize from -1 to MAX of size_t
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//RPN - Reverse Polish Notation (for shortness)
class RPN {
public:
	//default constructor
	RPN();

	//constructor for C-styled from infix to postfix (only explicit casting)
	explicit RPN(const char * c_string);

	//constructor from infix to postfix (only explicit casting)
	explicit RPN(const std::string& string);

	//counting result of expression
	double getResult() const;

	//checking expression for mistakes (without saving error message)
	static bool isCorrect(const std::string& string);

	//checking expression for mistakes (with saving error message)
	static bool isCorrect(const std::string& string, std::ostream& error_output);

	//print postfix form of expression
	std::string getPostfixForm() const;

private:
	std::string _postfix_notation;		//string for postfix notation expression
	mutable double _counted_result;	//result of the expression (mutable also in const method "count")
	mutable bool _is_counted;		//was double result counted (mutable also in const method "count")

	//transforming correct infix expression to postfix notation
	static std::string getPostfixByInfix(const std::string& str);

	//checking expression for incorrect commands and symbols
	static bool isLexicallyCorrect(const std::string& str, std::ostream& err);

	//checking expression for logical mistakes
	static bool isSemanticallyCorrect(const std::string& str, std::ostream& err);

	//getting priority of operation
	static int priority(char operation);

	//get first left character which is differnt from 'space' (from index)
	static ssize_t getIndexOfFirstLeftNonspaceCharacter(const std::string& str, size_t index);

	//get first right character which is differnt from 'space' (from index)
	static size_t getIndexOfFirstRightNonspaceCharacter(const std::string& str, size_t index);

	//printing string with show position of mistake
	static void printErrorPosition(const std::string& str, size_t number, std::ostream& err);

	//deleting spaces from string
	static void eraseSpaces(std::string& str);

	//is symbol punctuation sign
	static bool isPunctuationSign(char symbol);

	//is symbol digit
	static bool isDigit(char symbol);
};

#endif //! _RPN_H_

