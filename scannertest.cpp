#include <iostream>
#include <cstdlib>
#include "scanner.h"

extern unsigned int LineCount;
extern unsigned int LinePos;

/*
int main() {
	std::cout << "Scanner Test..." << std::endl;
	if (InitScanner("C://test.funcs") == -1) {
		std::cout << "Test Finished..." << std::endl;
		return -1;
	}
	Token token;
	token = GetToken();
	while (token.type != NONTOKEN && token.type != ERRTOKEN) {
		printf("Type %d lexeme \"%s\"\nvalue % lf\nfuncprt % p\n\n", 
			token.type, token.lexeme, token.value, token.FuncPtr);
		token = GetToken();
	}
	CloseScanner();
	return 0;
}
*/