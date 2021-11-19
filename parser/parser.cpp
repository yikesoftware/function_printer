// parser.cpp: 目标的源文件。
//

#include <cstdio>
#include <cstdlib>
#include "parser.h"

#define __BASE_FILE__ "./parser/parser.cpp"

Token token;
extern unsigned int LineCount;
extern unsigned int LinePos;

void FetchToken() {
	token = GetToken();
	if (token.type == ERRTOKEN) {
		
	}
}

void Program() {
	return;
}

void Parser(const char *filename) {
	if (InitScanner(filename) < 0) {
		fprintf(stderr, "[%s, %d, %s()] Parse error!\n", __BASE_FILE__, __LINE__, __func__);
		exit(-1);
	}
}