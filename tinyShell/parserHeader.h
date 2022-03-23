#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_
#define MAXARG 512
#include "constant.h"
void doTee(int, char**);
void doCat(int, char**);
void doCp(int, char**);
void doPwd(int, char**);
void doCd(int, char**);
void doGrep(int, char*[]);
int doDiff(int, char**);
#endif