
/**
*@author Aayush Neupane
*@date feb 2019
*/

#pragma once
const bool DEBUG = !true;

namespace lexer {

	/*
	*an enum that contain all possible token type
	*/
	enum class TOKEN
	{
		TOK_INT,
		TOK_REAL,
		TOK_ADDITIVE_OP,
		TOK_MULTIPLICATIVE_OP,
		TOK_RELATIONAL_OP,
		TOK_EQUALS,
		TOK_VAR,
		TOK_SET,
		TOK_DEF,
		TOK_RETURN,
		TOK_IF,
		TOK_ELSE,
		TOK_WHILE,
		TOK_PRINT,
		TOK_INT_TYPE,
		TOK_REAL_TYPE,
		TOK_BOOL_TYPE,
		TOK_STRING_TYPE,
		TOK_BOOL,
		TOK_NOT,
		TOK_IDENTIFIER,
		TOK_COMMENT,
		TOK_STRING,
		TOK_LEFT_CURLY,
		TOK_RIGHT_CURLY,
		TOK_LEFT_BRACKET,
		TOK_RIGHT_BRACKET,
		TOK_COMMA,
		TOK_SEMICOLON,
		TOK_COLON,
		TOK_EOF,
		TOK_ERROR
	};

	///this is a Token class.
	///this is description about this class
	class Token
	{
	private:
		int lineNumber;
		
		TOKEN determineTokenType(int  finalState, std::string);

	public:
		std::string value;
		int getLineNumber() { return lineNumber; }
		int getType();
		TOKEN type;
		std::string getVal() { return value; }
		Token(int  finalState, std::string, int  line_number = 0);
		Token();

	};

}




