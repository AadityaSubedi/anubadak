#pragma once
//just inculded for DEBUG
#include "../lexer/token.h"
#include <iostream>
#include <vector>
#include "../visitor/visitor.h"

//abstract nodes which cannot be instantiaated
namespace parser
{
	enum class TYPE
	{
		INT,
		REAL,
		BOOL,
		STRING
	};

	//abstract nodes
	class ASTNode
	{
	public:
		virtual void accept( visitor::Visitor*) = 0;
	};

	class ASTExprNode :public ASTNode
	{
	public:
		virtual void accept(visitor ::Visitor*)=0;
	};
	class ASTStatementNode : public ASTNode
	{
	public:
		virtual void accept(visitor::Visitor*) = 0;

	};

	//statement Nodes
	class ASTProgramNode :public ASTStatementNode
	{
	public:
		std::vector<ASTNode*> _statements;
		ASTProgramNode(std::vector<ASTNode*>statements);
		virtual void accept(visitor::Visitor*) override;
	};


	class ASTDeclarationNode :public ASTStatementNode
	{
	public:
		virtual void accept(visitor::Visitor*) override;
		TYPE _type;
		std::string _identifier;
		int _lineNumber;
		ASTExprNode* _expression;

		ASTDeclarationNode(TYPE type, std::string identifier, int lineNumber, ASTExprNode* expression);

	};


	class ASTAssignmentNode :public ASTStatementNode
	{
	private:
		
	public:
		std::string _identifier;
		int _lineNumber;
		ASTExprNode* _expression;
		virtual void accept(visitor::Visitor*) override;
		ASTAssignmentNode(std::string identifier, int lineNumber, ASTExprNode* expression);
	};


	class ASTPrintNode :public ASTStatementNode
	{
	private:
	public:
		int _lineNumber;
		ASTExprNode* _expression;
		virtual void accept(visitor::Visitor*) override;
		ASTPrintNode(int lineNumber, ASTExprNode* expression);
	};


	class ASTReturnNode :public ASTStatementNode
	{
		
	public:
		int _lineNumber;
		ASTExprNode* _expr;
		virtual void accept(visitor::Visitor*) override;
		ASTReturnNode(ASTExprNode * expression, int lineNumber);
	};


	class ASTBlockNode :public ASTStatementNode
	{
		
	public:
		std::vector<ASTStatementNode*> _statements;
		int _lineNumber;
		virtual void accept(visitor::Visitor*) override;
		ASTBlockNode(std::vector<ASTStatementNode*> statements, int lineNumber);

	};


	class ASTIfNode :public ASTStatementNode
	{
		
	public:
		int _lineNumber;
		ASTExprNode* _condition;
		ASTBlockNode* _ifBlock;
		ASTBlockNode* _elseBlock;
		virtual void accept(visitor::Visitor*) override;
		ASTIfNode(ASTExprNode *condition, ASTBlockNode * ifBlock , 
			int lineNumber, ASTBlockNode* elseBlock=nullptr);
	};
	class ASTWhileNode :public ASTStatementNode
	{
		
	public:
		int _lineNumber;
		ASTExprNode* _condition;
		ASTBlockNode* _whileBlock;
		virtual void accept(visitor::Visitor*) override;
		ASTWhileNode(ASTExprNode * condition, ASTBlockNode* ifBlock,
			int lineNumber);
	};
	class ASTFunctionDefinitionNode :public ASTStatementNode
	{
		

	public:
		int _lineNumber;
		std::string _funcName;
		std::vector < std::pair<std::string, TYPE >> _parameters;
		std::vector <TYPE>_signature;
		std::vector <std::string>_variableNames;

		ASTBlockNode* _funcDefBlock;
		TYPE _returnType;

		virtual void accept(visitor::Visitor*) override;
		ASTFunctionDefinitionNode(std::string,std::vector<std::pair<std::string,TYPE>>,TYPE,
			ASTBlockNode *funcDefBlock,int lineNumber);
	};

	template <typename T>
	class ASTLiteralNode:public ASTExprNode
	{
	public:
		virtual void accept(visitor::Visitor*) override;
		ASTLiteralNode(T value, int lineNumber) :_value(value), _lineNumber(lineNumber){}
		T _value;
		int _lineNumber;
	};
	
	class ASTIdentifierNode :public ASTExprNode {
		std::string _identifier;
		int _lineNumber;
	public:
		int getLineNumber() { return _lineNumber; }
		std::string getIdentifier() { return _identifier; }
		virtual void accept(visitor::Visitor*) override;
		ASTIdentifierNode(std::string identifier , int lineNumber);
	};
	
	class ASTFunctionCallNode :public ASTExprNode {
		
	public:
		std::string _identifier;
		int _lineNumber;
		std::vector<ASTExprNode*>_parameters;
		int getLineNumber() { return _lineNumber; }
		std::string getIdentifier() { return _identifier; }
		virtual void accept(visitor::Visitor*) override;
		ASTFunctionCallNode(std::string identifier,std::vector<ASTExprNode*>parameters, int lineNumber);
	};
	
	class ASTBinaryExprNode :public ASTExprNode {
	public:
		std::string _optor;
		int _lineNumber;
		ASTExprNode* _expr1;
		ASTExprNode* _expr2;
		virtual void accept(visitor::Visitor*) override;
		ASTBinaryExprNode(std::string  optor , ASTExprNode* expr1, ASTExprNode* expr2, int lineNumber);
	};
	
	class ASTUnaryExprNode :public ASTExprNode {
		
		
	public:
		std::string _optor;
		int _lineNumber;
		ASTExprNode* _expr;
		virtual void accept(visitor::Visitor*) override;
		ASTUnaryExprNode(std::string optor, ASTExprNode * expr, int lineNumber);
	};
	
}

