#pragma once
#ifndef _HEADER_H_
#include "constant.h"
#define MAX_LINE_LENGTH 4096
#define MAX_LINE_COUNT 16384
struct LineRelation {
	int x;
	int y;
	char op;
};//�����庯���жϵó����м��ϵ
struct diffios
{
	bool st = 1;//�Ƿ�Ϊ��׼���
	bool ud = 0;//�Ƿ���Դ�Сд
	bool lb = 0;//�Ƿ������ĩ�ո�
	bool wb = 0;//�Ƿ�������пո�
	bool bl = 0;//�Ƿ���Կ���
	bool qk = 0;//�Ƿ�ֻ��ʾ���޲�ͬ
	bool I = 0;//�Ƿ�ָ���ض��ַ���
	string target;
};//diff���������ʽ�ķ�װ
int readFileByLine(ifstream*, char**);
int readStrinByLine(char**, char**);
int customStrcmp(char*, char*, diffios);
bool printResult(stack<LineRelation>, char**, char**, int, int, diffios);
#endif