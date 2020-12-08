
#include <iostream>
#include <stack>
#include "lexer.h"


using namespace lexer;

Lexer::Lexer(std::string& program,int currentTokenNumber):_currentTokenNumber(currentTokenNumber)
{
	if(DEBUG)
		std::cout << "\n\n********LEXICAL ANALYSIS ********\n\n\n" << std::endl;
	int currentIndex = 0;
	Token tempTok;
	while (currentIndex <= static_cast<int>(program.length()))
	{
		tempTok = generateToken(program, currentIndex);
		//ignore comment
		if(tempTok.getType()!= static_cast<int>(TOKEN::TOK_COMMENT))
			tokens.push_back(tempTok);

	}
}

Token Lexer::nextToken()
{
	if (_currentTokenNumber < static_cast<int>(tokens.size()))
		return tokens[_currentTokenNumber++];
	else {
		std::string error = "Final token surpassed.";
		return Token(31, error);
	}

}
Token Lexer::generateToken(std::string& program, int& currentIndex)
{
	int current_state = 0;
	std::stack<int> state_stack;
	char current_symbol;
	std::string lexeme;

	// Push 'BAD' state on the stack
	state_stack.push(-1);

	// Ignore whitespaces or newlines in front of lexeme
	while (currentIndex < static_cast<int>(program.length()) &&
		(program[currentIndex] == ' ' || program[currentIndex] == '\n' 
			|| program[currentIndex] == '\t'))
		currentIndex++;

	// Check if EOF
	if (currentIndex == program.length()) {
		lexeme = (char)EOF;
		currentIndex++;
		return Token(22, lexeme, getLineNumber(program, currentIndex));
	}

	// While current state is not error state
	while (current_state != e) {
		current_symbol = program[currentIndex];
		lexeme += current_symbol;

		// If current state is final, remove previously recorded final states
		if (isFinal[current_state])
			while (!state_stack.empty())
				state_stack.pop();

		// and push current one on the stack
		state_stack.push(current_state);

		// Go to next state using delta function in DFA
		current_state = transitionFunction(current_state, current_symbol);

		// Update current index for next iteration
		currentIndex++;
	}

	// Rollback loop
	while (!isFinal[current_state] && current_state != -1) {
		current_state = state_stack.top();
		state_stack.pop();
		lexeme.pop_back();
		currentIndex--;
	}

	if (current_state == -1)
		throw std::runtime_error("Lexical error.");


	if (isFinal[current_state])
		return Token(current_state, std::move(lexeme), getLineNumber(program, currentIndex));
	else
		throw std::runtime_error("Lexical error occured");

}

int Lexer::transitionFunction(int currentState, char symbol)
{
	switch (symbol)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return transitions[static_cast <int>(TRANSITION_TYPE::DIGIT)][currentState];

	case '.':
		return transitions[static_cast <int>(TRANSITION_TYPE::PERIOD)][currentState];

	case '+':
	case '-':
		return transitions[static_cast <int>(TRANSITION_TYPE::ADDITIVE_OP)][currentState];

	case '*':
		return transitions[static_cast <int>(TRANSITION_TYPE::ASTERISK)][currentState];

	case '!':
		return transitions[static_cast <int>(TRANSITION_TYPE::EXCL_MARK)][currentState];

	case '>':
	case '<':
		return transitions[static_cast <int>(TRANSITION_TYPE::ORDER_REL)][currentState];

	case '=':
		return transitions[static_cast <int>(TRANSITION_TYPE::EQUALS)][currentState];
	case '_':
		return transitions[static_cast <int>(TRANSITION_TYPE::UNDERSCORE)][currentState];
	case '/':
		return transitions[static_cast <int>(TRANSITION_TYPE::FORWARDSLASH)][currentState];
	case '\\':
		return transitions[static_cast <int>(TRANSITION_TYPE::BACKSLASH)][currentState];
	case '\"':
		return transitions[static_cast <int>(TRANSITION_TYPE::QUOTATION_MARK)][currentState];

	case ':':
	case ';':
	case ',':
	case '(':
	case ')':
	case '{':
	case '}':
		return transitions[static_cast <int>(TRANSITION_TYPE::PUNCTUATION)][currentState];

	case '\n':
		return transitions[static_cast <int>(TRANSITION_TYPE::NEWLINE)][currentState];
	case EOF:
		return transitions[static_cast <int>(TRANSITION_TYPE::ENDOFFILE)][currentState];

	default:
		auto ascii = (int)symbol;

		// If alpha is in [A-Z] or [a-z]
		if (((0x41 <= ascii) && (ascii <= 0x5A)) ||
			((0x61 <= ascii) && (ascii <= 0x7A)))
			return transitions[static_cast <int>(TRANSITION_TYPE::LETTER)][currentState];

		// Else if Printable
		if ((0x20 <= ascii) && (ascii <= 0x7E))
			return transitions[static_cast <int>(TRANSITION_TYPE::PRINTABLE)][currentState];

		// If other
		return transitions[static_cast <int>(TRANSITION_TYPE::OTHER)][currentState];


		/*default:
			auto ascii = (int)symbol;
			if ((ascii >= 0x61) && (ascii <= 0x7A))
				return transitions[static_cast <int>(TRANSITION_TYPE::LETTER)][currentState];
			else
				return transitions[static_cast <int>(TRANSITION_TYPE::OTHER)][currentState];
				*/

	}
}

int Lexer::getLineNumber(std::string& program, int currentIndex)
{
	int lineNumber = 1;
	for (int i = 0; i < currentIndex; i++)
	{
		if (program[i] == '\n')
			lineNumber++;
	}
	return lineNumber;

}





