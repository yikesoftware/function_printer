// semantic.cpp: 目标的源文件。
//

#include <stdio.h>
#include "semantic.h"
#include "SDL.h"

#define __BASE_FILE__ "semantic.cpp"

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

double GetExprValue(struct ExprNode* root) {
	double result;
	if (root == NULL) {
		return 0.0; // Be careful not to miss this termination condition
	}

	switch (root->OpCode) {
	case PLUS:
		result = GetExprValue(root->Content.CaseOperator.Left) +
			GetExprValue(root->Content.CaseOperator.Right);
		break;
	case MINUS:
		result = GetExprValue(root->Content.CaseOperator.Left) -
			GetExprValue(root->Content.CaseOperator.Right);
		break;
	case MUL:
		result = GetExprValue(root->Content.CaseOperator.Left) *
			GetExprValue(root->Content.CaseOperator.Right);
		break;
	case DIV:
		result = GetExprValue(root->Content.CaseOperator.Left) /
			GetExprValue(root->Content.CaseOperator.Right);
		break;
	case POWER:
		result = pow(GetExprValue(root->Content.CaseOperator.Left),
			GetExprValue(root->Content.CaseOperator.Right));
	case FUNC:
		result = root->Content.CaseFunc.MathFuncPtr(
			GetExprValue(root->Content.CaseFunc.Child) // 
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
	return result;
}

static void CalcCoord(struct ExprNode* x_nptr, struct ExprNode* y_nptr,
	double& x_val,
	double& y_val)
{
	double local_x, local_y, temp;
	local_x = GetExprValue(x_nptr); 	// 计算点的原始坐标
	local_y = GetExprValue(y_nptr);
	local_x *= ScaleX; 			// 比例变换
	local_y *= ScaleY;
	temp = local_x * cos(RotAngle) + local_y * sin(RotAngle);
	local_y = local_y * cos(RotAngle) - local_x * sin(RotAngle);
	local_x = temp; 			// 旋转变换
	local_x += OriginX;			// 平移变换
	local_y += OriginY;
	x_val = local_x; 			// 返回变换后点的坐标
	y_val = local_y;
}


void DrawLoop(double Start,
	double End,
	double Step,
	struct ExprNode* x_ptr,
	struct ExprNode* y_ptr)
{
	extern double Parameter;	// 参数T的存储空间
	double x, y;

	for (Parameter = Start; Parameter <= End; Parameter += Step)
	{
		CalcCoord(x_ptr, y_ptr, x, y);		// 计算实际坐标
		//printf("Draw: (%lf, %lf)\n", x, y);
		DrawPixel((unsigned long)(x+0.5), (unsigned long)(y+0.5));	// 根据坐标绘制点
	}
	Refresh();
}

void Refresh() {
	SDL_RenderPresent(renderer);
}

void DrawPixel(int x, int y) {
	SDL_RenderDrawPoint(renderer, x, y);
}


