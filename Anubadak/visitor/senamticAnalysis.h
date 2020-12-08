#pragma once
#include <utility>
#include <map>
#include <string>
#include "../parser/ast.h"
#include "visitor.h"
#include <stack>

namespace visitor
{
	class SemanticScope {
		//this class should contain following things
		//1. what variables are inside the scope and what are their type
		//2. whether the variable is already declared in that scope
		//3.should be able to declare variable in this scope

		std::map<std::string, std::pair<parser::TYPE, int>>_variableSymbolTable;
		//multimap is choosen to enable function overloading
		//function symbol table should store 
		//function return type
		//function signature
		//and function lineNumber
		std::multimap<std::string, std::tuple<parser::TYPE, std::vector<parser::TYPE>, int>>functionSymbolTable;

	public:


		parser::TYPE getType(std::string identifier);
		parser::TYPE getType(std::string identifier, std::vector<parser::TYPE>signature);
		void declare(std::string identifier, parser::TYPE, int LineNumber);
		void declare(std::string identifier, parser::TYPE returnType, std::vector<parser::TYPE>signature,
			int lineNumber);
		//declaration check for the variable other than function
		bool alreadyDeclared(std::string identifier);
		//declaration check for the function
		bool alreadyDeclared(std::string identifier, std::vector<parser::TYPE>signature);
		int getDeclarationLine(std::string identifier, parser::TYPE);
		int getDeclarationLine(std::string identifier, std::vector<parser::TYPE>signature);

	};


	// in this class i must override all the visit methods for different syntax tree node
	class SemanticAnalyser : public visitor::Visitor
	{
	private:
		parser::TYPE _currentExpressionType;
		std::vector<SemanticScope*>_scopes;
		bool returns(parser::ASTStatementNode* statement);
		std::stack<parser::TYPE>  functions;
		std::vector <std::pair<std::string, parser::TYPE>> currentFunctionParameters;
	public:
		//constructor for semanticAnalyser
		SemanticAnalyser();
		SemanticAnalyser(SemanticScope* global_scope);




		virtual  void visit(parser::ASTProgramNode*) override;
		virtual  void visit(parser::ASTDeclarationNode*) override;
		virtual  void visit(parser::ASTAssignmentNode*) override;
		virtual  void visit(parser::ASTPrintNode*) override;
		virtual  void visit(parser::ASTReturnNode*) override;
		virtual  void visit(parser::ASTBlockNode*) override;
		virtual  void visit(parser::ASTIfNode*) override;
		virtual  void visit(parser::ASTWhileNode*) override;
		virtual  void visit(parser::ASTFunctionDefinitionNode*) override;

		////template class derived from the ASTExpr node
		virtual void visit(parser::ASTLiteralNode<int>*) override;
		virtual void visit(parser::ASTLiteralNode<float>*) override;
		virtual void visit(parser::ASTLiteralNode<bool>*) override;
		virtual void visit(parser::ASTLiteralNode<std::string>*) override;

		virtual  void visit(parser::ASTBinaryExprNode*) override;
		virtual  void visit(parser::ASTIdentifierNode*) override;
		virtual  void visit(parser::ASTUnaryExprNode*) override;
		virtual  void visit(parser::ASTFunctionCallNode*) override;
	};
	std::string inString(parser::TYPE t);
}
