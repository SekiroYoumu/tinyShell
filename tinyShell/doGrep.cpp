/*Grep 指令 作者：金洪民 2021010742
*        程序简介：本程序为TinyShell程序的一部分——grep指令，本指令用于查找文件里符合
* 条件的字符串，并把含有该字符串的那一行显示出来，符合条件的字符串以加粗亮红色显示。
*        程序指令说明参见大作业说明文档
* 更新日志：
* 2021/12/09 ver alpha 1.0 :完成函数的主题部分，但缺少标准输入功能
* 2021/12/13 ver alpha 1.1 :补充了标准输入功能和错误提示功能
* 2021/12/17 ver alpha 1.2 :完成与主程序的组合
* 2022/01/04 ver alpha 1.3 :修复了错误输入的处理情况
*/
//以下为测试用main函数
#include "grepHeader.h"
// int main()
//{
//	gTerm.strin[0] = 'B';
//	gTerm.strin[1] = '\n';
//	int argc;
//	cin >> argc;
//	char** argv = new char*[argc];
//	for (int i = 0; i < argc; i++)
//	{
//		argv[i] = new char[20];
//		cin >> argv[i];
//	}
//	doGrep(argc, argv);
//	for (int i = 0; gTerm.strout[i] != '\0' || gTerm.strout[i + 1] != '\0'; i++)
//		cout << gTerm.strout[i];
//	delete[] argv, argc;
//	return 0;
//
//}
void doGrep(int argc, char* argv[])
{
	
	//0.补充可能的情况：1.单个或多个文件中存在无法打开的文件，直接返回错误，2.没有找到文件名（即指令不完全、返回指令不完全情况）
	if (argc == 1)
	{
		cerr << "grep:missing operand"<<endl<<"Usage: grep [OPTION]... PATTERNS [FILE]..." << endl << "Try 'grep --help' for more information." << endl;
		return;
	}
	//1.help功能部分
	if (!strcmp(argv[1], "--help"))
	{
		help(gTerm);
		return;
	}
	//多文件处理与标准输入转化部分
	int doccount = -1;
	int position=0;
	bool check = true;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-'&&argv[i][0]!='\0')
		{
			doccount++;//统计文件数
		}
	}
	if (doccount > 0)
		for (int i = 0; i < doccount; i++)
		{
			position += sin_doGrep(argc - doccount + 1, argv, argv[argc - doccount + i], position);
			if (position < 0)
				return;
		}
	else if (doccount == 0 && argv[argc - 1][0] == '-')
	{
		char sign[] = "-";
		sin_doGrep(argc, argv,sign , position);
	}
	else 
		cerr << "grep:missing operand" << endl << "Usage: grep [OPTION]... PATTERNS [FILE]..." << endl << "Try 'grep --help' for more information." << endl;
}