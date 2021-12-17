#include "RPN.h"

//getting priority of operation
int RPN::priority(char operation) {
	if (operation == '^')
		return 2;
	else if (operation == '*' || operation == '/')
		return 1;
	else if (operation == '+' || operation == '-')
		return 0;
	return -1;
}

//get first left character which is differnt from 'space' (from index)
ssize_t RPN::getIndexOfFirstLeftNonspaceCharacter(const std::string& str, size_t index) {
	ssize_t i;
	for (i = index - 1; i >= 0 && str[i] == ' '; i--);
	return i;
}

//get first right character which is differnt from 'space' (from index)
size_t RPN::getIndexOfFirstRightNonspaceCharacter(const std::string& str, size_t index) {
	size_t i;
	size_t n = str.size();
	for (i = index + 1; i <= n && str[i] == ' '; i++);
	return i;
}

//printing string with show position of mistake
void RPN::printErrorPosition(const std::string& str, size_t number, std::ostream& err) {
	size_t index = number - 1;
	size_t n = str.size();

	err << str;			//print string with error
	err << std::endl;

	size_t i;
	for (i = 0; i < index; i++) //print spaces before the arrow to the index
		err << ' ';
	err << '^';

	err << std::endl;
}

//deleting spaces from string
void RPN::eraseSpaces(std::string& str) {
	for (size_t i = 0; i < str.size(); i++)
		if (str[i] == ' ') {
			str.erase(str.begin() + i);
			i--;
		}
}

//is symbol punctuation sign
bool RPN::isPunctuationSign(char symbol) {
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^'
		|| symbol == '(' || symbol == ')' || symbol == ' ')
		return true;

	return false;
}

//is symbol digit
bool RPN::isDigit(char symbol) {
	if ('0' <= symbol && symbol <= '9')
		return true;

	return false;
}

//checking expression for incorrect commands and symbols
bool RPN::isLexicallyCorrect(const std::string& str, std::ostream& err) {
	size_t n = str.size();

	for (size_t i = 0; i < n; i++) //check temp symbol by index i and maybe shift i to the end of sequence
		if (!isDigit(str[i]) && str[i] != '.' && !isPunctuationSign(str[i])) //if temp symbol is not correct single symbol
			if (str[i] == 'e' || str[i] == 'p') { //if it's letter of identifier 'e' or 'pi'

				if (str[i] == 'p')
					i++; //get next index after 'p'

				if ((str[i] == 'p' && (i == n || str[i] != 'i')) || //if pi is not complete OR
					i + 1 != n && !isPunctuationSign(str[i + 1])) {		//if identidier 'e' or 'pi' is not ended
					err << "Incorrect command in position " << (i + 1) + 1 << ":" << std::endl;
					printErrorPosition(str, (i + 1) + 1, err);
					return false;
				}
			}
			else { //checking other identifiers for functions
				bool isWrongCommand = false;
				switch (str[i]) {

					//is it sin / sqrt
				case 's':
					if (i + 2 >= n)		//check lenght
						isWrongCommand = true;
					else if (str[i + 1] == 'i' && str[i + 2] == 'n')	//check sin
						i += 2;
					else
						if (i + 3 < n && str[i + 1] == 'q' && str[i + 2] == 'r' && str[i + 3] == 't')	//check sqrt
							i += 3;
						else isWrongCommand = true;
					break;

					//is it cos / ctg
				case 'c':
					if (i + 2 >= n)		//check lenght
						isWrongCommand = true;
					else if (str[i + 1] == 'o' && str[i + 2] == 's'
						|| str[i + 1] == 't' && str[i + 2] == 'g')		//check cos / ctg
						i += 2;
					else isWrongCommand = true;
					break;

					//is it tg
				case 't':
					if (i + 1 >= n)		//check lenght
						isWrongCommand = true;
					else if (str[i + 1] == 'g')
						i += 1;
					else isWrongCommand = true;
					break;

					//is it log / ln
				case 'l':
					if (i + 1 >= n)		//check lenght
						isWrongCommand = true;
					else if (str[i + 1] == 'n') 	//check ln
						i += 1;
					else if (i + 2 < n && str[i + 1] == 'o' && str[i + 2] == 'g') 	//check log
						i += 2;
					else isWrongCommand = true;
					break;

					//is it abs
				case 'a':
					if (i + 2 >= n)		//check lenght
						isWrongCommand = true;
					else if (str[i + 1] == 'b' && str[i + 2] == 's')
						i += 2;
					else isWrongCommand = true;
					break;

					//it's something wrong
				default:
					err << "Incorrect symbol in position " << i + 1 << ":" << std::endl;
					printErrorPosition(str, i + 1, err);
					return false;
				}

				if (isWrongCommand || !(i + 1 == n || str[i + 1] == ' ' ||
					str[i] < 'A' || str[i] > 'Z' ||
					str[i] < 'a' || str[i] > 'z')) { //if symbols are possible, but function isn't correct
					err << "Incorrect command in position " << i + 1 << std::endl;
					printErrorPosition(str, i + 1, err);
					return false;
				}
			}
	return true;
}

//checking expression for logical mistakes
bool RPN::isSemanticallyCorrect(const std::string& str, std::ostream& err) {
	Stack<size_t> stack;	//stack for checking brackets
	size_t n = str.size();

	for (size_t i = 0; i < n; i++)

		//if its number in any form
		if (str[i] == '.' || isDigit(str[i]) || str[i] == 'p' || str[i] == 'e') {

			//checking element from the left
			ssize_t indexLeft = getIndexOfFirstLeftNonspaceCharacter(str, i);

			if (indexLeft >= 0)		//if left element is exist
				if (str[indexLeft] == ')') {	//if it's closing bracket, then error
					err << "Missed operator in position " << indexLeft + 2 << ":" << std::endl;
					printErrorPosition(str, indexLeft + 2, err);
					return false;
				}
				else if (isDigit(str[indexLeft]) || str[indexLeft] == '.'
					|| str[indexLeft] == 'i' || str[indexLeft] == 'e') { 	//if it's other number, then error
					err << "Extra operand in position " << i + 1 << ":" << std::endl;
					printErrorPosition(str, i + 1, err);
					return false;
				}

			//if number is by digit
			if (isDigit(str[i]) || str[i] == '.') {
				size_t countOfDigits = 0;

				for (; i < n && isDigit(str[i]); i++) //iterate all digits before point or another symbol
					countOfDigits++;

				//if string is not over
				if (i != n)
					if (str[i] == '.') { //if cicle stopped on point
						i++; //skip point

						if (i < n && (str[i] == '.')) { //if number have second point
							err << "Extra point in position " << i + 1 << ":" << std::endl;
							printErrorPosition(str, i + 1, err);
							return false;
						}

						for (; i < n && isDigit(str[i]); i++) //iterate all digits after point
							countOfDigits++;

						if (countOfDigits == 0) { //if no one digit is met
							err << "Incorrect number in position " << (i - 1) + 1 << ":" << std::endl;
							printErrorPosition(str, (i - 1) + 1, err);
							return false;
						}
					}

				//i--;		//stay on symbol after number
			}
			else if (str[i] == 'e')	//if it's 'e' skip one symbol
				i += 1;
			else if (str[i] == 'p')	//if it's 'pi' skip two symbols
				i += 2;

			size_t indexRight = (i == n ? n : getIndexOfFirstRightNonspaceCharacter(str, i - 1)); //searsh right notspace from the last symbol of number  

			if (indexRight < n && str[indexRight] == '(') {	//if right element is bracket, then error
				err << "Missed operator in position " << i + 1 << ":" << std::endl;
				printErrorPosition(str, i + 1, err);
				return false;
			}

			i--;	//stay on last number symbol 

		}	//!endif its number in any form

		//if openning bracket
		else if (str[i] == '(') //openning bracket is pushing to stack
			stack.push(i);

	//if closing bracket
		else if (str[i] == ')') //closing bracket pop the last open bracket if it's exist
			if (!stack.is_empty())
				stack.pop();
			else {				//or error
				err << "Missing opening bracket for closing in position " << i + 1 << std::endl;
				printErrorPosition(str, i + 1, err);
				return false;
			}

	//if symbol is operation
		else if (str[i] == '+' || str[i] == '*' || str[i] == '/' || str[i] == '-') {

			ssize_t indexLeft = getIndexOfFirstLeftNonspaceCharacter(str, i); //get left element

			if (indexLeft < 0) //if left operand is absent
				if (str[i] == '-')	//and sign is unary minus
					continue;		//it's okey, continue
				else {
					err << "Missed operand before operator in position " << i + 1 << ":" << std::endl;
					printErrorPosition(str, i + 1, err);
					return false;
				}

			if (!(str[indexLeft] == '.' || isDigit(str[indexLeft]) ||
				str[indexLeft] == 'i' || str[indexLeft] == 'e' ||
				str[indexLeft] == ')' ||
				(str[i] == '-' && str[indexLeft] == '('))) { //if NOT: left operand is number or constant or closing bracket
															//it's unary minus and left operand is openning bracket
				err << "Missed operand before operator in position " << i + 1 << ":" << std::endl;
				printErrorPosition(str, i + 1, err);
				return false;
			}

			//if left element is okey
			size_t indexRight = getIndexOfFirstRightNonspaceCharacter(str, i);	//check right

			if (indexRight >= n ||
				!(str[indexRight] == '.' || isDigit(str[indexRight]) ||
					str[indexRight] == '(' ||
					str[indexRight] == 's' || str[indexRight] == 'c' || str[indexRight] == 't'
					|| str[indexRight] == 'l' || str[indexRight] == 'e'
					|| str[indexRight] == 'p' || str[indexRight] == 'a')) { //if right elment is absent or it's not number / constant / 
																			//function / openning bracket
				err << "Missed operand in position " << (i + 1) + 1<< ":" << std::endl;
				printErrorPosition(str, (i + 1) + 1, err);
				return false;
			}

		} //! endif symbol is operation

		//if symbol is a function
		else if (str[i] == 's' || str[i] == 'c' || str[i] == 't'
			|| str[i] == 'l' || str[i] == 'a') {

			ssize_t indexLeft = getIndexOfFirstLeftNonspaceCharacter(str, i); //get left element

			//if left is not absent
			if (indexLeft >= 0 && !(str[indexLeft] == '(' || str[indexLeft] == '^' ||
				str[indexLeft] == '+' || str[indexLeft] == '-' ||
				str[indexLeft] == '*' || str[indexLeft] == '/')) { //if left element is not operation or oppening bracket

				err << "Missed operand before function in position " << i + 1 << ":" << std::endl;
				printErrorPosition(str, i + 1, err);
				return false;
			}

			if (str[i] == 't' || str[i] == 'l' && str[i + 1] == 'n') //skip symbols to the end of any function
				i += 1;
			else if (str[i] == 's' && str[i + 1] == 'q')
				i += 3;
			else
				i += 2;

			size_t indexRight = getIndexOfFirstRightNonspaceCharacter(str, i); //get right element

			if (indexRight >= n || str[indexRight] != '(') { //if right element is absent or not opening bracket
				err << "Missed function bracket for operand in position " << (i + 1) + 1 << ":" << std::endl;
				printErrorPosition(str, (i + 1) + 1, err);
				return false;
			}
		}

	if (!stack.is_empty()) {	//if stack is not empty, there were extra brackets
		err << "Missed closing bracket for bracket in position " << stack.top() + 1 << ":" << std::endl;
		printErrorPosition(str, stack.top() + 1, err);
		return false;
	}

	return true;
}

/*
Stack contains:
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

//transforming correct infix expression to postfix notation
std::string RPN::getPostfixByInfix(const std::string& string) {
	std::string infix_str = string;

	eraseSpaces(infix_str); //delete all spaces

	std::string postfix_str;

	Stack<char> stack; //stack for operations

	size_t n = infix_str.size();
	for (size_t i = 0; i < n; i++) {

		//if it's number
		if (isDigit(infix_str[i]) || infix_str[i] == '.') {
			//push it to string
			for (i; isDigit(infix_str[i]) || infix_str[i] == '.'; i++)
				postfix_str.push_back(infix_str[i]);
			postfix_str.push_back(' ');

			i -= 1;		//cursor to the last digit / point
		}

		//if it's exponent
		else if (infix_str[i] == 'e') {
			postfix_str.push_back('e'); //push it to string
			postfix_str.push_back(' ');
		}

		//if it's 'pi'
		else if (infix_str[i] == 'p') {
			postfix_str.push_back('p'); //push it to string
			postfix_str.push_back(' ');

			i += 1;		//skip 'i'
		}

		//if it's unary minus
		else if (infix_str[i] == '-' && (i == 0 || infix_str[i - 1] == '('))
			stack.push('_'); //push it to STACK

		//if it's sin or sqrt
		else if (infix_str[i] == 's') {
			if (infix_str[i + 1] == 'i') {
				stack.push('s'); //push it to STACK
				i += 2;
			}
			else if (infix_str[i + 1] == 'q') {
				stack.push('q');  //push it to STACK
				i += 3;
			}
		}

		//if it's cos or ctg
		else if (infix_str[i] == 'c') {
			if (infix_str[i + 1] == 'o') {
				stack.push('c'); //push it to STACK
				i += 2;
			}
			else if (infix_str[i + 1] == 't') {
				stack.push('g'); //push it to STACK
				i += 2;
			}
		}

		//if it's tg
		else if (infix_str[i] == 't') {
			stack.push('t'); //push it to STACK
			i += 1;		//skip 'g'
		}

		//if it's log or ln
		else if (infix_str[i] == 'l') {
			if (infix_str[i + 1] == 'o') {
				stack.push('o'); //push it to STACK
				i += 2;
			}
			else if (infix_str[i + 1] == 'n') {
				stack.push('l'); //push it to STACK
				i += 1;
			}
		}

		//if it's abs
		else if (infix_str[i] == 'a') {
			stack.push('a'); //push it to STACK
			i += 2;
		}

		//if it's openning bracket
		else if (infix_str[i] == '(')
			stack.push('('); //push it to STACK

		//if it's closing bracket
		else if (infix_str[i] == ')') {
			while (stack.top() != '(') {
				postfix_str.push_back(stack.pop());  //push all before openning bracket into string
				postfix_str.push_back(' ');
			}

			stack.pop(); //pop bracket
		}

		//if it's operation 
		else if (infix_str[i] == '+' || infix_str[i] == '-' || infix_str[i] == '*' || infix_str[i] == '/' || infix_str[i] == '^') {
			//while on stack top unary operation / function or binary with priority more or equal then temp operation
			while (!stack.is_empty()
				&& (stack.top() == 's' || stack.top() == 'c'
					|| stack.top() == 't' || stack.top() == 'g'
					|| stack.top() == 'o' || stack.top() == 'l'
					|| stack.top() == 'q' || stack.top() == 'a'
					|| stack.top() == '_'
					|| ((stack.top() == '+' || stack.top() == '-' || stack.top() == '*'
						|| stack.top() == '/' || stack.top() == '^') &&
						priority(stack.top()) >= priority(infix_str[i])))) {

				postfix_str.push_back(stack.pop()); //popping from stack into string
				postfix_str.push_back(' ');
			}


			stack.push(infix_str[i]); //push temp operation
		}
	}

	//popping all from stack into string
	while (!stack.is_empty()) {
		postfix_str.push_back(stack.pop());
		postfix_str.push_back(' ');
	}

	return postfix_str; //return string
}