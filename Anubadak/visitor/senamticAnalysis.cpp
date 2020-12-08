#include "senamticAnalysis.h"
#include <map>

static void check(std::string printText)
{
	if (DEBUG)
		std::cout << printText << std::endl;
}

namespace visitor
{


	bool SemanticScope::alreadyDeclared(std::string identifier, std::vector<parser::TYPE> signature) {

		//because multimap is sorted 
		auto funcs = functionSymbolTable.equal_range(identifier);

		// If key is not present in multimap
		if (std::distance(funcs.first, funcs.second) == 0)
			return false;

		// Check signature for each function in multimap
		for (auto i = funcs.first; i != funcs.second; i++)
			if (std::get<1>(i->second) == signature)
				return true;

		// Function with matching signature not found
		return false;
	}
	bool SemanticScope::alreadyDeclared(std::string identifier) {
		return _variableSymbolTable.find(identifier) != _variableSymbolTable.end();
	}
	parser::TYPE SemanticScope::getType(std::string identifier) {
		if (alreadyDeclared(identifier)) {
			return _variableSymbolTable[identifier].first;
		}
		else
			throw std::runtime_error(
				"error occured while determining type of rhs in getType function semanticAnalysis.cppp");
	}

	parser::TYPE SemanticScope::getType(std::string identifier, std::vector<parser::TYPE>signature) {
		auto funcs = functionSymbolTable.equal_range(identifier);

		// If key is not present in multimap
		if (std::distance(funcs.first, funcs.second) == 0)
			throw std::runtime_error("Something went wrong when determining the type of '" + identifier + "'.");

		// Check signature for each

		//get<0> extracts 0th member of its argument tuple. in my program its TYPE
		for (auto i = funcs.first; i != funcs.second; i++)
			if (std::get<1>(i->second) == signature)
				return std::get<0>(i->second);

		// Function with matching signature not found
		throw std::runtime_error("Something went wrong when determining the type of '" + identifier + "'.");
	}

	void SemanticScope::declare(std::string identifier, parser::TYPE varType, int lineNumber) {
		_variableSymbolTable[identifier] = std::make_pair(varType, lineNumber);
	}
	void SemanticScope::declare(std::string identifier, parser::TYPE returnType, std::vector<parser::TYPE>signature,
		int lineNumber) {
		////checkPurpose
		//check("\n\n\n\n checking declarion of semanticscope calss");
		//check("identifier="+identifier);
		//check("return type="+inString(returnType));


		functionSymbolTable.insert(std::make_pair(identifier, std::make_tuple(returnType, signature, lineNumber)));

	}

	int SemanticScope::getDeclarationLine(std::string identifier, parser::TYPE type) {
		if (alreadyDeclared(identifier))
			return _variableSymbolTable[identifier].second;
		throw std::runtime_error("Something went wrong when determining the line number of '" + identifier + "'.");
	}
	int SemanticScope::getDeclarationLine(std::string identifier, std::vector<parser::TYPE> signature) {
		auto funcs = functionSymbolTable.equal_range(identifier);

		// If key is not present in multimap
		if (std::distance(funcs.first, funcs.second) == 0)
			throw std::runtime_error("Something went wrong when determining the line of '" + identifier + "'.");

		// Check signature for each

		//get<0> extracts 0th member of its argument tuple. in my program its TYPE
		for (auto i = funcs.first; i != funcs.second; i++)
			if (std::get<1>(i->second) == signature)
				return std::get<2>(i->second);

		// Function with matching signature not found
		throw std::runtime_error("Something went wrong when determining the line of '" + identifier + "'.");
	}

	SemanticAnalyser::SemanticAnalyser() {
		if (DEBUG) {
			std::cout << std::endl;
			check("________________________________________________________________________________________________-");
			check("________________________________________________________________________________________________-");
			std::cout << std::endl;
			std::cout << std::endl;
			check("****SEMANTIC ANALYSIS ****\n\n");
		}
		_scopes.push_back(new SemanticScope());
	}
	SemanticAnalyser::SemanticAnalyser(SemanticScope* global_scope) {
		// Add global scope
		_scopes.push_back(global_scope);
	}



	void SemanticAnalyser::visit(parser::ASTProgramNode* progNode) {
		for (auto statement : progNode->_statements) {
			statement->accept(this);
		}

	}

	void SemanticAnalyser::visit(parser::ASTDeclarationNode* declaration) {
		SemanticScope* currentScope = _scopes.back();
		////checkPurpose
		//std::cout << std::boolalpha << "identifier = "+ declaration->_identifier << std::endl;
		//std::cout <<std::boolalpha<< currentScope->alreadyDeclared(declaration->_identifier) << std::endl;

		if (currentScope->alreadyDeclared(declaration->_identifier)) {
			throw std::runtime_error(declaration->_identifier + " again declared in line " +
				std::to_string(declaration->_lineNumber));
		}
		declaration->_expression->accept(this);
		//checkPurpose
		/*std::cout << static_cast<int>(declaration->_type) << std::endl;
		std::cout << static_cast<int>(_currentExpressionType) << std::endl;*/

		if (declaration->_type == parser::TYPE::REAL && _currentExpressionType == parser::TYPE::INT)
		{
			currentScope->declare(declaration->_identifier, parser::TYPE::REAL, declaration->_lineNumber);

		}
		else if (declaration->_type == _currentExpressionType)
		{
			currentScope->declare(declaration->_identifier, declaration->_type, declaration->_lineNumber);
		}
		else {
			std::cout << "inside type check condition" << std::endl;
			throw std::runtime_error("type mismatch at line " + std::to_string(declaration->_lineNumber));
		}

		check("variable declaration completed with semantic analysis");
		check("________________________________________________________________________________________________-");
		check("________________________________________________________________________________________________-");

	}


	void SemanticAnalyser::visit(parser::ASTIdentifierNode* id) {
		int  i;
		check("identifier node visit ");
		for (i = _scopes.size() - 1; !_scopes[i]->alreadyDeclared(id->getIdentifier()); i--) {
			if (i <= 0)
			{
				throw std::runtime_error("Identifier '" + id->getIdentifier() + "' appearing on line " +
					std::to_string(id->getLineNumber()) + " was never declared " +
					((_scopes.size() == 1) ? "globally." : "in this scope."));
			}
		}
		_currentExpressionType = _scopes[i]->getType(id->getIdentifier());
	}

	void SemanticAnalyser::visit(parser::ASTBinaryExprNode* binaryNode) {
		std::string op = binaryNode->_optor;

		binaryNode->_expr1->accept(this);
		parser::TYPE typeFirst = _currentExpressionType;
		binaryNode->_expr2->accept(this);
		parser::TYPE typeSecond = _currentExpressionType;


		/*if (op == "+")
			std::cout << "true<<<<<<<<" << std::endl;
		else
			std::cout << "false<<<<<<<<" << std::endl;*/

			// these three operations are defined only if data types in binary expression are real or integer
		if (op == "-" || op == "/" || op == "*") {
			if ((typeFirst != parser::TYPE::INT && typeFirst != parser::TYPE::REAL) ||
				(typeSecond != parser::TYPE::INT && typeSecond != parser::TYPE::REAL))
				throw std::runtime_error("Expected numerical operands for '" + op +
					"' operator on line " + std::to_string(binaryNode->_lineNumber) + ".");

			_currentExpressionType = (typeFirst == parser::TYPE::INT && typeSecond == parser::TYPE::INT) ?
				parser::TYPE::INT : parser::TYPE::REAL;

		}
		else if (op == "+")
		{
			if (typeFirst == parser::TYPE::BOOL || typeSecond == parser::TYPE::BOOL)
				throw std::runtime_error("Invalid operand for '+' operator, expected numerical or string"
					" operand on line " + std::to_string(binaryNode->_lineNumber) + ".");

			if (typeFirst == parser::TYPE::STRING && typeSecond == parser::TYPE::STRING)
				_currentExpressionType = parser::TYPE::STRING;

			// only one is string, error
			else if (typeFirst == parser::TYPE::STRING || typeSecond == parser::TYPE::STRING)
				throw std::runtime_error("Mismatched operands for '+' operator, found " + inString(typeFirst) +
					" on the left, but " + inString(typeSecond) + " on the right (line " +
					std::to_string(binaryNode->_lineNumber) + ").");

			// real/int possibilities remain. If both int, then result is int, otherwise result is real
			else
				_currentExpressionType = (typeFirst == parser::TYPE::INT && typeSecond == parser::TYPE::INT) ?
				parser::TYPE::INT : parser::TYPE::REAL;
		}
		//and and or operator are only for bool type
		else if (op == "and" || op == "or") {
			if (typeFirst == parser::TYPE::BOOL && typeSecond == parser::TYPE::BOOL)
				_currentExpressionType = parser::TYPE::BOOL;
			else throw std::runtime_error("Expected two boolean-type operands for '" + op + "' operator " +
				"on line " + std::to_string(binaryNode->_lineNumber) + ".");
		}


		else if (op == "<" || op == ">" || op == "<=" || op == ">=") {
			if ((typeFirst != parser::TYPE::REAL && typeFirst != parser::TYPE::INT) ||
				(typeSecond != parser::TYPE::REAL && typeSecond != parser::TYPE::INT))
				throw std::runtime_error("Expected two numerical operands for '" + op + "' operator " +
					"on line " + std::to_string(binaryNode->_lineNumber) + ".");
			_currentExpressionType = parser::TYPE::BOOL;
		}
		else if (op == "==" || op == "!=") {
			if (typeFirst != typeSecond && (typeFirst != parser::TYPE::REAL || typeSecond != parser::TYPE::INT) &&
				(typeFirst != parser::TYPE::INT || typeSecond != parser::TYPE::REAL))
				throw std::runtime_error("Expected arguments of the same type '" + op + "' operator " +
					"on line " + std::to_string(binaryNode->_lineNumber) + ".");
			_currentExpressionType = parser::TYPE::BOOL;
		}
		else
			throw std::runtime_error("error during semantic analysis of binary operator on  line "
				+ std::to_string(binaryNode->_lineNumber) + ".");

	}

	void SemanticAnalyser::visit(parser::ASTUnaryExprNode* unaryNode) {
		std::string op = unaryNode->_optor;
		unaryNode->_expr->accept(this);
		parser::TYPE type = _currentExpressionType;

		if (op == "+" || op == "-") {
			if (type != parser::TYPE::INT and type != parser::TYPE::REAL) {
				throw std::runtime_error("error occured in unary operation int or real required in line" +
					std::to_string(unaryNode->_lineNumber));
			}
			_currentExpressionType = type;
		}
		if (op == "not") {
			if (type != parser::TYPE::BOOL) {
				throw std::runtime_error("error occured in unary operation in line:: bool ecpected" +
					std::to_string(unaryNode->_lineNumber));
			}
			_currentExpressionType = parser::TYPE::BOOL;
		}

	}



	void SemanticAnalyser::visit(parser::ASTFunctionCallNode* func) {
		//checkPurpose
		//std::cout << std::endl;
		//std::cout << func->_lineNumber;
		// Determine the signature of the function
		std::vector<parser::TYPE> signature;

		//checkPurpose

		//std::cout << "\n\n****************started checking func*************\n\n" << std::endl;
		// For each parameter,
		for (auto param : func->_parameters) {
			// visit to update current expr type
			param->accept(this);

			// add the type of current expr to signature
			//std::cout << "\n\n current expression type  " + inString(_currentExpressionType) << std::endl;
			signature.push_back(_currentExpressionType);
		}
		//checkPurpose
		//std::cout << "for the fnction =" << func->getIdentifier() << std::endl;
		if (DEBUG) {
			for (auto param : signature) {
				std::cout << inString(param) << std::endl;
			}
		}
		////checkPurpose
		//std::cout << "\n\n\n\n\n\n\nparammeters of called function " << std::endl;
		//for (auto params : signature)
		//	std::cout <<  inString(params)+" ,";
		//checkPurpose

		/*std::cout << " \n size of _scope = " << _scopes.size()<<std::endl;*/

		//// checkPurpose
		//std::cout << " printing details inside the scope variable " << std::endl;
		//for (auto param : _scopes) {
		//	std::cout << "first loop " << std::endl;
		//	for (auto param1 : param->functionSymbolTable)
		//	{
		//		std::cout <<"first of the functionSymbol table ="+ param1.first << std::endl;
		//	}
		//}
		//check("\n\n\n\n");

		// Make sure the function exists in some scope i
		unsigned long i;
		for (i = _scopes.size() - 1;
			!_scopes[i]->alreadyDeclared(func->_identifier, signature);
			i--)
			if (i <= 0) {
				std::string func_name = func->_identifier + "(  ";
				bool has_params = false;
				for (auto param : signature) {
					has_params = true;
					func_name += inString(param) + ", ";
				}
				func_name.pop_back();   // remove last whitespace
				func_name.pop_back();   // remove last comma
				func_name += ")";
				throw std::runtime_error("Function '" + func_name + "' appearing on line " +
					std::to_string(func->_lineNumber) + " was never declared " +
					((_scopes.size() == 1) ? "globally." : "in this scope."));
			}

		// Set current expression type to the return value of the function
		_currentExpressionType = _scopes[i]->getType(func->_identifier, std::move(signature));
	}


	void SemanticAnalyser::visit(parser::ASTAssignmentNode* assign) {

		assign->_expression->accept(this);
		parser::TYPE rhsType = _currentExpressionType;
		//SemanticScope *currentScope = _scopes.back();
		int i;
		for (i = _scopes.size() - 1; !_scopes[i]->alreadyDeclared(assign->_identifier); i--) {
			if (i <= 0)
			{
				throw std::runtime_error("Identifier '" + assign->_identifier + "' appearing on line " +
					std::to_string(assign->_lineNumber) + " was never declared " +
					((_scopes.size() == 1) ? "globally." : "in this scope."));
			}
		}
		//alow int to float
		if (_scopes[i]->getType(assign->_identifier) == parser::TYPE::REAL and rhsType == parser::TYPE::INT)
		{
		}
		else if (_scopes[i]->getType(assign->_identifier) == rhsType) {}
		else
		{
			throw std::runtime_error("type mismatch appearing on line " +
				std::to_string(assign->_lineNumber));
		}
	}

	void SemanticAnalyser::visit(parser::ASTPrintNode* print) {

		// Update current expression
		print->_expression->accept(this);
	}


	void SemanticAnalyser::visit(parser::ASTLiteralNode<int>* declaration) {
		_currentExpressionType = parser::TYPE::INT;
	}
	void SemanticAnalyser::visit(parser::ASTLiteralNode<float>* declaration) {
		//checkPurpose
		//std::cout << "checking the func parameters in the float node" << std::endl;
		_currentExpressionType = parser::TYPE::REAL;
	}
	void SemanticAnalyser::visit(parser::ASTLiteralNode<bool>* declaration) {
		_currentExpressionType = parser::TYPE::BOOL;
	}
	void SemanticAnalyser::visit(parser::ASTLiteralNode<std::string>* declaration) {
		_currentExpressionType = parser::TYPE::STRING;
	}

	std::string inString(parser::TYPE t) {

		switch (t) {
		case parser::TYPE::INT:
			return "int";
		case parser::TYPE::REAL:
			return "real";
		case parser::TYPE::BOOL:
			return "bool";
		case parser::TYPE::STRING:
			return "string";
		default:
			throw std::runtime_error("Invalid type encountered.");
		}
	}

	//this function checks whether the function returns or not
	bool SemanticAnalyser::returns(parser::ASTStatementNode* statements) {
		//during recursion when the argument of this returns function is return statement it self 
		//it must return true
		if (dynamic_cast<parser::ASTReturnNode*>(statements)) {
			return true;
		}
		// for the first input case
		//if the input is block check if it returns looping through whole statements in block
		if (auto stmnt = dynamic_cast<parser::ASTBlockNode*>(statements)) {
			for (auto statement : stmnt->_statements) {
				return(statement);
			}
		}

		if (auto ifStatement = dynamic_cast<parser::ASTIfNode*>(statements)) {
			if (ifStatement->_elseBlock) {
				return (returns(ifStatement->_ifBlock) && returns(ifStatement->_elseBlock));
			}
			//this may have to be removed
			//returns true if only contains if block and returns
			return returns(ifStatement->_ifBlock);

		}
		if (auto whilestmt = dynamic_cast<parser::ASTWhileNode*>(statements))
			return returns(whilestmt->_whileBlock);

		else
			return false;


	}

	void SemanticAnalyser::visit(parser::ASTReturnNode* ret) {

		//visit current expression for semantics check
		ret->_expr->accept(this);


		//TODO check this condition i dont think  it will yield good result
		if (!functions.empty() && functions.top() != _currentExpressionType) {
			throw std::runtime_error("Invalid return type on line " + std::to_string(ret->_lineNumber) +
				". Expected " + inString(functions.top()) + ", found " +
				inString(_currentExpressionType) + ".");
		}
	}

	void SemanticAnalyser::visit(parser::ASTBlockNode* block) {
		_scopes.push_back(new SemanticScope());

		for (auto param : currentFunctionParameters) {
			_scopes.back()->declare(param.first, param.second, block->_lineNumber);
		}

		currentFunctionParameters.clear();
		for (auto statement : block->_statements)
			statement->accept(this);
		_scopes.pop_back();
	}

	void SemanticAnalyser::visit(parser::ASTFunctionDefinitionNode* func) {
		//function are global i think


		for (auto& scope : _scopes) {

			if (scope->alreadyDeclared(func->_funcName, func->_signature)) {

				// Determine line number of error and the corresponding function signature
				int line = scope->getDeclarationLine(func->_funcName, func->_signature);
				std::string signature = "(  ";
				//bool has_params = false;
				for (auto param : func->_signature) {
					//has_params = true;
					signature += inString(param) + ", ";
				}
				signature.pop_back();   // remove last whitespace
				signature.pop_back();   // remove last comma
				signature += ")";


				throw std::runtime_error("Error on line " + std::to_string(func->_lineNumber) +
					". Function " + func->_funcName + signature +
					" already defined on line " + std::to_string(line) + ".");
			}
		}

		// Add function to symbol table
		_scopes.back()->declare(func->_funcName, func->_returnType, func->_signature, func->_lineNumber);

		////checkPurpose
		//check("Printing functionSymbol table after declaration \n");
		//check(" size of the function symbol table=" +_scopes[0]->functionSymbolTable.size());
		//for (auto param : _scopes) {
		//	std::cout << "first loop " << std::endl;
		//	for (auto param1 : param->functionSymbolTable)
		//	{
		//		std::cout << "first of the functionSymbol table =" + param1.first << std::endl;
		//	}
		//}

		// Push current function type onto function stack
		functions.push(func->_returnType);

		// Empty and update current function parameters vector
		currentFunctionParameters.clear();
		currentFunctionParameters = func->_parameters;

		// Check semantics of function block by visiting nodes
		func->_funcDefBlock->accept(this);

		// Check that the function body returns
		if (!returns(func->_funcDefBlock))
			throw std::runtime_error("Function " + func->_funcName + " defined on line " +
				std::to_string(func->_lineNumber) + " is not guaranteed to " +
				"return a value.");

		// End the current function
		functions.pop();
		check("function definition semantically analysed sucessfully");
	}

	void SemanticAnalyser::visit(parser::ASTIfNode* ifnode) {

		// Set current type to while expression
		ifnode->_condition->accept(this);

		// Make sure it is boolean
		if (_currentExpressionType != parser::TYPE::BOOL)
			throw std::runtime_error("Invalid if-condition on line " + std::to_string(ifnode->_lineNumber)
				+ ", expected boolean expression.");

		// Check the if block
		ifnode->_ifBlock->accept(this);

		// If there is an else block, check it too
		if (ifnode->_elseBlock)
			ifnode->_elseBlock->accept(this);

	}
	void SemanticAnalyser::visit(parser::ASTWhileNode* whileStatement)
	{
		whileStatement->_condition->accept(this);
		if (_currentExpressionType != parser::TYPE::BOOL) {
			throw std::runtime_error("Invalid while-condition on line " + std::to_string(whileStatement->_lineNumber)
				+ ", expected boolean expression.");
		}
		whileStatement->_whileBlock->accept(this);
	}
}