// parser.cpp: 目标的源文件。
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdarg.h>
#include "parser.h"

#define __BASE_FILE__ "parser.cpp"

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
	struct Token TmpToken = token;

	switch (token.type) {
	case CONST_ID:
		//puts("Atom case CONST_ID");
		MatchToken(CONST_ID);
		Node = MakeExprNode(CONST_ID, TmpToken.value);
		break;
	case T:
		//puts("Atom case T");
		MatchToken(T);
		Node = MakeExprNode(T);
		break;
	case FUNC:
		//puts("Atom case FUNC");
		MatchToken(FUNC);
		MatchToken(L_BRACKET);
		FuncParam = Expression();
		Node = MakeExprNode(FUNC, TmpToken.FuncPtr, FuncParam);
		MatchToken(R_BRACKET);
		break;
	case L_BRACKET:
		//puts("Atom case L_BRACKET");
		MatchToken(L_BRACKET);
		Node = Expression();
		MatchToken(R_BRACKET);
		break;
	default:
		//puts("Component case DEFAULT");
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
		MatchToken(POWER);
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
		MatchToken(PLUS);
		Right = Factor();
		break;
	case MINUS:
		MatchToken(MINUS);
		Right = Factor();
		Left = (struct ExprNode*)calloc(1, sizeof(struct ExprNode*));
		if (!Left) {
			fprintf(stderr, "[%s, %d, %s()] Can not allocate memory!\n", __BASE_FILE__, __LINE__, __func__);
			exit(-1);
		}
		Left->OpCode = CONST_ID;
		Left->Content.CaseConst = 0.0;
		Right = MakeExprNode(MINUS, Left, Right);
		break;
	default:
		Right = Component();
		break;
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
		MatchToken(TmpType);
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
	//printf("Expression left: %p value: %lf Type: %d\n", Left, Left->Content.CaseConst, Left->OpCode);
	while (token.type == PLUS || token.type == MINUS) {
		TmpType = token.type;
		MatchToken(TmpType);
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

	MatchToken(ORIGIN);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	
	TmpExpr = Expression();
	OriginX = GetExprValue(TmpExpr);
	DelExprTree(TmpExpr);

	MatchToken(COMMA);

	TmpExpr = Expression();
	OriginY = GetExprValue(TmpExpr);
	DelExprTree(TmpExpr);

	MatchToken(R_BRACKET);
}

static void RotStatement() { 
	struct ExprNode* TmpExpr;

	MatchToken(ROT);
	MatchToken(IS);
	TmpExpr = Expression();
	RotAngle = GetExprValue(TmpExpr);
	DelExprTree(TmpExpr);
}

static void ScaleStatement() { 
	struct ExprNode* TmpExpr;

	MatchToken(SCALE);
	MatchToken(IS);
	MatchToken(L_BRACKET);

	TmpExpr = Expression();
	ScaleX = GetExprValue(TmpExpr);
	DelExprTree(TmpExpr);

	MatchToken(COMMA);

	TmpExpr = Expression();
	ScaleY = GetExprValue(TmpExpr);
	DelExprTree(TmpExpr);

	MatchToken(R_BRACKET);
}

/* e.g:
* for t from 0 to 2*pi step pi/100 draw(cos(t), sin(t));
*/
static void ForStatement() { 
	struct ExprNode* TmpExpr;
	struct ExprNode* x_ptr, * y_ptr;
	double start, end, step;
	MatchToken(FOR);
	MatchToken(T);
	MatchToken(FROM);

	TmpExpr = Expression();
	start = GetExprValue(TmpExpr);
	//printf("Expr: %p ExpVal: %lf start: %lf\n", TmpExpr, TmpExpr->Content.CaseConst, start);

	MatchToken(TO);

	TmpExpr = Expression();
	end = GetExprValue(TmpExpr);
	//printf("Expr: %p ExpVal: %lf end: %lf\n", TmpExpr, TmpExpr->Content.CaseConst, end);

	MatchToken(STEP);

	TmpExpr = Expression();
	step = GetExprValue(TmpExpr);
	//printf("Expr: %p ExpVal: %lf step: %lf\n", TmpExpr, TmpExpr->Content.CaseConst, step);

	MatchToken(DRAW);
	MatchToken(L_BRACKET);

	x_ptr = Expression();
	// PrintSyntaxTree(x_ptr, 5);

	MatchToken(COMMA);

	y_ptr = Expression();
	// PrintSyntaxTree(y_ptr, 5);

	MatchToken(R_BRACKET);

	printf("DrawLoop(%lf, %lf, %lf, %p, %p)\n", start, end, step, x_ptr, y_ptr);
	//getchar();
	DrawLoop(start, end, step, x_ptr, y_ptr);
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

static void MatchToken(enum Token_Type ExpectedType) {
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
		MatchToken(SEMICO); 
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

void PrintSyntaxTree(struct ExprNode* root, int indent)
{
	int temp;
	for (temp = 1; temp <= indent; temp++) printf("\t");
	switch (root->OpCode)
	{
	case PLUS:		printf("%s\n", "+");								break;
	case MINUS:		printf("%s\n", "-");								break;
	case MUL:		printf("%s\n", "*");								break;
	case DIV:		printf("%s\n", "/");								break;
	case POWER:		printf("%s\n", "**");								break;
	case FUNC:		printf("%p\n", root->Content.CaseFunc.MathFuncPtr);	break;
	case CONST_ID:	printf("%f\n", root->Content.CaseConst);			break;
	case T:			printf("%s\n", "T");								break;
	default:		printf("Error Tree Node !\n");						exit(0);
	}
	if (root->OpCode == CONST_ID || root->OpCode == T) 
		return;
	if (root->OpCode == FUNC)
		PrintSyntaxTree(root->Content.CaseFunc.Child, indent + 1);
	else
	{
		PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
		PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
	}
}
