// parser.cpp: 目标的源文件。
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "parser.h"

#define __BASE_FILE__ "./parser/parser.cpp"

Token token;
extern unsigned int LineCount;
extern unsigned int LinePos;

static void SyntaxError(int errtype) {
	switch (errtype) {
		case ERROR_TOKEN:
			fprintf(stderr, "%s: Error token '%s' in line %u column %llu!\n",
				__func__, token.lexeme, LineCount, LinePos-strlen(token.lexeme));
			break;
		case UNEXPECTED_TOKEN:
			fprintf(stderr, "%s: Unexpected token '%s' in line %u column %llu!\n",
				__func__, token.lexeme, LineCount, LinePos-strlen(token.lexeme));
			break;
	}
}

static void FetchToken() {
	token = GetToken();
	if (token.type == ERRTOKEN) {
		// Get an error token
		SyntaxError(1);
	}
}

static void OriginStatement() { return; }

static void RotStatement() { return; }

static void ScaleStatement() { return; }

static void ForStatement() { return; }

static void Statement() { 
	// Enter different statement processing subroutines according to different token types
	switch (token.type)
	{
		case ORIGIN:
			OriginStatement();
			break;
		case SCALE:
			ScaleStatement();
			break;
		case ROT:
			RotStatement();
			break;
		case FOR:
			ForStatement();
			break;
		default:
			SyntaxError(UNEXPECTED_TOKEN);
			break;
	}
}

static void MathchToken(enum Token_Type ExpectedType) {
	// Check whether the next token in the token stream is the expected token
	if (token.type != ExpectedType) {
		SyntaxError(UNEXPECTED_TOKEN);
	}
	FetchToken(); // Get next token
}

static void Program() {
	// Get into a loop to process token stream
	while (token.type != NONTOKEN) {
		// Parse entire statement
		Statement(); 
		// Expecting a terminator of statement - ';'
		MathchToken(SEMICO); 
	}
	return;
}

void Parser(const char *filename) {
	if (InitScanner(filename) < 0) {
		fprintf(stderr, "[%s, %d, %s()] Parse error!\n", __BASE_FILE__, __LINE__, __func__);
		exit(-1);
	}
	FetchToken(); // Fetch the first token
	Program(); // start program
	CloseScanner();
}