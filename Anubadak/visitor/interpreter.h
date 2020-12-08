#pragma once
#include <vector>
#include <string>
#include <map >
#include "../parser/ast.h"
#include "visitor.h"
#include <wx/frame.h>
#include <wx/textctrl.h>

namespace visitor
{
	struct value_t {
		value_t() :i(0), f(0.0), b(false), s("") {}
		int i;
		float f;
		bool b;
		std::string s;

	};
	class InterpreterScope
	{
	private:
		std::map < std::string, std::pair<parser::TYPE, value_t>> variableSymbolTable;

		//function name,signature,parameterlist ra block hola
		std::multimap < std::string,
			std::tuple <
			std::vector<parser::TYPE>,
			std::vector < std::string>,
			parser::ASTBlockNode* >> functionSymbolTable;
	public:
		//checking declaration of variable
		bool alreadyDeclared(std::string);
		value_t getValueOf(std::string identifier);
		std::vector<std::string>getVariablesNamesOf(std::string,
			std::vector<parser::TYPE>signature);
		bool alreadyDeclared(std::string identifier, std::vector<parser::TYPE>);

		parser::ASTBlockNode* getBlockOf(std::string,
			std::vector<parser::TYPE>signature);
		void declare(std::string, int);
		void declare(std::string, float);
		void declare(std::string, bool);
		void declare(std::string, std::string);
		void declare(std::string, std::vector<parser::TYPE>,
			std::vector<std::string>, parser::ASTBlockNode*);
		parser::TYPE getTypeOf(std::string);
	};


	class Interpreter : public visitor::Visitor {
	private:
		wxFrame* console;
		enum{CONSOLE,OUTPUT};
		std::vector<InterpreterScope*>_scopes;
		parser::TYPE _currentExpressionType;
		value_t _currentExpressionValue;
		//A parameter is a variable in a method definition. When a method is 
		//called, the arguments are the data you pass into the method's parameters.
		std::vector<std::string>_currentFunctionParameters;
		std::vector<std::pair<parser::TYPE, value_t>>_currentFunctionArguments;

	public:
		wxTextCtrl* output;
		Interpreter(InterpreterScope* globalScope);
		Interpreter();

		// it is different tha ~InterPreter(){} ,this is user defined constructor but it is safe to use default in\
		//this condition
		~Interpreter() = default;
		//done
		virtual  void visit(parser::ASTProgramNode*) override;
		//done
		virtual  void visit(parser::ASTDeclarationNode*) override;
		//done
		virtual  void visit(parser::ASTAssignmentNode*) override;
		//done
		virtual  void visit(parser::ASTPrintNode*) override;
		//done
		virtual  void visit(parser::ASTReturnNode*) override;

		virtual  void visit(parser::ASTBlockNode*) override;
		virtual  void visit(parser::ASTIfNode*) override;
		virtual  void visit(parser::ASTWhileNode*) override;
		virtual  void visit(parser::ASTFunctionDefinitionNode*) override;

		//template class derived from the ASTExpr node
		//all done
		virtual void visit(parser::ASTLiteralNode<int>*) override;
		virtual void visit(parser::ASTLiteralNode<float>*) override;
		virtual void visit(parser::ASTLiteralNode<bool>*) override;
		virtual void visit(parser::ASTLiteralNode<std::string>*) override;

		//done
		virtual  void visit(parser::ASTBinaryExprNode*) override;
		//done
		virtual  void visit(parser::ASTIdentifierNode*) override;
		//done
		virtual  void visit(parser::ASTUnaryExprNode*) override;
		//done
		virtual  void visit(parser::ASTFunctionCallNode*) override;



	};
	//std::string inString(parser::TYPE);

}