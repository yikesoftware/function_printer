// parser.cpp: 目标的源文件。
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdarg.h>
#include "parser.h"

#define __BASE_FILE__ "./parser/parser.cpp"

Token token;
extern unsigned int LineCount;
extern unsigned int LinePos;

extern double Parameter;
extern double OriginX;
extern double OriginY;
extern double RotAngle;
extern double ScaleX;
extern double ScaleY;

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
		//fprintf(stderr, "[%s, %d, %s()] ", __BASE_FILE__, __LINE__, __func__);
		SyntaxError(1);
	}
}


/*
* There are four types of nodes in the syntax tree:
* ==============================================
* 1. CONST_ID:	Constants in expressions
* 2. FUNC:		Built in mathematical functions
* 3. T:			Parameter T
* 4. Operator:	PLUS|MINUS|MUL|DIV|POWER
* ==============================================
*/
static struct ExprNode* MakeExprNode(enum Token_Type opcode, ...) {
	va_list Args;
	va_start(Args, opcode);
	struct ExprNode* NewNode = (struct ExprNode*)calloc(1, sizeof(struct ExprNode));
	NewNode->OpCode = opcode;
	switch (opcode) {
	case CONST_ID:
		NewNode->Content.CaseConst = va_arg(Args, double);
		break;
	case FUNC:
		NewNode->Content.CaseFunc.MathFuncPtr = va_arg(Args, FuncPtr);
		NewNode->Content.CaseFunc.Child = va_arg(Args, struct ExprNode*);
		break;
	case T:
		NewNode->Content.CaseParmPtr = &Parameter;
		break;
	default:
		NewNode->Content.CaseOperator.Left = va_arg(Args, struct ExprNode*);
		NewNode->Content.CaseOperator.Right = va_arg(Args, struct ExprNode*);
		break;
	}
	va_end(Args);

	return NewNode;
}

/*
Atom → CONST_ID
		| T
		| FUNC L_BRACKET Expression R_BRACKET
		| L_BRACKET Expression R_BRACKET
*/
static struct ExprNode* Atom() {
	struct ExprNode* Node, * FuncParam;
	switch (token.type) {
	case CONST_ID:
		Node = MakeExprNode(CONST_ID, token.value);
		MathchToken(CONST_ID);
		break;
	case T:
		Node = MakeExprNode(T);
		MathchToken(T);
		break;
	case FUNC:
		FuncParam = Expression();
		Node = MakeExprNode(FUNC, token.FuncPtr, FuncParam);
		MathchToken(FUNC);
		break;
	case L_BRACKET:
		MathchToken(L_BRACKET);
		Node = Expression();
		MathchToken(R_BRACKET);
		break;
	default:
		SyntaxError(UNEXPECTED_TOKEN);
		return NULL;
	}
	return Node;
}

/*
* Component → Atom[POWER Component]
*
* The power operation has two child nodes.
* The left node represents the base and the right node represents the index
*/
static struct ExprNode* Component() {
	struct ExprNode* Left, * Right;
	Left = Atom();
	if (token.type == POWER) {
		MathchToken(POWER);
		Right = Component();
		Left = MakeExprNode(POWER, Left, Right); // The left node of the power operation is itself
	}
	return Left;
}

/*
* Factor → PLUS Factor | MINUS Factor | Component
*/
static struct ExprNode* Factor() {
	struct ExprNode* Left, * Right;
	switch (token.type) {
	case PLUS:
		MathchToken(PLUS);
		Right = Factor();
		break;
	case MINUS:
		MathchToken(MINUS);
		Right = Factor();
		Left = (struct ExprNode*)calloc(1, sizeof(struct ExprNode*));
		Left->OpCode = CONST_ID;
		Left->Content.CaseConst = 0.0;
		Right = MakeExprNode(MINUS, Left, Right);
		break;
	default:
		Right = Component();
	}

	return Right;
}

/*
* Term → Factor{(MUL | DIV) Factor}
*
* Binary multiplication and division are performed
* according to the left combination rule
*/
static struct ExprNode* Term() {
	struct ExprNode* Left, * Right;
	Token_Type TmpType;
	Left = Factor();
	while (token.type == MUL || token.type == DIV) {
		TmpType = token.type;
		MathchToken(TmpType);
		Right = Factor();
		Left = MakeExprNode(TmpType, Left, Right); // left combination
	}
	return Left;
}

/* Expression → Term{(PLUS | MINUS) Term} */
static struct ExprNode* Expression() {
	struct ExprNode* Left, * Right;
	Token_Type TmpType;
	Left = Term();
	while (token.type == PLUS || token.type == MINUS) {
		TmpType = token.type;
		MathchToken(TmpType);
		Right = Term();
		Left = MakeExprNode(TmpType, Left, Right); // left combination
	}
	return Left;
}

/*
* ORIGIN IS (x，y); 
*/
static void OriginStatement() { 
	struct ExprNode *TmpExpr;

	MathchToken(ORIGIN);
	MathchToken(IS);
	MathchToken(L_BRACKET);
	
	TmpExpr = Expression();
	OriginX = CalcExpression(TmpExpr);
	DelExprTree(TmpExpr);

	MathchToken(COMMA);

	TmpExpr = Expression();
	OriginY = CalcExpression(TmpExpr);
	DelExprTree(TmpExpr);

	MathchToken(R_BRACKET);
}

static void RotStatement() { 
	struct ExprNode* TmpExpr;

	MathchToken(ROT);
	MathchToken(IS);
	TmpExpr = Expression();
	RotAngle = CalcExpression(TmpExpr);
	DelExprTree(TmpExpr);
}

static void ScaleStatement() { 
	struct ExprNode* TmpExpr;

	MathchToken(SCALE);
	MathchToken(IS);
	MathchToken(L_BRACKET);

	TmpExpr = Expression();
	ScaleX = CalcExpression(TmpExpr);
	DelExprTree(TmpExpr);

	MathchToken(COMMA);

	TmpExpr = Expression();
	ScaleY = CalcExpression(TmpExpr);
	DelExprTree(TmpExpr);

	MathchToken(R_BRACKET);
}

static void ForStatement() { 
	
}

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
			// fprintf(stderr, "[%s, %d, %s()] ", __BASE_FILE__, __LINE__, __func__);
			SyntaxError(UNEXPECTED_TOKEN);
			break;
	}
}

static void MathchToken(enum Token_Type ExpectedType) {
	// Check whether the next token in the token stream is the expected token
	if (token.type != ExpectedType) {
		// fprintf(stderr, "[%s, %d, %s()] ", __BASE_FILE__, __LINE__, __func__);
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