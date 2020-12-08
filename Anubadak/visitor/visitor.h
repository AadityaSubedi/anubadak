#pragma once

namespace parser {

	class ASTProgramNode;

	class ASTDeclarationNode;
	class ASTAssignmentNode;
	class ASTPrintNode;
	class ASTReturnNode;
	class ASTBlockNode;
	class ASTIfNode;
	class ASTWhileNode;
	class ASTFunctionDefinitionNode;

	template <typename T> class ASTLiteralNode;
	class ASTBinaryExprNode;
	class ASTIdentifierNode;
	class ASTUnaryExprNode;
	class ASTFunctionCallNode;
}

namespace visitor
{
	class Visitor {

	public:
		virtual  void visit(parser::ASTProgramNode*) = 0;
		virtual  void visit(parser::ASTDeclarationNode*) = 0;
		virtual  void visit(parser::ASTAssignmentNode*) = 0;
		virtual  void visit(parser::ASTPrintNode*) = 0;
		virtual  void visit(parser::ASTReturnNode*) = 0;
		virtual  void visit(parser::ASTBlockNode*) = 0;
		virtual  void visit(parser::ASTIfNode*) = 0;
		virtual  void visit(parser::ASTWhileNode*) = 0;
		virtual  void visit(parser::ASTFunctionDefinitionNode*) = 0;
		//
		////template class derived from the ASTExpr node
		virtual void visit(parser::ASTLiteralNode<int>*) = 0;
		virtual void visit(parser::ASTLiteralNode<float>*) = 0;
		virtual void visit(parser::ASTLiteralNode<bool>*) = 0;
		virtual void visit(parser::ASTLiteralNode<std::string>*) = 0;

		////Derived classes of ASTExprNode
		virtual  void visit(parser::ASTBinaryExprNode*) = 0;
		virtual  void visit(parser::ASTIdentifierNode*) = 0;
		virtual  void visit(parser::ASTUnaryExprNode*) = 0;
		virtual  void visit(parser::ASTFunctionCallNode*) = 0;
	};
}
