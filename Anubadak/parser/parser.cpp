#pragma once
#include <iostream>
#include <exception>
#include <string>


#include"parser.h"
#include "../lexer/lexer.h"

static void check(std::string printText)
{
	if(DEBUG)
		std::cout << printText << std::endl;
}


using namespace parser;
using namespace lexer;

namespace parser
{
	Parser::Parser(lexer::Lexer* lex) :_lex(lex)
	{
		if(DEBUG)
			check("\n\n ***** PARSING PORTION ****\n\n");
		_currentToken = lex->nextToken();
		_nextToken = lex->nextToken();
	}

	///
	Parser::Parser(lexer::Lexer* lex, unsigned int tokens) : _lex(lex) {
		_nextToken = lex->nextToken();
		for (unsigned int i = 0; i < tokens; i++)
			consumeToken();
	}

	///
	void Parser::consumeToken()
	{
		_currentToken = _nextToken;
		_nextToken = _lex->nextToken();
	}
	ASTProgramNode* Parser::parse_program() {
		auto statements = new::std::vector<ASTNode*>;
		while (_currentToken.type != TOKEN::TOK_EOF)
		{
			statements->push_back(parseStatement());
			consumeToken();
		}
		//rememnber that astprogramnode constructor uses std::move() to the statements argument
		return new parser::ASTProgramNode(*statements);
	}
	ASTNode* Parser::parseStatement() {
		switch (_currentToken.type)
		
		{
		case TOKEN::TOK_VAR:
			return parseDeclarationStatement();

		case TOKEN::TOK_SET:
			return parseAssignmentStatement();

		case TOKEN::TOK_PRINT:
			return parsePrintStatement();

		case TOKEN::TOK_LEFT_CURLY:
			return parseBlock();
		
		case TOKEN::TOK_IF:
			return parseIfBlock();
		
		case TOKEN::TOK_RETURN:
			return parseReturnStatement();

		case TOKEN::TOK_WHILE:
			return parseWhileBlock();

		case TOKEN::TOK_DEF:
			return parseFunctionDefinition();

		case TOKEN::TOK_IDENTIFIER:
			if (_nextToken.type == TOKEN::TOK_LEFT_BRACKET) {
				auto x = parseFunctionCallExpr();
				consumeToken();
				if(_currentToken.type!=TOKEN::TOK_SEMICOLON)
					throw std::runtime_error("invalid syntax missing semicolon "
						+ std::to_string(_currentToken.getLineNumber()) + ".");
				return x;
			}

			else
				throw std::runtime_error("invalid syntax  at line "
					+ std::to_string(_currentToken.getLineNumber()) + ".");

		default: {
			if (DEBUG)
				std::cout << "current token = "<<_currentToken.value<<std::endl;
			throw std::runtime_error("invalid syntax at line may be you forgot set "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}
		}	
	}
	ASTDeclarationNode* Parser::parseDeclarationStatement()
	{
		TYPE type;
		std::string identifier;
		ASTExprNode* expr;
		int lineNumber;

		consumeToken();
		lineNumber = _currentToken.getLineNumber();

		if (_currentToken.getType() != static_cast<int>(TOKEN::TOK_IDENTIFIER)) {
			throw std::runtime_error("Expected variable name after 'var' on line "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}
		identifier = _currentToken.getVal();

		consumeToken();
		if (_currentToken.type != TOKEN::TOK_COLON)
		{
			throw std::runtime_error("expected colon ':' after identifier  in line "
				+ std::to_string(_currentToken.getLineNumber()));
		}

		consumeToken();
		// in future identifier will be passed into this function
		type = parseType();

		consumeToken();
		if (_currentToken.type != TOKEN::TOK_EQUALS) {
			throw std::runtime_error("expected '=' after  in line "
				+ std::to_string(_currentToken.getLineNumber()));
		}
		expr = parseExpression();
		//checkPurpose
		//std::cout << "curretn token=" << _currentToken.getVal() << " type= "<<_currentToken.getType() << std::endl;
		if (_currentToken.type != TOKEN::TOK_SEMICOLON)
		consumeToken();	
		//checkPurpose
		//std::cout << "curretn token="<<_currentToken.getVal() << " type= "<< _currentToken.getType() << std::endl;
		if (_currentToken.type != TOKEN::TOK_SEMICOLON)
		{
			throw std::runtime_error("expected ';'   in line "
				+ std::to_string(_currentToken.getLineNumber()));
		}
		if(DEBUG)
			std::cout << "sucessfully declared variable " << identifier << std::endl;
		
		return new ASTDeclarationNode(type, identifier, lineNumber, expr);
	}

	ASTAssignmentNode* Parser:: parseAssignmentStatement() {
		consumeToken();

		ASTExprNode* expr;
		std::string identifier = _currentToken.getVal();
		int lineNumber= _currentToken.getLineNumber();

		
		if (_currentToken.type != TOKEN::TOK_IDENTIFIER)
		{
			throw std::runtime_error("expexted identifier in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_EQUALS)
		{
			throw std::runtime_error("expexted '=' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		expr = parseExpression();

		consumeToken();
		if (_currentToken.type != TOKEN::TOK_SEMICOLON)
		{
			throw std::runtime_error("expexted ';' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		//checkPurpose
		check("variable assignment sucessful");
		return new ASTAssignmentNode(identifier, lineNumber, expr);
	}

	ASTPrintNode* Parser::parsePrintStatement() {
		int lineNumber = _currentToken.getLineNumber();
		ASTExprNode *expr;
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_BRACKET)
		{
			throw std::runtime_error("expexted '(' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		if (_nextToken.type == TOKEN::TOK_RIGHT_BRACKET) {
			throw std::runtime_error("dont try to fool me i m machine but smarter than you idiot! " + 
				std::to_string(_currentToken.getLineNumber()));
		}
		expr = parseExpression();
		
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET)
		{
			throw std::runtime_error("expexted ')' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_SEMICOLON)
		{
			throw std::runtime_error("expexted ';' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		//checkPurpose
		check("print statement sucessfully parsedd");

		return new ASTPrintNode(lineNumber,expr);
	}

	ASTBlockNode* Parser::parseBlock() {
		auto statements = new std::vector<ASTStatementNode* >;
		int lineNumber = _currentToken.getLineNumber();

		consumeToken();
		while (_currentToken.type != TOKEN::TOK_RIGHT_CURLY and
			_currentToken.type != TOKEN::TOK_ERROR and
			_currentToken.type != TOKEN::TOK_EOF)
		{
			statements->push_back(dynamic_cast<ASTStatementNode*>(parseStatement()));
			consumeToken();

		}

		if(_currentToken.type==TOKEN::TOK_RIGHT_CURLY){ 
			//checkPurpose 
			check("block of code sucessfully parsed");
			return new ASTBlockNode(*statements, lineNumber);
		}
		else {
			throw std::runtime_error("Reached end of file while parsing."
				" Mismatched scopes.");
		}
		
	}

	ASTIfNode* Parser::parseIfBlock() {
		ASTExprNode* condition;
		ASTBlockNode* ifBlock;
		ASTBlockNode* elseBlock;
		int lineNumber = _currentToken.getLineNumber();
		
		//consumes '('
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_BRACKET)
		{
			throw std::runtime_error("expexted '(' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		if (_nextToken.type == TOKEN::TOK_RIGHT_BRACKET)
		{
			throw std::runtime_error("please provide condition for if in  " + std::to_string(_currentToken.getLineNumber()));
		}

		condition = parseExpression();
		//consumes ')'
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET)
		{
			throw std::runtime_error("expexted ')' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_CURLY)
		{
			throw std::runtime_error(" xpexted '{ in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		ifBlock = parseBlock();

		if (_nextToken.type != TOKEN::TOK_ELSE) {
			return new ASTIfNode(condition, ifBlock, lineNumber);
		}
		else {
			//consumes else
			consumeToken();
			//consume '{'
			consumeToken();
			if (_currentToken.type != TOKEN::TOK_LEFT_CURLY)
			{
				throw std::runtime_error(" xpexted '{ in the line " + std::to_string(_currentToken.getLineNumber()));
			}
			elseBlock = parseBlock();

			//checkPurpose
			check("elseblock sucessfully parsed");
			return new ASTIfNode(condition, ifBlock, lineNumber,elseBlock);

		}

	}

	ASTWhileNode* Parser::parseWhileBlock() {
		ASTExprNode* condition;
		ASTBlockNode* whileBlock;
		int lineNumber = _currentToken.getLineNumber();

		//consumes '('
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_BRACKET)
		{
			throw std::runtime_error("expexted '(' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		if (_nextToken.type == TOKEN::TOK_RIGHT_BRACKET)
		{
			throw std::runtime_error("please provide condition for if in  " + std::to_string(_currentToken.getLineNumber()));
		}

		condition = parseExpression();
		//consumes ')'
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET)
		{
			throw std::runtime_error("expexted ')' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_CURLY)
		{
			throw std::runtime_error(" xpexted '{ in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		whileBlock = parseBlock();
		return new ASTWhileNode(condition, whileBlock, lineNumber);
	}



	ASTReturnNode* Parser::parseReturnStatement() {
		int lineNumber = _currentToken.getLineNumber();
		ASTExprNode* expr = parseExpression();

		consumeToken();
		if (_currentToken.type != TOKEN::TOK_SEMICOLON)
		{
			throw std::runtime_error("expexted ';' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		//checkPurpose
		check("return statement parsed sucessfully");
		return new ASTReturnNode(expr, lineNumber);

	}

	ASTFunctionDefinitionNode* Parser::parseFunctionDefinition() {
		std::string funcName;
		std::vector<std::pair<std::string, TYPE>>parameters;
		ASTBlockNode* funcDefBlock;
		TYPE type;
		int lineNumber = _currentToken.getLineNumber();

		consumeToken();
		if (_currentToken.getType() != static_cast<int>(TOKEN::TOK_IDENTIFIER)) {
			throw std::runtime_error("Expected identifier after 'def' on line "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}
		funcName = _currentToken.value;
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_BRACKET)
		{
			throw std::runtime_error("expexted '(' in the line " + std::to_string(_currentToken.getLineNumber()));
		}
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET)
		{
			parameters.push_back(*parseFormalParameter());
			
			consumeToken();
			while (_currentToken.type == TOKEN::TOK_COMMA) {
				consumeToken();

				//checkPurpose
				if (DEBUG)
					std::cout << _currentToken.getVal() << std::endl << std::endl;
				parameters.push_back(*parseFormalParameter());
				consumeToken();
			}
			if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET) {
				throw std::runtime_error("Expected ')' on line "
					+ std::to_string(_currentToken.getLineNumber()) + ".");
			}
		}
		consumeToken();
		if (_currentToken.type != TOKEN::TOK_COLON) {
			throw std::runtime_error("Expected ':' on line "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}
		consumeToken();
		type = parseType();

		consumeToken();
		if (_currentToken.type != TOKEN::TOK_LEFT_CURLY) {
			throw std::runtime_error("Expected '{' on line "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}
		funcDefBlock=parseBlock();
		//checkPurpose
		check("function definition parsed sucessfully");
		return new ASTFunctionDefinitionNode(funcName, parameters, type, funcDefBlock, lineNumber);



	}
	

	 std::pair<std::string, TYPE>*Parser::parseFormalParameter() {
		 std::string identifier;
		 TYPE type;
		 //checkPurpose
		 if(DEBUG)
			std::cout << _currentToken.getVal() << std::endl<<std::endl;

		 if (_currentToken.getType() != static_cast<int>(TOKEN::TOK_IDENTIFIER)) {
			 throw std::runtime_error("Expected identifier on line "
				 + std::to_string(_currentToken.getLineNumber()) + ".");
		 }
		 identifier = _currentToken.getVal();
		 consumeToken();
		 if (_currentToken.type != TOKEN::TOK_COLON)
		 {
			 throw std::runtime_error("expected colon ':' after identifier  in line "
				 + std::to_string(_currentToken.getLineNumber()));
		 }
		 consumeToken();
		 type = parseType();

		 return new std::pair<std::string, TYPE>(identifier, type);


		

	}
	// in future this function will take string i e identifier from calling place
	TYPE Parser::parseType()
	{

		switch (_currentToken.type)
		{
		case TOKEN::TOK_INT_TYPE:
			return TYPE::INT;

		case TOKEN::TOK_STRING_TYPE:
			return TYPE::STRING;

		case TOKEN::TOK_REAL_TYPE:
			return TYPE::REAL;

		case TOKEN::TOK_BOOL_TYPE:
			return TYPE::BOOL;

		default: {
	
			throw std::runtime_error(_currentToken.getVal() + " is not valid type  in line "
				+ std::to_string(_currentToken.getLineNumber()));
		}


		}
	}

	ASTExprNode*  Parser::parseExpression() {
		ASTExprNode* simpleExpression = parseSimpleExpression();
		int lineNumber = _currentToken.getLineNumber();

		if (_nextToken.type == TOKEN::TOK_RELATIONAL_OP) {

			consumeToken();
			//i cant write _currentToken.getVal() in the argument because c++ donot guranteee the order of
			//execution of funciton
			std::string op = _currentToken.getVal();
			return new ASTBinaryExprNode(op, simpleExpression, parseExpression(), lineNumber);
		}
		return simpleExpression;



	}
	ASTExprNode* Parser::parseSimpleExpression() {
		ASTExprNode* term = parseExpressionTerm();
		
		int lineNumber = _currentToken.getLineNumber();

		if (_nextToken.type == TOKEN::TOK_ADDITIVE_OP) {
			//i cant write _currentToken.getVal() in the argument because c++ donot guranteee the order of
			//execution of funciton
			consumeToken();
			std::string op = _currentToken.value;
			return new ASTBinaryExprNode(op, term, parseSimpleExpression(), lineNumber);
		}
		return term;
		

	}
	ASTExprNode* Parser::parseExpressionTerm() {
		ASTExprNode* factor = parseExpressionFactor();
		int lineNumber = _currentToken.getLineNumber();

		if (_nextToken.type == TOKEN::TOK_MULTIPLICATIVE_OP) {
			//i cant write _currentToken.getVal() in the argument because c++ donot guranteee the order of
			//execution of funciton
			consumeToken();
			std::string op = _currentToken.getVal();
			return new ASTBinaryExprNode(op, factor, parseExpressionTerm(), lineNumber);
		}
		return factor;
	}
	
	ASTExprNode* Parser::parseExpressionFactor() {
		consumeToken();
		int lineNumber = _currentToken.getLineNumber();

		switch (_currentToken.type)
		{
		case TOKEN::TOK_INT: {
			//check("in int type");
			//std::cout << "int maaa" << std::endl;
			return new ASTLiteralNode<int>(std::stoi(_currentToken.getVal()), lineNumber);
		}

		case TOKEN::TOK_REAL: {
			//check("in real type");
			/*std::cout << _currentToken.getLineNumber();
			std::cout << "float maaa" << std::endl;
			std::cout << "float maaa"<< std::stof(_currentToken.getVal()) << std::endl;*/

			return new ASTLiteralNode<float>(std::stof(_currentToken.getVal()), lineNumber); 
		}

		case TOKEN::TOK_BOOL:
			return new ASTLiteralNode<bool>(_currentToken.getVal() == "true", lineNumber);

		case TOKEN::TOK_STRING:
		{
			// Remove " character from front and end of lexeme
			std::string str = _currentToken.value.substr(1, _currentToken.value.size() - 2);

			// Replace \" with quote
			size_t pos = str.find("\\\"");
			while (pos != std::string::npos) {
				// Replace
				str.replace(pos, 2, "\"");
				// Get next occurrence from current position
				pos = str.find("\\\"", pos + 2);
			}

			// Replace \n with newline
			pos = str.find("\\n");
			while (pos != std::string::npos) {
				// Replace
				str.replace(pos, 2, "\n");
				// Get next occurrence from current position
				pos = str.find("\\n", pos + 2);
			}

			// Replace \t with tab
			pos = str.find("\\t");
			while (pos != std::string::npos) {
				// Replace
				str.replace(pos, 2, "\t");
				// Get next occurrence from current position
				pos = str.find("\\t", pos + 2);
			}

			// Replace \b with backslash
			pos = str.find("\\b");
			while (pos != std::string::npos) {
				// Replace
				str.replace(pos, 2, "\\");
				// Get next occurrence from current position
				pos = str.find("\\b", pos + 2);
			}

			return new ASTLiteralNode<std::string>(std::move(str), lineNumber);

			////removes two "  from begining and end
			//std::string str = _currentToken.getVal().substr(1, _currentToken.getVal().size() - 2);
			////TODO: string checking for varipus specia; characters is left for  now
			//return new ASTLiteralNode<std::string>(_currentToken.getVal(), lineNumber);
		}

		case TOKEN::TOK_IDENTIFIER: {
			if (_nextToken.type == TOKEN::TOK_LEFT_BRACKET) {
				return parseFunctionCallExpr();
			}
			else
				return new ASTIdentifierNode(_currentToken.getVal(), _currentToken.getLineNumber());
		}
		case TOKEN::TOK_ADDITIVE_OP:
		case TOKEN::TOK_NOT: {
			//checkPurpose
			check("Unary expression node....................................");
			std::string op = _currentToken.getVal();
			return new ASTUnaryExprNode(op, parseExpression(), _currentToken.getLineNumber());
		}
		case TOKEN::TOK_LEFT_BRACKET: {
			ASTExprNode* subExpr = parseExpression();

			consumeToken();
			if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET)
				throw std::runtime_error("Expected ')' on line "
					+ std::to_string(_currentToken.getLineNumber()) + ".");

			return subExpr;
		}
		}
	}
	ASTFunctionCallNode* Parser::parseFunctionCallExpr()
	{
		std::string identifier = _currentToken.getVal();
		int lineNumber = _currentToken.getLineNumber();
		auto parameters = new std::vector<ASTExprNode*>;

		consumeToken();
		//checkPurpose
		//std::cout << "curretn token =" << _currentToken.getVal()<<std::endl;
		if (_currentToken.type != TOKEN::TOK_LEFT_BRACKET)
		{
			throw std::runtime_error("Expected 'a(' on line "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}
		if (_nextToken.type != TOKEN::TOK_RIGHT_BRACKET)
		{
			parameters = parseActualParameter();
		}
		else {
			//consumes ')'
			consumeToken();
		}
			
		//checkPurpose
		if (DEBUG)
			std::cout << "current token = in function call =" << _currentToken.getVal()<<std::endl;
		
		if (_currentToken.type != TOKEN::TOK_RIGHT_BRACKET) {
			throw std::runtime_error("Expected ')' on line "
				+ std::to_string(_currentToken.getLineNumber()) + ".");
		}


		return new ASTFunctionCallNode(identifier, *parameters, lineNumber);

	}
	std::vector<ASTExprNode*>* Parser::parseActualParameter() {
		//checkPurpose
		//std::cout << "check>>>>>>>>>>>>>>>>>" << std::endl;
		auto parameters = new std::vector<ASTExprNode*>;
		
		//this pushes first token then checks for comma token we also could have used do while loop instead
		parameters->push_back(parseExpression());
		
		//if there are more parameters than one
		consumeToken();
		while (_currentToken.type == TOKEN::TOK_COMMA)
		{
			parameters->push_back(parseExpression());
			consumeToken();
		}
		return parameters;
	}
		
}
