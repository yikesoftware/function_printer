// parser.h: 目标的头文件。

#pragma once
#include "scanner.h"

#define ERROR_TOKEN 1
#define UNEXPECTED_TOKEN 2

typedef double (*FuncPtr)(double);
struct ExprNode
{
	enum Token_Type OpCode;	// type of token
	union
	{
		struct {
			ExprNode* Left, * Right;
		} CaseOperator;	// Binary operation
		struct {
			ExprNode* Child;
			FuncPtr MathFuncPtr;
		} CaseFunc;	// Function call
		double CaseConst; 	// Constant, R-value
		double* CaseParmPtr; 	// Parameter 'T', L-value
	} Content; // The content of a node can only be one of them
};


void Parser(const char *filename);
void FetchToken();
void Program();
