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
//结构体声明
struct Terminal {
	char user[MAXLINE]; // 用户名
	char mach[MAXLINE]; // 计算机名
	char root[MAXLINE]; // 根目录
	char wdir[MAXLINE]; // 工作目录
	char strin[MAXFILE]; // 重定向标准输入
	char strout[MAXFILE]; // 重定向标准输出
};
//全局变量声明
extern Terminal gTerm;
//函数声明
#endif