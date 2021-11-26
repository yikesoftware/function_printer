// scanner.cpp: 目标的源文件。
//

#include <cstdio>
#include <iostream>
#include <cstring>
#include "scanner.h"

#define __BASE_FILE__ "scanner.cpp"
#define MAX_TOKEN_LEN 0x40

char TokenBufStream[MAX_TOKEN_LEN + 8];

FILE* srcFile;
unsigned int LineCount = 1;
unsigned int LinePos = 0;

int InitScanner(const char* filename) {
	srcFile = fopen(filename, "r");
	if (srcFile == NULL) {
		fprintf(stderr, "[%s, %d, %s()] Can not open file '%s'!\n", __BASE_FILE__, __LINE__, __func__, filename);
		return -1;
	}
	return 0;
}

static fpos_t GetFilePos() {
	fpos_t pos;
	fgetpos(srcFile, &pos);
	return pos;
}

static char FetchChar() {
	// Read one char from srcFile stream;
	char ch = fgetc(srcFile);
	if (ch != EOF) {
		LinePos++;
	}
	return ch;
}

static void IgnoreChar(const char ch) {
	// Put ch back to the srcFile stream
	// It will be fetched again next turn;
	if (ch != EOF) {
		ungetc(ch, srcFile);
		LinePos--;
	}
}

static void EmptyTokenBuf() {
	memset(TokenBufStream, 0, sizeof(TokenBufStream));
}

static int TokenBufPush(const char ch) {
	if (strlen(TokenBufStream) >= MAX_TOKEN_LEN) {
		fprintf(stderr, "[%s, %d, %s()] Token too long! - '%s'\n", __BASE_FILE__, __LINE__, __func__, TokenBufStream);
		exit(-1);
	}
	TokenBufStream[strlen(TokenBufStream)] = ch;
	TokenBufStream[strlen(TokenBufStream) + 1] = '\0';
	return 0;
}

static Token GetErrorToken() {
	Token errtoken;
	memset(&errtoken, 0, sizeof(errtoken));
	return errtoken;
}

static Token KeyTokenMatch() {
	int TokenTabSize = sizeof(TokenTab) / sizeof(Token);
	// Traverse the entire TokenTab
	for (int i = 0; i < TokenTabSize; i++) {
		Token* item = &TokenTab[i];
		#if _WIN32
		if(!stricmp(TokenBufStream, item->lexeme))
		#else
		if(!strcasecmp(TokenBufStream, item->lexeme))
		#endif
		{
			// Match successful
			return TokenTab[i];
		}
	}
	// Matching failed
	return GetErrorToken();
}

Token GetToken() {
	Token token;
	char CurrChar;
	int FiniFlag = 0;
	// bzero
GET_TOKEN:
	memset(&token, 0, sizeof(token));
	EmptyTokenBuf();

	CurrChar = FetchChar();
	token.lexeme = TokenBufStream;
	// strip
	switch (CurrChar) {
		// strip whitespace
	case ' ':
	case '\\':
	case '\r':
	case '\t':
		goto GET_TOKEN;
	case '\n':
		// start new line
		LineCount++;
		LinePos = 0;
		goto GET_TOKEN;
	default:
		break;
	}


	// Process symbols beginning with alpha letters
	if (isalpha(CurrChar)) {
		do {
			TokenBufPush(CurrChar);
			CurrChar = FetchChar();
		} while (isalnum(CurrChar));
		IgnoreChar(CurrChar);

		token = KeyTokenMatch();
		return token;
	}

	// Process symbols beginning with numeric character
	if (isdigit(CurrChar)) {
		// Integer part
		do {
			TokenBufPush(CurrChar);
			CurrChar = FetchChar();
		} while (isdigit(CurrChar));

		// Fractional part (if any)
		if (CurrChar == '.') {
			TokenBufPush(CurrChar);
			CurrChar = FetchChar();
			// Cannot be empty after decimal point
			if (!isdigit(CurrChar)) {
				IgnoreChar(CurrChar);
				return GetErrorToken();
			}
			do {
				TokenBufPush(CurrChar);
				CurrChar = FetchChar();
			} while (isdigit(CurrChar));
			IgnoreChar(CurrChar);
		}
		else {
			IgnoreChar(CurrChar);
		}
		token.type = CONST_ID;
		token.lexeme = TokenBufStream;
		token.value = atof(TokenBufStream);

		return token;
	}

	TokenBufPush(CurrChar);
	switch (CurrChar) {
		// Separator
	case ';':
		token.type = SEMICO;
		break;
	case '(':
		token.type = L_BRACKET;
		break;
	case ')':
		token.type = R_BRACKET;
		break;
	case ',':
		token.type = COMMA;
		break;

		// Operators
	case '+':
		token.type = PLUS;
		break;
	case '-':
		CurrChar = FetchChar();
		if (CurrChar != '-') {
			IgnoreChar(CurrChar);
			token.type = MINUS;
			break;
		}
		do {
			CurrChar = FetchChar();
		} while (CurrChar != '\n');
		IgnoreChar(CurrChar); // put '\n' back to the stream
		goto GET_TOKEN;
	case '/':
		CurrChar = FetchChar();
		if (CurrChar != '/') {
			IgnoreChar(CurrChar);
			token.type = DIV;
			break;
		}
		do {
			CurrChar = FetchChar();
		} while (CurrChar != '\n');
		IgnoreChar(CurrChar); // put '\n' back to the stream
		goto GET_TOKEN;
	case '*':
		CurrChar = FetchChar();
		if (CurrChar == '*') {
			token.type = POWER;
			break;
		}
		else {
			IgnoreChar(CurrChar);
			token.type = MUL;
			break;
		}

	case EOF:
		token.type = NONTOKEN;
		break;

	default:
		fprintf(stderr, "[%s, %d, %s()] Invalid char '%c' at line %u column %u!\n",
			__BASE_FILE__, __LINE__, __func__,
			CurrChar, LineCount, LinePos);
		token.type = ERRTOKEN;
		break;
	}
	return token;
}

int CloseScanner() {
	return fclose(srcFile);
}
