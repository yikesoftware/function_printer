// parser.h: 目标的头文件。

#pragma once
#include "scanner.h"
#include "semantic.h"

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


static struct ExprNode* MakeExprNode(enum Token_Type opcode, ...);
static struct ExprNode* Atom();
static struct ExprNode* Component();
static struct ExprNode* Factor();
static struct ExprNode* Term();
static struct ExprNode* Expression();
static void MathchToken(enum Token_Type ExpectedType);