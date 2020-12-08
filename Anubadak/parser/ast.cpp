#include <iostream>
#include "ast.h"
using namespace parser;

//i think std::move is used to invoke move constructor 
ASTProgramNode::ASTProgramNode(std::vector<ASTNode*>statements)
	:_statements(std::move(statements)){}


ASTDeclarationNode::ASTDeclarationNode(TYPE type, std::string identifier, 
	int lineNumber, ASTExprNode* expression)
		: _type(type),_identifier(identifier),_lineNumber(lineNumber),
			_expression(std::move(expression)){}

ASTAssignmentNode::ASTAssignmentNode(std::string identifier, int lineNumber,
	ASTExprNode* expression)
	: _identifier(identifier), _lineNumber(lineNumber), _expression(std::move(expression)) {}

ASTPrintNode::ASTPrintNode( int lineNumber,
	ASTExprNode* expression)
	:_lineNumber(lineNumber), _expression(std::move(expression)) {}

ASTIdentifierNode::ASTIdentifierNode(std::string identifier, int lineNumber):
	_identifier(identifier),
		_lineNumber(lineNumber){}

ASTFunctionCallNode::ASTFunctionCallNode(std::string identifier, 
	std::vector<ASTExprNode*>parameters,
	int lineNumber):_identifier(identifier),_parameters(std::move(parameters)) ,
	_lineNumber(lineNumber){}

ASTBinaryExprNode::ASTBinaryExprNode(std::string optor, ASTExprNode* expr1,
	ASTExprNode* expr2, int lineNumber)
	:_optor(optor),_expr1(expr1), _expr2(expr2), _lineNumber(lineNumber) {
	
	//std::cout << _optor +"*************************************" << std::endl;
}

ASTUnaryExprNode::ASTUnaryExprNode(std::string optor, ASTExprNode* expr, int lineNumber)
	: _optor(std::move(optor)), _expr(expr), _lineNumber(lineNumber) {}

ASTBlockNode::ASTBlockNode(std::vector<ASTStatementNode*> statements,
	int lineNumber) : _statements(std::move(statements)), _lineNumber(lineNumber){}

ASTIfNode::ASTIfNode(ASTExprNode* condition, ASTBlockNode* ifBlock, 
	int lineNumber, ASTBlockNode* elseBlock )
	:_condition(condition), _ifBlock(ifBlock ),
	_lineNumber(lineNumber), _elseBlock(elseBlock) {}

ASTReturnNode::ASTReturnNode(ASTExprNode* expression, int lineNumber)
	:_expr(expression),_lineNumber(lineNumber){}

ASTWhileNode::ASTWhileNode(ASTExprNode* condition, ASTBlockNode* ifBlock,
	int lineNumber)
	: _condition(condition), _whileBlock(ifBlock),
	_lineNumber(lineNumber) {}

ASTFunctionDefinitionNode::ASTFunctionDefinitionNode(
	std::string identifier, 
	std::vector<std::pair<std::string, TYPE>>parameters, 
	TYPE type,
	ASTBlockNode* funcDefBlock, 
	int lineNumber)
	: _funcName(identifier),
	_parameters(std::move(parameters)),
	_returnType(type),
	_funcDefBlock(funcDefBlock),
	_lineNumber(lineNumber)
{
	for (auto parameter : _parameters) {
		_variableNames.push_back(parameter.first);
		_signature.push_back(parameter.second);
	}
}

//definitions of accept funtions
void ASTProgramNode::accept(visitor::Visitor* visitor) {
	//checkPurpose
	//std::cout << "accept function definition for astProgramNode" << std::endl;
	visitor->visit(this);
}

void ASTDeclarationNode::accept(visitor::Visitor* visitor) {
	//checkPurpose
	//std::cout << "accept function definition for astDeclaration" << std::endl;
	visitor->visit(this);

}
void ASTPrintNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);

}

void ASTAssignmentNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}

void ASTReturnNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTBlockNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTIfNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTWhileNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTFunctionDefinitionNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTIdentifierNode::accept(visitor::Visitor* visitor) {
	
	visitor->visit(this);
	//std::cout << "inside iddntifiernode accept definition ast.cpp" << std::endl;
}
void ASTFunctionCallNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTBinaryExprNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}
void ASTUnaryExprNode::accept(visitor::Visitor* visitor) {
	visitor->visit(this);
}

	template<>
	void ASTLiteralNode<int>::accept(visitor::Visitor* visitor) {
		//std::cout << "astLiteralNode definition in ast.cpp" << std::endl;
		visitor->visit(this);
	}

	template<>
	void ASTLiteralNode<float>::accept(visitor::Visitor* visitor) {
		visitor->visit(this);
	}

	template<>
	void ASTLiteralNode<bool>::accept(visitor::Visitor* visitor) {
		visitor->visit(this);
	}

	template<>
	void ASTLiteralNode<std::string>::accept(visitor::Visitor* visitor) {
		visitor->visit(this);
	}
