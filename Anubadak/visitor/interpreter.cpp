
#include "interpreter.h"

void check(std::string someString) {
	if (DEBUG)
		std::cout << someString << std::endl;
}

namespace visitor
{


	bool InterpreterScope::alreadyDeclared(std::string identifier) {
		return variableSymbolTable.find(identifier) != variableSymbolTable.end();
	}
	parser::TYPE InterpreterScope::getTypeOf(std::string identifier) {
		return variableSymbolTable[identifier].first;
	}
	value_t InterpreterScope::getValueOf(std::string identifier) {
		return variableSymbolTable[identifier].second;

	}

	std::vector<std::string> InterpreterScope::getVariablesNamesOf(std::string funcName,
		std::vector<parser::TYPE>signature) {

		auto range = functionSymbolTable.equal_range(funcName);

		for (auto i = range.first; i != range.second; i++) {
			if (std::get<0>(i->second) == signature)
			{
				return std::get<1>(i->second);
			}
		}
	}
	void InterpreterScope::declare(std::string identifier, int n) {
		value_t v;
		v.i = n;
		variableSymbolTable[identifier] = std::make_pair(parser::TYPE::INT, v);
	}
	void InterpreterScope::declare(std::string identifier, float floatValue) {
		value_t v;
		v.f = floatValue;
		variableSymbolTable[identifier] = std::make_pair(parser::TYPE::REAL, v);
	}
	void InterpreterScope::declare(std::string identifier, bool boolValue) {
		value_t v;
		v.b = boolValue;
		variableSymbolTable[identifier] = std::make_pair(parser::TYPE::BOOL, v);
	}
	void InterpreterScope::declare(std::string identifier, std::string s) {
		value_t v;
		v.s = s;
		variableSymbolTable[identifier] = std::make_pair(parser::TYPE::STRING, v);
	}
	void InterpreterScope::declare(std::string identifier, std::vector<parser::TYPE>signature,
		std::vector<std::string>variableNames, parser::ASTBlockNode* block) {

		functionSymbolTable.insert(std::make_pair(identifier,
			std::make_tuple(signature, variableNames, block)));

	}


	parser::ASTBlockNode* InterpreterScope::getBlockOf(std::string funcName,
		std::vector<parser::TYPE>signature) {

		auto range = functionSymbolTable.equal_range(funcName);

		for (auto i = range.first; i != range.second; i++) {
			if (std::get<0>(i->second) == signature)
			{
				return std::get<2>(i->second);
			}
		}
		return nullptr;
	}

	bool InterpreterScope::alreadyDeclared(std::string identifier,
		std::vector<parser::TYPE>signature) {

		auto range = functionSymbolTable.equal_range(identifier);
		for (auto i = range.first; i != range.second; i++) {

			if (std::get<0>(i->second) == signature)
			{
				return true;
			}
		}
		return false;

	}

	Interpreter::Interpreter() {
		check("________________________________________________________________________________________________-");
		check("________________________________________________________________________________________________-");
		std::cout << std::endl;
		check("****INTERPRETER STARTED ****\n\n");
		console = new wxFrame((wxFrame*)NULL, CONSOLE, "Output", wxPoint(100, 100), wxSize(700, 600));
		console->Show(true);
		output = new wxTextCtrl(console, OUTPUT,
			"", wxDefaultPosition, console->GetSize(),
			wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
		output->SetBackgroundColour(*wxBLACK);
		output->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_BOLD, false));
		output->SetForegroundColour(*wxWHITE);

		_scopes.push_back(new InterpreterScope());
	}
	Interpreter::Interpreter(InterpreterScope* globalScope) {
		_scopes.push_back(globalScope);
	}

	void Interpreter::visit(parser::ASTProgramNode* prog) {
		for (auto statement : prog->_statements) {
			statement->accept(this);
		}
	}
	void Interpreter::visit(parser::ASTDeclarationNode* declaration) {

		declaration->_expression->accept(this);

		switch (declaration->_type) {
		case parser::TYPE::INT:
			_scopes.back()->declare(declaration->_identifier,
				_currentExpressionValue.i);
			break;
		case parser::TYPE::REAL:
			if (_currentExpressionType == parser::TYPE::INT)
				_scopes.back()->declare(declaration->_identifier,
				(float)_currentExpressionValue.i);
			else
				_scopes.back()->declare(declaration->_identifier,
					_currentExpressionValue.f);
			break;
		case parser::TYPE::BOOL:
			_scopes.back()->declare(declaration->_identifier,
				_currentExpressionValue.b);
			break;
		case parser::TYPE::STRING:
			_scopes.back()->declare(declaration->_identifier,
				_currentExpressionValue.s);
			break;
		}
		// check("declaration sucessfully interpreted");

	}
	void Interpreter::visit(parser::ASTAssignmentNode* assignment) {
		int i;
		int count = 1;
		for (i = _scopes.size() - 1; !_scopes[i]->alreadyDeclared(assignment->_identifier); i--)
		{
		};
		assignment->_expression->accept(this);

		switch (_scopes[i]->getTypeOf(assignment->_identifier)) {
		case parser::TYPE::INT: {

			_scopes[i]->declare(assignment->_identifier,
				_currentExpressionValue.i);
			break;
		}
		case parser::TYPE::REAL:
			if (_currentExpressionType == parser::TYPE::INT)
				_scopes[i]->declare(assignment->_identifier,
				(float)_currentExpressionValue.i);
			else
				_scopes[i]->declare(assignment->_identifier,
					_currentExpressionValue.f);
			break;
		case parser::TYPE::BOOL:
			_scopes[i]->declare(assignment->_identifier,
				_currentExpressionValue.b);
			break;
		case parser::TYPE::STRING:
			_scopes[i]->declare(assignment->_identifier,
				_currentExpressionValue.s);
			break;
		}
	}

	void Interpreter::visit(parser::ASTPrintNode* print) {
		// Visit expression node to update current value/type
		print->_expression->accept(this);
		// check("in the print");

		 // Print, depending on type
		switch (_currentExpressionType) {
		case parser::TYPE::INT:
			output->AppendText(std::to_string(_currentExpressionValue.i));
			//std::cout << _currentExpressionValue.i;
			break;
		case parser::TYPE::REAL:
			output->AppendText(std::to_string(_currentExpressionValue.f));
			//std::cout << _currentExpressionValue.f;
			break;
		case parser::TYPE::BOOL:
			output->AppendText((_currentExpressionValue.b) ? "true" : "false");
			//std::cout << ((_currentExpressionValue.b) ? "true" : "false");
			break;
		case parser::TYPE::STRING:
			output->AppendText(_currentExpressionValue.s);
			//std::cout << _currentExpressionValue.s;
			break;
		}
	}
	void Interpreter::visit(parser::ASTReturnNode* ret) {
		ret->_expr->accept(this);
	}


	void Interpreter::visit(parser::ASTBlockNode* block) {
		_scopes.push_back(new InterpreterScope());
		for (int i = 0; i < _currentFunctionArguments.size(); i++)
		{
			switch (_currentFunctionArguments[i].first)
			{

			case parser::TYPE::INT:
				_scopes.back()->declare(_currentFunctionParameters[i],
					_currentFunctionArguments[i].second.i);
				break;
			case parser::TYPE::REAL:
				_scopes.back()->declare(_currentFunctionParameters[i],
					_currentFunctionArguments[i].second.f);
				break;
			case parser::TYPE::BOOL:
				_scopes.back()->declare(_currentFunctionParameters[i],
					_currentFunctionArguments[i].second.b);
				break;
			case parser::TYPE::STRING:
				_scopes.back()->declare(_currentFunctionParameters[i],
					_currentFunctionArguments[i].second.s);
				break;
			}

		}
		// Clear the global function parameter/argument vectors
		_currentFunctionParameters.clear();
		_currentFunctionArguments.clear();

		// Visit each statement in the block
		for (auto& stmt : block->_statements)
			stmt->accept(this);

		// Close scope
		_scopes.pop_back();
	}

	//template class derived from the ASTExpr node
	void Interpreter::visit(parser::ASTLiteralNode<int>* lit) {
		value_t v;
		v.i = lit->_value;
		_currentExpressionType = parser::TYPE::INT;
		_currentExpressionValue = std::move(v);

	}
	void Interpreter::visit(parser::ASTLiteralNode<float>* lit) {
		value_t v;
		v.f = lit->_value;
		_currentExpressionType = parser::TYPE::REAL;
		_currentExpressionValue = std::move(v);
		//std::cout << " hello there i m float literal" << std::endl;
	}
	void Interpreter::visit(parser::ASTLiteralNode<bool>* lit) {
		value_t v;
		v.b = lit->_value;
		_currentExpressionType = parser::TYPE::BOOL;
		_currentExpressionValue = std::move(v);
		//std::cout << " hello there i m bool literal" << std::endl;
	}
	void Interpreter::visit(parser::ASTLiteralNode<std::string>* lit) {
		value_t v;
		v.s = lit->_value;
		_currentExpressionType = parser::TYPE::STRING;
		_currentExpressionValue = std::move(v);
		//std::cout << " hello there i m string literal" << std::endl;
	}

	void Interpreter::visit(parser::ASTIdentifierNode* identifier) {
		int i;
		for (i = _scopes.size() - 1; !_scopes[i]->alreadyDeclared(identifier->getIdentifier()); i--);

		_currentExpressionType = _scopes[i]->getTypeOf(identifier->getIdentifier());
		_currentExpressionValue = _scopes[i]->getValueOf(identifier->getIdentifier());
	}
	void Interpreter::visit(parser::ASTIfNode* ifNode) {
		ifNode->_condition->accept(this);
		if (_currentExpressionValue.b)
		{
			ifNode->_ifBlock->accept(this);
		}
		else
		{
			if (ifNode->_elseBlock)
				ifNode->_elseBlock->accept(this);
		}
	}

	void Interpreter::visit(parser::ASTWhileNode* whileNode) {
		// check("in the begining of while");
		whileNode->_condition->accept(this);
		//std::cout << std::boolalpha << _currentExpressionValue.b << std::endl;	
		while (_currentExpressionValue.b) {
			whileNode->_whileBlock->accept(this);
			//agian check th econdition 
			whileNode->_condition->accept(this);
		}
	}
	void Interpreter::visit(parser::ASTFunctionDefinitionNode* fn) {
		_scopes.back()->declare(fn->_funcName, fn->_signature, fn->_variableNames,
			fn->_funcDefBlock);
	}
	void Interpreter::visit(parser::ASTBinaryExprNode* binaryStatement) {
		binaryStatement->_expr1->accept(this);
		auto firstExpType = _currentExpressionType;
		auto firstExpVal = _currentExpressionValue;

		binaryStatement->_expr2->accept(this);
		auto secExpType = _currentExpressionType;
		auto secExpVal = _currentExpressionValue;

		auto op = binaryStatement->_optor;

		value_t v;
		if (op == "+" or op == "-" or op == "/" or op == "*") {

			if (firstExpType == parser::TYPE::INT && secExpType == parser::TYPE::INT) {
				_currentExpressionType = parser::TYPE::INT;
				if (op == "+")
				{
					v.i = firstExpVal.i + secExpVal.i;
				}
				else if (op == "-")
					v.i = firstExpVal.i - secExpVal.i;
				else if (op == "*")
					v.i = firstExpVal.i * secExpVal.i;
				else if (op == "/") {
					if (secExpVal.i == 0) {

						throw std::runtime_error("Division by zero encountered on line "
							+ std::to_string(binaryStatement->_lineNumber) + ".");
					}
					v.i = firstExpVal.i / secExpVal.i;
				}
			}
			//if any one of them is float
			else if (firstExpType == parser::TYPE::REAL or secExpType == parser::TYPE::REAL) {
				_currentExpressionType = parser::TYPE::REAL;
				float l = firstExpVal.f;
				float r = secExpVal.f;

				if (firstExpType == parser::TYPE::INT)
					l = firstExpVal.i;
				if (secExpType == parser::TYPE::INT)
					r = secExpVal.i;

				if (op == "+")
				{
					v.f = l + r;
				}
				else if (op == "-")
					v.f = l - r;
				else if (op == "*")
					v.f = l * r;
				else if (op == "/") {
					if (secExpVal.f == 0 && secExpVal.i == 0) {
						std::cout << "ho raixa................................. " << std::endl;
						throw std::runtime_error("Division by zero encountered on line "
							+ std::to_string(binaryStatement->_lineNumber) + ".");
					}

					v.f = l / r;
				}
				// Remaining case is for strings
			}
			else {

				_currentExpressionType = parser::TYPE::STRING;
				v.s = firstExpVal.s + secExpVal.s;
			}
		}
		// Now bool
		else if (op == "and" || op == "or") {
			_currentExpressionType = parser::TYPE::BOOL;
			if (op == "and")
				v.b = firstExpVal.b && secExpVal.b;
			else if (op == "or")
				v.b = firstExpVal.b || secExpVal.b;
		}
		else {
			_currentExpressionType = parser::TYPE::BOOL;
			if (firstExpType == parser::TYPE::BOOL)
				v.b = (op == "==") ? firstExpVal.b == secExpVal.b : firstExpVal.b != secExpVal.b;

			else if (firstExpType == parser::TYPE::STRING)
				v.b = (op == "==") ? firstExpVal.s == secExpVal.s : firstExpVal.s != secExpVal.s;

			else {
				float l = firstExpVal.f, r = secExpVal.f;
				if (firstExpType == parser::TYPE::INT)
					l = firstExpVal.i;
				if (secExpType == parser::TYPE::INT)
					r = secExpVal.i;
				if (op == "==")
					v.b = l == r;
				else if (op == "!=")
					v.b = l != r;
				else if (op == "<") {
					//check("conditiono <");
					v.b = l < r;
				}

				else if (op == ">")
					v.b = l > r;
				else if (op == ">=")
					v.b = l >= r;
				else if (op == "<=")
					v.b = l <= r;
			}
		}
		//std::cout << "at the end of binary expression value of v is=" << v.i<<std::endl;
		_currentExpressionValue = v;
	}

	void Interpreter::visit(parser::ASTUnaryExprNode* un) {
		un->_expr->accept(this);

		switch (_currentExpressionType) {
		case parser::TYPE::INT:
			if (un->_optor == "-")
				_currentExpressionValue.i *= -1;
			break;
		case parser::TYPE::REAL:
			if (un->_optor == "-")
				_currentExpressionValue.f *= -1;
			break;
		case parser::TYPE::BOOL:
			_currentExpressionValue.b ^= 1;
		}
	}
	void Interpreter::visit(parser::ASTFunctionCallNode* func) {
		//_parameters inside thos functionCallNode is vector of expression

		std::vector < parser::TYPE > signature;
		for (auto param : func->_parameters) {
			param->accept(this);
			//after acception the gloabal class variable
			//_currentExpressionType and CurretnExpressionValue are changed or set
			//to different values
			signature.push_back(_currentExpressionType);
			//emplace_back is same like push_back but little faster
			//because it removes expensive copy of the objects
			_currentFunctionArguments.emplace_back(_currentExpressionType, _currentExpressionValue);

		}
		//this int i is declared here but not inside for loop because it need
		//to be used outside the loop too

		int i;
		for (i = _scopes.size() - 1; !_scopes[i]->alreadyDeclared(func->_identifier,
			signature); i--);
		_currentFunctionParameters = _scopes[i]->getVariablesNamesOf(func->_identifier,
			signature);

		//visit the correspondig block
		_scopes[i]->getBlockOf(func->_identifier, signature)->accept(this);
	}

}