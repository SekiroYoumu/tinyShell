#pragma once
#ifndef _HEADER_H_
#define _HEADER_H_
#include "constant.h"
void help(Terminal gTerm);
void doGrep(int argc, char* argv[]);
int sin_doGrep(int argc, char* targv[], char* doc, int position);
bool cprfunc(char* argv, bool* func, int* lineA, int* lineB);
void Pre_Grep(int argc, char** argv, bool* func, string original, int* counter_wildchar, string* temp);
void Main_Grep(int counter_wildchar, string* temp, int filelen, string* file, int** fout, bool* check, bool func);
void sec_grep(int* start, int* end, int* temp_s, int counter, string* temp, string* file, bool* fcheck, int i, bool func, int counter_wildchar);
string ignore_grep(string file, bool func);
int out_grep(string* file, int** fout, bool* check, bool* func, int countline, Terminal* gTerm, string filename, int lineA, int lineB, int position);
#endif