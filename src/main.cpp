/*
Copyright (c) 2018 K. S. Ernest (iFire) Lee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "thirdparty/catch2/catch.hpp"
#include "thirdparty/cpp-peglib/peglib.h"
#include <assert.h>
#include <iostream>

using namespace peg;
using namespace std;


SCENARIO("Ink text can be converted into data", "[ink]") {

	GIVEN("An ink grammar") {
		// clang-format off
//		auto grammar = R"(
//# ? (option, aka zero-or-one)
//# lpeg ^-1
//# * (aka Kleene star, zero-or-more) 
//# lpeg ^0
//# + (one-or-more)
//# lpeg ^1
//# a b (with spaces between them) is a 'sequence' expression
//# lpeg *
//# a / b is an 'ordered choice' expression
//# lpeg +
//# https://github.com/PhilippeSigaud/Pegged/wiki/PEG-Basics
//# Ported from https://github.com/premek/pink/blob/master/pink/parser.lua
//
//LINES <- TAG_GLOBAL* LINE*
//LINE <- (STMT / GATHER / PARA) COMM*
//STMT <- GLUE / DIVERT / KNOT / STITCH / OPTION / COMM / TEXT COMM*
//
//PARA <- TAG_ABOVE / TEXT TAG_END
//TEXT <- < (!TAG_END .)* > _ / (!COMM_OL)* _ / (!COMM_ML)* _ / (!NL)* _ / (!EOF)* _
//
//TAG_END <- TAG
//TAG_ABOVE <- TAG
//TAG_GLOBAL <- TAG
//
//HASH <- '#' _
//TAG <- ~HASH TAG_CONTENT _
//TAG_CONTENT <- (![\r\n] .)*
//
//OPTION <- OPT_STARS OPT_ANS _
//OPT_DIV_CONT <- (!']' .)*
//OPT_ANS_WITH_DIV <- _ '[' _ OPT_DIV_CONT _ ']' / _ OPT_ANS_CONTENT _
//OPT_ANS_WITHOUT_DIV <- _
//OPT_ANS <- OPT_ANS_WITH_DIV / OPT_ANS_WITHOUT_DIV
//OPT_ANS_CONTENT <- (![\r\n] .)*
//
//OPT_STARS <- OPT_STAR OPT_STAR* OPT_ANS OPT_CONT _
//~OPT_STAR <- _ '*'
//OPT_CONT <- (![\r\n] .)*
//
//GATHER <- GATHER_MARKS
//GATHER_MARKS <- GATHER_MARK _ GATHER_MARK* _ GATHER_CONTENT
//~GATHER_MARK <- _ '-'
//GATHER_CONTENT <- (![\r\n] .)*
//
//GLUE <- _ '<>' _
//
//DIVERT <- DIVERT_END / DIVERT_JUMP
//
//DIVERT_SYM <- '->' _
//DIVERT_END <- DIVERT_SYM 'END' _
//DIVERT_JUMP <- DIVERT_SYM _ ADDR _
//
//KNOT <- _ ('=' '='+ ) _ ID _ ('=')* _
//STITCH <- '=' _ ID _ ('=')* _
//
//COMM <- COMM_OL / COMM_ML / TODO
//TODO <- _ 'TODO:' ' '* TODO_CONTENT
//TODO_CONTENT <- (![\r\n] .)*
//COMM_OL <- _ '//' ' '* COMM_OL_CONTENT
//COMM_OL_CONTENT <- (![\r\n] .)*
//COMM_ML <- _ '/*' [ \r\n]* COMM_ML_CONTENT
//COMM_ML_CONTENT <- (!'*/' .)*
//ADDR <- ID ('.' ID)?
//ID <- ([a-zA-Z]+ / '_') ([a-zA-Z0-9] / '_')*
//~NL <- [\r\n]*
//~_ <- [ \t\r\n]*
//EOF <- !.
//)";

		auto grammar = R"(
CONTENT <- TEXT (!NL / !DIVERT)*

DIVERT <- ' '* TK_DIVERT ' '+ END
TEXT <- (! NL .)*
END <- TK_END / TK_DONE

TK_DIVERT <- '->'
TK_END <- 'END'
TK_DONE <- 'DONE'

~NL <- [\r\n]
		)";
//TK_ID <- ([a-zA-Z]+ / '_') ([a-zA-Z0-9] / '_')*

		parser parser;
		// clang-format on
		std::stringstream out;
		parser.log = [&out](size_t line, size_t col, const string &msg) {
			out << line << ":" << col << ": " << msg << "\n";
		};

		auto ok = parser.load_grammar(grammar);
		assert(ok);

		parser = parser.enable_ast();
		parser.enable_packrat_parsing();

		auto parse_string = [&parser, &out](const char *p_input, bool p_print = true) {
			shared_ptr<peg::Ast> ast;
			if (parser.parse(p_input, ast)) {
				ast = peg::AstOptimizer(true).optimize(ast);
				out << peg::ast_to_s(ast);
				if (p_print)
					cout << out.str() << std::endl;
			}
			return ast;
		};

		WHEN("given an empty string") {
			auto ast = parse_string("");
			THEN("parses as text") {
				REQUIRE(ast->name == "DOCUMENT");
				REQUIRE(ast->token == "");
			}
		}
		WHEN("given a term") {
			auto ast = parse_string(R"(Hello)");
			THEN("parses as text") {
				REQUIRE(ast->name == "DOCUMENT");
				REQUIRE(ast->token == "Hello");
			}
		}
		WHEN("given a phrase") {
			auto ast = parse_string(R"(Hello, world!)");
			THEN("parses as text") {
				REQUIRE(ast->name == "DOCUMENT");
				REQUIRE(ast->token == "Hello, world!");
			}
		}
		//WHEN("given a TODO") {
		//	shared_ptr<peg::Ast> ast;
		//	if (parser.parse(R"(TODO: This is a todo.)", ast)) {
		//		ast = peg::AstOptimizer(true).optimize(ast);
		//		out << peg::ast_to_s(ast);
		//	}
		//	THEN("parses TODO content") {
		//		REQUIRE(ast->name == "TODO_CONTENT");
		//		REQUIRE(ast->token == "This is a todo.");
		//	}
		//}
		//WHEN("given a one-line comment") {
		//	shared_ptr<peg::Ast> ast;
		//	if (parser.parse(R"(
		//			This is text. // and this is an OL comment
		//			and more text.
		//		)", ast)) {
		//		ast = peg::AstOptimizer(true).optimize(ast);
		//		out << peg::ast_to_s(ast);
		//	}
		//	THEN("parses out the one-line comment") {
		//		auto node = ast;
		//		REQUIRE(ast->name == "LINES");
		//		node = node->nodes[0];
		//		REQUIRE(ast->name == "LINE");
		//		node = node->nodes[1];
		//		REQUIRE(ast->name == "COMM_OL");
		//		node = node->nodes[0];
		//		REQUIRE(node->name == "COMM_OL_CONTENT");
		//		REQUIRE(node->token == "and this is an OL comment");
		//	}
		//}
		//WHEN("given a multi-line comment") {
		//	shared_ptr<peg::Ast> ast;
		//	if (parser.parse(R"(
		//			Text /* with a
		//			multiline
		//			comment */ can be useful.
		//		)", ast)) {
		//		ast = peg::AstOptimizer(true).optimize(ast);
		//		out << peg::ast_to_s(ast);
		//	}
		//	THEN("parses out the multi-line comment") {
		//		auto node = ast;
		//		REQUIRE(ast->name == "LINES");
		//		node = node->nodes[0];
		//		REQUIRE(ast->name == "LINE");
		//		node = node->nodes[1];
		//		REQUIRE(ast->name == "COMM_ML");
		//		node = node->nodes[0];
		//		REQUIRE(node->name == "COMM_ML_CONTENT");
		//		REQUIRE(node->token == "and this is an OL comment");
		//		//REQUIRE(ast->token == "with a\n multiline\n comment");
		//		cout << node->token << std::endl;
		//		cout << node->token.length();
		//	}
		//}
	}
}
