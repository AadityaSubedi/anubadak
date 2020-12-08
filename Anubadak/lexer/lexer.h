#pragma once
#include <iostream>
#include <vector>

#include "token.h"

namespace lexer
{
	enum class TRANSITION_TYPE {
		DIGIT = 0,
		PERIOD = 1,
		ADDITIVE_OP = 2,
		ASTERISK = 3,
		EXCL_MARK = 4,
		ORDER_REL = 5,
		EQUALS = 6,
		UNDERSCORE = 7,
		FORWARDSLASH = 8,
		BACKSLASH = 9,
		QUOTATION_MARK = 10,
		PUNCTUATION = 11,
		NEWLINE = 12,
		ENDOFFILE = 13,
		LETTER = 14,
		PRINTABLE = 15,
		OTHER = 16
	};


	class Lexer
	{
	public:
		///Lexer class constructor  takes string form of whole program as a input 
		Lexer(std::string& program,int currentTokenNumber=0);

		///this method is used by the parser to move through the tokens vectors 
		Token nextToken();
		std::vector<Token> tokens;

	private:
		const int e = 23;


		/**
			 * Encodes the transition function of the DFA.
			 * The value of <tt>transitions[i][j]</tt> is the state to go to from
			 * S<SUB>j</SUB> when receiving input i, that is, <tt>transitions[i][j]</tt>
			 * \f$=\delta(j, i)\f$.
			 * @see Lexer::transition_delta(int, char)
			 */
		const bool isFinal[24] = { 0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  0,  0,  1,  0,  1,  0,  0,  0,  1,  1,  1, 0 };
		const int transitions[17][23] = {
			                      /* S0  S1  S2  S3  S4  S5  S6  S7  S8  S9 S10 S11 S12 S13 S14 S15 S16 S17 S18 S19 S20 S21 S22 */
			/* DIGIT          */ {   1,  1,  3,  3,  e,  e,  e,  e,  e,  e, 10,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* PERIOD         */ {   2,  3,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* ADDITIVE_OP    */ {   4,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* ASTERISK       */ {   5,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 13, 12, 15,  e, 15,  e, 17, 17, 17,  e,  e,  e},
			/* EXCL_MARK      */ {   6,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* ORDER_REL      */ {   7,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* EQUALS         */ {   8,  e,  e,  e,  e,  e,  9,  9,  9,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* UNDERSCORE     */ {  10,  e,  e,  e,  e,  e,  e,  e,  e,  e, 10,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* FORWARDSLASH   */ {  11,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 12, 13,  e, 16,  e, 17, 17, 17,  e,  e,  e},
			/* BACKSLASH      */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 18, 18, 18,  e,  e,  e},
			/* QUOTATION_MARK */ {  17,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 20, 19, 20,  e,  e,  e},
			/* PUNCTUATION    */ {  21,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* NEWLINE        */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 14, 13,  e, 13,  e,  e,  e,  e,  e,  e,  e},
			/* ENDOFFILE      */ {  22,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e},
			/* LETTER         */ {  10,  e,  e,  e,  e,  e,  e,  e,  e,  e, 10,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* PRINTABLE      */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
			/* OTHER          */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e}
		};


		///provided program and the current index this member function generate one token and return and also update current index
		Token generateToken(std::string& program, int& currentIndex);

		/// given program and current index this method calculate the line number by couunting \n in the program
		int getLineNumber(std::string& program, int current_index);
		///this function behaves as the transition function of the deterministic finite state automata
		int transitionFunction(int current_state, char symbol);


		///to keep track of how far in the tokens vetor we have reached
		int _currentTokenNumber;


	};
}

