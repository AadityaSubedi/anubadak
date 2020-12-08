#pragma once

#include "ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include <vector>

namespace parser
{
	class Parser
	{
	public:
		//parser ko main kura parser constructor ta lekhekai xaina ni
		Parser(lexer::Lexer* lex);
		Parser(lexer::Lexer* lex, unsigned int tokens);
		void consumeToken();
		parser::ASTProgramNode* parse_program();
		parser::ASTNode* parseStatement();
		parser::ASTDeclarationNode* parseDeclarationStatement();
		parser::ASTAssignmentNode* parseAssignmentStatement();
		parser::ASTPrintNode* parsePrintStatement();
		parser::ASTBlockNode* parseBlock();
		parser::ASTIfNode* parseIfBlock();
		parser::ASTReturnNode* parseReturnStatement();
		parser::ASTWhileNode* parseWhileBlock();
		parser::ASTFunctionDefinitionNode* parseFunctionDefinition();
		std::pair<std::string, TYPE>* parseFormalParameter();
		parser::TYPE parseType();
		

		//member functions to evaluate expressoion
		parser::ASTExprNode* parseExpression();
		parser::ASTExprNode* parseSimpleExpression();
		parser::ASTExprNode* parseExpressionTerm();
		parser::ASTExprNode* parseExpressionFactor();

		parser::ASTFunctionCallNode* parseFunctionCallExpr();
		

		//parse type and parameters
		std::vector<ASTExprNode*>* parseActualParameter();
		
	private:
		lexer::Lexer* _lex;
		lexer::Token _currentToken;
		lexer::Token _nextToken;
	};
}

