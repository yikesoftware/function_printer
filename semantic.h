// semantic.h: 目标的头文件。

#pragma once
#include "parser.h"

void DelExprTree(struct ExprNode* root);

double GetExprValue(struct ExprNode* root);

static void CalcCoord(struct ExprNode* x_nptr, struct ExprNode* y_nptr,
	double& x_val,
	double& y_val);

void DrawLoop(double Start,
	double End,
	double Step,
	struct ExprNode* x_ptr,
	struct ExprNode* y_ptr);

void DrawPixel(int x, int y);

void Refresh();