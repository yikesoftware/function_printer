// semantic.cpp: 目标的源文件。
//

#include "semantic.h"
#include "SDL.h"

double Parameter;
double OriginX;
double OriginY;
double RotAngle;
double ScaleX;
double ScaleY;

SDL_Window* sdl_window;
SDL_Renderer* renderer;

void DelExprTree(struct ExprNode* root) {
	if (root == NULL) {
		return;
	}
	switch (root->OpCode) {
	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case POWER:
		DelExprTree(root->Content.CaseOperator.Left);
		DelExprTree(root->Content.CaseOperator.Right);
		break;
	case FUNC:
		DelExprTree(root->Content.CaseFunc.Child);
		break;
	default:
		break;
	}
	free(root);
}

double CalcExpression(struct ExprNode* root) {
	double result;
	if (root == NULL) {
		return 0.0; // Be careful not to miss this termination condition
	}
	switch (root->OpCode) {
	case PLUS:
		result = CalcExpression(root->Content.CaseOperator.Left) +
			CalcExpression(root->Content.CaseOperator.Right);
		break;
	case MINUS:
		result = CalcExpression(root->Content.CaseOperator.Left) -
			CalcExpression(root->Content.CaseOperator.Right);
		break;
	case MUL:
		result = CalcExpression(root->Content.CaseOperator.Left) *
			CalcExpression(root->Content.CaseOperator.Right);
		break;
	case DIV:
		result = CalcExpression(root->Content.CaseOperator.Left) /
			CalcExpression(root->Content.CaseOperator.Right);
		break;
	case POWER:
		result = pow(CalcExpression(root->Content.CaseOperator.Left),
			CalcExpression(root->Content.CaseOperator.Right));
	case FUNC:
		result = root->Content.CaseFunc.MathFuncPtr(
			CalcExpression(root->Content.CaseFunc.Child) // 
		);
		break;
	case CONST_ID:
		result = root->Content.CaseConst;
		break;
	case T:
		result = *(root->Content.CaseParmPtr);
		break;
	default:
		break;
	}
	return 0.0;
}

void DrawPoint(int x, int y) {
	SDL_RenderDrawPoint(renderer, x, y);
}
