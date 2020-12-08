#include <iostream>
#include "token.h"

using namespace lexer;

Token::Token(int finalState, std::string value, int lineNumber)
	:type(determineTokenType(finalState, value)), value(value), lineNumber(lineNumber)
{
}
Token::Token() = default;

///shows the relationship between states and tokens
TOKEN Token::determineTokenType(int finalState, std::string value)
{
	switch (finalState) {
	case 1:
		return TOKEN::TOK_INT;

	case 3:
		return TOKEN::TOK_REAL;

	case 4:
		return TOKEN::TOK_ADDITIVE_OP;

	case 5:
	case 11:
		return TOKEN::TOK_MULTIPLICATIVE_OP;

	case 7:
		return TOKEN::TOK_RELATIONAL_OP;

	case 8:
		return TOKEN::TOK_EQUALS;

	case 9:
		return TOKEN::TOK_RELATIONAL_OP;

	case 10:
		if (value == "var")
			return TOKEN::TOK_VAR;
		if (value == "set")
			return TOKEN::TOK_SET;
		if (value == "def")
			return TOKEN::TOK_DEF;
		if (value == "return")
			return TOKEN::TOK_RETURN;
		if (value == "if")
			return TOKEN::TOK_IF;
		if (value == "else")
			return TOKEN::TOK_ELSE;
		if (value == "while")
			return TOKEN::TOK_WHILE;
		if (value == "print")
			return TOKEN::TOK_PRINT;
		if (value == "int")
			return TOKEN::TOK_INT_TYPE;
		if (value == "real")
			return TOKEN::TOK_REAL_TYPE;
		if (value == "bool")
			return TOKEN::TOK_BOOL_TYPE;
		if (value == "string")
			return TOKEN::TOK_STRING_TYPE;
		if (value == "true" || value == "false")
			return TOKEN::TOK_BOOL;
		if (value == "and")
			return TOKEN::TOK_MULTIPLICATIVE_OP;
		if (value == "or")
			return TOKEN::TOK_ADDITIVE_OP;
		if (value == "not")
			return TOKEN::TOK_NOT;
		else return TOKEN::TOK_IDENTIFIER;

	case 14:
	case 16:
		return TOKEN::TOK_COMMENT;

	case 20:
		return TOKEN::TOK_STRING;

	case 21:
		if (value == "{")
			return TOKEN::TOK_LEFT_CURLY;
		if (value == "}")
			return TOKEN::TOK_RIGHT_CURLY;
		if (value == "(")
			return TOKEN::TOK_LEFT_BRACKET;
		if (value == ")")
			return TOKEN::TOK_RIGHT_BRACKET;
		if (value == ",")
			return TOKEN::TOK_COMMA;
		if (value == ";")
			return TOKEN::TOK_SEMICOLON;
		if (value == ":")
			return TOKEN::TOK_COLON;

	case 22:
		return TOKEN::TOK_EOF;

	default:
		return TOKEN::TOK_ERROR;
	}
}
int Token::getType()
{
	return static_cast<int>(type);

}