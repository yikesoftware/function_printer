// scanner.h: 目标的头文件。

#pragma once

#include <ctype.h>
#include <cmath>

enum Token_Type {
	ORIGIN, SCALE, ROT, IS, // Reserved word
	TO, STEP, DRAW, FOR, FROM, // Reserved word
	T, // Parameter
	SEMICO, L_BRACKET, R_BRACKET, COMMA, // Separator
	PLUS, MINUS, MUL, DIV, POWER, // Operator
	FUNC, // Function
	CONST_ID, // Constant
	NONTOKEN, // End Mark
	ERRTOKEN // Error Maek
};

struct Token {
	Token_Type type;
	char* lexeme;
	// value or function pointer
	double value;
	double (*FuncPtr)(double);
};

static Token TokenTab[] = {
	{CONST_ID,	"PI",		3.1415926,	NULL},
	{CONST_ID,	"E",		2.71828,	NULL},
	{T,			"T",		0,			NULL},
	{FUNC,		"SIN",		0.0,		sin},
	{FUNC,		"COS",		0.0,		cos},
	{FUNC,		"TAN",		0.0,		tan},
	{FUNC,		"LN",		0.0,		log},
	{FUNC,		"EXP",		0.0,		exp},
	{FUNC,		"SQRT",		0.0,		sqrt},
	{ORIGIN,	"ORIGIN",	0.0,		NULL},
	{SCALE,		"SCALE",	0.0,		NULL},
	{ROT,		"ROT",		0.0,		NULL},
	{IS,		"IS",		0.0,		NULL},
	{FOR,		"FOR",		0.0,		NULL},
	{FROM,		"FROM",		0.0,		NULL},
	{TO,		"TO",		0.0,		NULL},
	{STEP,		"STEP",		0.0,		NULL},
	{DRAW,		"DRAW",		0.0,		NULL},
};

int InitScanner(const char* filename);
Token GetToken();
int CloseScanner();