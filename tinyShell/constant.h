#define _CRT_SECURE_NO_WARNINGS
#pragma once
#ifndef _CONSTANT_H_
#define _CONSTANT_H_
#define MAXLINE 1024
#define MAXFILE 65536
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include <string>
#include<string.h>
using namespace std;
//�ṹ������
struct Terminal {
	char user[MAXLINE]; // �û���
	char mach[MAXLINE]; // �������
	char root[MAXLINE]; // ��Ŀ¼
	char wdir[MAXLINE]; // ����Ŀ¼
	char strin[MAXFILE]; // �ض����׼����
	char strout[MAXFILE]; // �ض����׼���
};
//ȫ�ֱ�������
extern Terminal gTerm;
//��������
#endif