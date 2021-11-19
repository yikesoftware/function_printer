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

void SyntaxError(int errtype) {
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

void FetchToken() {
	token = GetToken();
	if (token.type == ERRTOKEN) {
		// Get an error token
		SyntaxError(1);
	}
}

void Program() {
	// Get into a loop to process token stream
	while (token.type != NONTOKEN) {
		
	}
	return;
}

void Parser(const char *filename) {
	if (InitScanner(filename) < 0) {
		fprintf(stderr, "[%s, %d, %s()] Parse error!\n", __BASE_FILE__, __LINE__, __func__);
		exit(-1);
	}
}