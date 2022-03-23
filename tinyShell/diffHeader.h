#pragma once
#ifndef _HEADER_H_
#include "constant.h"
#define MAX_LINE_LENGTH 4096
#define MAX_LINE_COUNT 16384
struct LineRelation {
	int x;
	int y;
	char op;
};//经主体函数判断得出的行间关系
struct diffios
{
	bool st = 1;//是否为标准输出
	bool ud = 0;//是否忽略大小写
	bool lb = 0;//是否忽略行末空格
	bool wb = 0;//是否忽略所有空格
	bool bl = 0;//是否忽略空行
	bool qk = 0;//是否只显示有无不同
	bool I = 0;//是否指定特定字符串
	string target;
};//diff输入输出形式的封装
int readFileByLine(ifstream*, char**);
int readStrinByLine(char**, char**);
int customStrcmp(char*, char*, diffios);
bool printResult(stack<LineRelation>, char**, char**, int, int, diffios);
#endif