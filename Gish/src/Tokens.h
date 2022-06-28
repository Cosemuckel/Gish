 #pragma once

TokenType TT_PLUS	("OP", "PLUS");
TokenType TT_MINUS	("OP", "MINUS");
TokenType TT_MULT	("OP", "MULT");
TokenType TT_DIV	("OP", "DIV");
TokenType TT_FAC	("OP", "FAC");
TokenType TT_POW	("OP", "POW");

TokenType TT_EQUALS ("OP", "EQUALS");

TokenType TT_ADD	  ("OPR", "ADD");
TokenType TT_SUB      ("OPR", "SUB");
TokenType TT_MULTIPLY ("OPR", "MULT");
TokenType TT_DIVIDE   ("OPR", "DIV");
TokenType TT_POWER    ("OPR", "POW");
TokenType TT_SQUARE   ("OPR", "SQR");
TokenType TT_CUBE     ("OPR", "CUBE");

TokenType TT_SQUARED ("OP", "SQRD");
TokenType TT_CUBED   ("OP", "CUBED");

TokenType TT_AND ("KEY", "AND");
TokenType TT_OR  ("KEY", "OR");

TokenType TT_GREAT    ("CMP", "GREAT");
TokenType TT_SMALL    ("CMP", "SMALL");
TokenType TT_GREAT_EQ ("CMP", "GREAT_EQ");
TokenType TT_SMALL_EQ ("CMP", "SMALL_EQ");
TokenType TT_NOT      ("CMP", "NOT");

// TokenType TT_IS   ("HELP", "IS");
// TokenType TT_THAN ("HELP", "THAN");
// TokenType TT_TO   ("HELP", "TO");
// TokenType TT_BY   ("HELP", "BY");
// TokenType TT_OF   ("HELP", "OF");
// TokenType TT_FROM ("HELP", "FROM");
// TokenType TT_THE  ("HELP", "THE");
// TokenType TT_TYPE ("HELP", "TYPE");
// TokenType TT_WITH ("HELP", "WITH");
// TokenType TT_A    ("HELP", "A");
// TokenType TT_AS   ("HELP", "AS");
// TokenType TT_LONG ("HELP", "LONG");

TokenType TT_WORD ("WORD", "WORD");

TokenType TT_KEYWORD_INDEX        ("KEY", "INDEX");
TokenType TT_KEYWORD_CHARACTER    ("KEY", "CHARACTER");
TokenType TT_KEYWORD_SIZEOF       ("KEY", "SIZEOF");
TokenType TT_KEYWORD_ITERATIONS   ("KEY", "ITERATIONS");

TokenType TT_KEYWORD_NEWLINE ("KEY", "NEWLINE");
TokenType TT_KEYWORD_CONSOLE ("KEY", "CONSOLE");
TokenType TT_KEYWORD_INPUT   ("KEY", "INPUT");

TokenType TT_KEYWORD_RETURN ("KEY", "RETURN");

TokenType TT_PRINT   ("USR", "PRINT");
TokenType TT_SCAN    ("USR", "SCAN");
TokenType TT_EXECUTE ("USR", "EXECUTE");
TokenType TT_COMMAND ("USR", "COMMAND");

TokenType TT_KEYWORD_IF      ("KEY", "IF");
TokenType TT_KEYWORD_ELSE    ("KEY", "ELSE");
TokenType TT_KEYWORD_FOR     ("KEY", "FOR");
TokenType TT_KEYWORD_WHILE   ("KEY", "WHILE");
TokenType TT_KEYWORD_DO      ("KEY", "DO");
TokenType TT_KEYWORD_TIMES   ("KEY", "TIMES");
TokenType TT_KEYWORD_SECONDS ("KEY", "SECONDS");

TokenType TT_KEYWORD_TYPEOF   ("KEY", "TYPEOF");
TokenType TT_KEYWORD_UNDEFINE ("KEY", "UNDEF");
TokenType TT_KEYWORD_VARIABLE ("KEY", "VARIBALE");
TokenType TT_KEYWORD_FUNCTION ("KEY", "FUNCTION");

TokenType TT_KEYWORD_CONTINUE ("KEY", "CONTINUE");
TokenType TT_KEYWORD_BREAK    ("KEY", "BREAK");

TokenType TT_KEYWORD_STRING  ("KEY", "STRING");
TokenType TT_KEYWORD_NUMBER  ("KEY", "NUMBER");
TokenType TT_KEYWORD_BOOLEAN ("KEY", "BOOLEAN");
TokenType TT_KEYWORD_ARRAY   ("KEY", "ARRAY");
TokenType TT_KEYWORD_VOID    ("KEY", "VOID");

TokenType TT_L_PAREN        ("SYMBOL", "L_PAREN");
TokenType TT_R_PAREN        ("SYMBOL", "R_PAREN");
TokenType TT_L_SQUARE_PAREN ("SYMBOL", "L_SQUARE_PAREN");
TokenType TT_R_SQUARE_PAREN ("SYMBOL", "R_SQUARE_PAREN");
TokenType TT_L_CURLY_PAREN  ("SYMBOL", "L_CURLY_PAREN");
TokenType TT_R_CURLY_PAREN  ("SYMBOL", "R_CURLY_PAREN");

TokenType TT_SET       ("VAR", "SET");
TokenType TT_EQUAL     ("VAR", "EQUAL");
TokenType TT_MAKE      ("VAR", "MAKE");
TokenType TT_REFERENCE ("VAR", "REFERENCE");
TokenType TT_TAKING    ("VAR", "TAKING");
TokenType TT_NOTHING   ("VAR", "NOTHING");

TokenType TT_INT     ("LITERAL", "INT");
TokenType TT_DOUBLE  ("LITERAL", "DOUBLE");
TokenType TT_STRING  ("LITERAL", "STRING");
TokenType TT_BOOLEAN ("LITERAL", "BOOLEAN");

TokenType TT_COMMA     ("CTRL", "COMMA");
TokenType TT_COLON     ("CTRL", "COLON");
TokenType TT_SEMICOLON ("CTRL", "SEMICOLON");
TokenType TT_EOF       ("CTRL", "EOF");
TokenType TT_NEWLINE   ("CTRL", "NEWLINE");

namespace LEXING {

	std::string spaceCharacters = std::string(" \t\n");
	std::string digits = std::string("0123456789");
	std::string letters = std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::string lettersDigits = std::string(letters + digits);

}
