// semantic.h: 目标的头文件。

#pragma once
#include "parser.h"

void DelExprTree(struct ExprNode* root);

double CalcExpression(struct ExprNode* root);

void DrawPoint(int x, int y);