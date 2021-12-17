#include "RPN.h"

//default constructor for empty expression
RPN::RPN() : _postfix_notation(), _counted_result(0.0), _is_counted(false) {

}

//constructor for C-styled from infix to postfix reverse polish notation (only explicit casing)
RPN::RPN(const char* c_string) : _counted_result(0.0), _is_counted(false) {
	std::string string(c_string);
	if (isCorrect(string))
		_postfix_notation = getPostfixByInfix(string);
	else
		throw std::invalid_argument("Expression is incorrect.\n");
}

//constructor from infix to postfix reverse polish notation (only explicit casing)
RPN::RPN(const std::string& string) : _counted_result(0.0), _is_counted(false) {
	if (isCorrect(string))
		_postfix_notation = getPostfixByInfix(string);
	else
		throw std::invalid_argument("Expression is incorrect.\n");
}

//checking expression for mistakes (without saving error message)
bool RPN::isCorrect(const std::string& string) {
	std::ostringstream buffer_for_err_text;

	return isCorrect(string, buffer_for_err_text);
}

//checking expression for mistakes (with saving error message)
bool RPN::isCorrect(const std::string& string, std::ostream& error_output) {

	if (isLexicallyCorrect(string, error_output) && isSemanticallyCorrect(string, error_output))
		return true;

	return false;
}

/*
_postfix_notation contains:
'p' - pi
'e' - exp
'_' - -
's' - sin
'c' - cos
't' - tg
'g' - ctg
'o' - log
'l' - ln
'q' - sqrt
'a' - abs
*/

//counting result of expression
double RPN::getResult() const {

	//if the result is not counted and string is not empty, run the code
	if (!_is_counted && _postfix_notation.size() != 0) { 
		_is_counted = true;

		Stack<double> stack;
		size_t n = _postfix_notation.size();

		for (size_t i = 0; i < n; i++)
			if (isDigit(_postfix_notation[i]) || _postfix_notation[i] == '.') { //if temp symbol is digit
				double number = 0;
				for (; i < n && isDigit(_postfix_notation[i]); i++) //collect integer part of number (numenator)
					number = number * 10 + (_postfix_notation[i] - '0');

				if (i < n && _postfix_notation[i] == '.') { //if temp is point
					int divisor = 1;
					for (i++; i < n && isDigit(_postfix_notation[i]); i++) { //collect numerator and denominator
						number = number * 10 + (_postfix_notation[i] - '0');
						divisor *= 10;
					}
					number /= divisor;	//get the number
				}

				if (i < n && !_postfix_notation[i] == '.')	//
					i--;		//cursor to last digit ??

				stack.push(number); //push number into stack
			}
			else if (_postfix_notation[i] == 'e') //if temp symbol is 'e'
				stack.push(exp(1));					//push exponent into stack
			else if (_postfix_notation[i] == 'p') //if temp symbol is 'e'
				stack.push(M_PI);					 //push pi into stack
			else if (_postfix_notation[i] == 's' || _postfix_notation[i] == 'c'
				|| _postfix_notation[i] == 't' || _postfix_notation[i] == 'g'
				|| _postfix_notation[i] == 'o' || _postfix_notation[i] == 'l'
				|| _postfix_notation[i] == 'q' || _postfix_notation[i] == 'a'
				|| _postfix_notation[i] == '_') {	//if it's unary operation

				double x = stack.pop();	//get the last number from stack

				switch (_postfix_notation[i]) { //and push the result of operation with x to stack
				case 's':
					stack.push(sin(x));
					break;
				case 'c':
					stack.push(cos(x));
					break;
				case 't':
					stack.push(tan(x));
					break;
				case 'g':
					stack.push(1 / tan(x));
					break;
				case 'o':
					stack.push(log10(x));
					break;
				case 'l':
					stack.push(log(x));
					break;
				case 'q':
					stack.push(sqrt(x));
					break;
				case 'a':
					stack.push(abs(x));
					break;
				case '_':
					stack.push(-x);
					break;
				}
			}
			else if (_postfix_notation[i] == '+' || _postfix_notation[i] == '-'
				|| _postfix_notation[i] == '*' || _postfix_notation[i] == '/'
				|| _postfix_notation[i] == '^') {	//if it's binary operation

				double b = stack.pop();	//get the last two numbers from stack
				double a = stack.pop();

				switch (_postfix_notation[i]) { //and push the result of operation with a and b to stack
				case'+':
					stack.push(a + b);
					break;
				case '-':
					stack.push(a - b);
					break;
				case '*':
					stack.push(a * b);
					break;
				case '/':
					stack.push(a / b);
					break;
				case '^':
					stack.push(pow(a, b));
					break;
				}
			}

		_counted_result = stack.pop();	//when actions is over, the result of the expression is on stack top
	}

	return _counted_result;	//return _counted_result
}

//print postfix form of expression
std::string RPN::getPostfixForm() const {
	return _postfix_notation;
}