#pragma once
#include "diffHeader.h"
/*格式化后的diff样本，按行分配，为避免局部变量的栈空间溢出而使用全局变量*/
char* diffSampleA[MAX_LINE_COUNT];
char* diffSampleB[MAX_LINE_COUNT];

//将路径暂存以备回溯
void copyPath(int* a, int* b, int d)
{
	for (int i = -d; i <= d; i++)
	{
		b[i] = a[i];
	}
	return;
}

//检查即将使用的参数有效性
bool checkValidity(char* last, char* check)
{
	if (check == NULL || strcmp(check,"\0") == 0)
	{
		cerr << "diff: missing operand after '" << last << "'" << endl << "diff: Try 'diff --help' for more information." << endl;
		return 0;
	}
	return 1;
}

//diff指令的主体函数,return -1错误，0无差别，1有差别，2从--help结束
int doDiff(int argc, char** argv)
{
	//1、命令读取与检验
	diffios ios;//输入输出模式参数初始化
	if (argv[0] == NULL)
	{
		cerr << "Debug error:invalid argument for 'diff'" << endl;
		return -1;
	}
	if (strcmp(argv[0], "diff") != 0)
	{
		cerr << "Debug error:invalid argument for 'diff'" << endl;
		return -1;
	}
	//以上表示解释器提供参数非法
	if (!checkValidity(argv[0], argv[1])) return -1;
	if (strcmp(argv[1], "--help") == 0)
	{
		string help = "diff Usage: diff [OPTION]... FILES\nCompare FILES line by line.\n\nMandatory arguments to long options are mandatory for short options too.\n- q, report only when files differ\n i, ignore case differences in file contents\n - b, ignore changes in the amount of white space\n - w, ignore all white space\n - B, ignore changes where lines are all blank\n - I, ignore changes where all lines match RE\n--help               display this helpand exit\n\n%<  lines from FILE1\n%>  lines from FILE2\n\nFILES are 'FILE1 FILE2' or 'DIR1 DIR2' or 'DIR FILE' or 'FILE DIR'.\nIf a FILE is '-', read standard input.\nExit status is 0 if inputs are the same, 1 if different, -1 if trouble.\n\n";
		strcpy_s(gTerm.strout, help.length() + 1, help.c_str());
		return 2;
	}
	for (int i = 1; i < argc - 2; i++)//逐个检验参数及其有效性
	{
		bool errorFlag = 1;
		if (!checkValidity(argv[i - 1], argv[i])) return -1;
		if (!ios.ud && (strcmp(argv[i], "-i") == 0))
		{
			ios.st = 0;
			ios.ud = 1;
			errorFlag = 0;
		}
		else if (!ios.lb && (strcmp(argv[i], "-b") == 0))
		{
			ios.st = 0;
			ios.lb = 1;
			errorFlag = 0;
		}
		else if (!ios.wb && (strcmp(argv[i], "-w") == 0))
		{
			ios.st = 0;
			ios.wb = 1;
			errorFlag = 0;
		}
		else if (!ios.bl && (strcmp(argv[i], "-B") == 0))
		{
			ios.bl = 1;
			errorFlag = 0;
		}
		else if (!ios.qk && (strcmp(argv[i], "-q") == 0))
		{
			ios.qk = 1;
			errorFlag = 0;
		}
		else if (!ios.I && (strcmp(argv[i], "-I") == 0))
		{
			ios.I = 1;
			errorFlag = 0;
			i++;//若-I，则认定下一个参数为其指定的字符串
			if (i == argc - 2)
			{
				cerr << "diff: missing operand after '" << argv[i-1] << "'" << endl << "diff: Try 'diff --help' for more information." << endl;
				return -1;
			}
			else ios.target = argv[i];
		}
		if (errorFlag)//如果以上都没没判断到，则说明这是无效参数
		{
			cerr << "diff: invalid option -- " << argv[i] << endl << "diff: Try 'diff --help' for more information." << endl;
			return -1;
		}
	}
	//2、文件读取
	ifstream afile, bfile;
	int sLenA, sLenB;
	string filenameA, filenameB;
	char* strinPtr = NULL;
	if (argc < 3)
	{
		cerr << "diff: missing operand after '" << argv[argc-1] << "'" << endl << "diff: Try 'diff --help' for more information." << endl;
		return -1;
	}
	if (strcmp(argv[argc - 2], "-") != 0)
	{
		filenameA = gTerm.root;
		filenameA.append(1, '/');
		filenameA.append(gTerm.wdir);
		filenameA.append(1, '/');
		filenameA.append(argv[argc - 2]);//将root,wdir和传入参数连接起来，作为读取文件的位置(我的理解是前两个变量的结尾不带/，需要补)；后同
		afile.open(filenameA);
		if (afile.is_open() == false)
		{
			cerr << "diff: " << argv[argc - 2] << ": No such file or directory" << endl;
			return -1;
		}
		sLenA = readFileByLine(&afile, diffSampleA);
	}
	else
	{
		sLenA = readStrinByLine(diffSampleA,&strinPtr);
	}
	if (!checkValidity(argv[argc - 2], argv[argc - 1])) return -1;
	if (strcmp(argv[argc - 1], "-") != 0)
	{
		filenameB = gTerm.root;
		filenameB.append(1, '/');
		filenameB.append(gTerm.wdir);
		filenameB.append(1, '/');
		filenameB.append(argv[argc - 1]);
		bfile.open(filenameB);
		if (bfile.is_open() == false)
		{
			cerr << "diff: " << argv[argc-1] << ": No such file or directory" << endl;
			return -1;
		}
		sLenB = readFileByLine(&bfile, diffSampleB);
	}
	else
	{
		sLenB = readStrinByLine(diffSampleB,&strinPtr);
	}
	//3、最短路径计算：Myers' 算法主体
	int* v = new int[2 * (sLenA + sLenB) + 1];
	memset(v, 0, (2 * (sLenA + sLenB) + 1) * sizeof(int));
	v = &v[sLenA + sLenB];//算法需要，将v置于新开空间的中间，使其构成一个[-sLenA-sLenB,sLenA+sLenB]的区间
	int x; //x代表当前判断样本A的第x行
	int y; //y代表当前判断样本B的第y行
	int k; //k=x-y，代表x与y的偏移量
	int d; //d代表从起点到当前位置的距离（即为编辑次数）
	int** result = new int* [sLenA + sLenB + 1];
	for (d = 0; d <= sLenA + sLenB; d++)//遍历起始点到当前点的距离d，寻找在该d下所能到达的最远位置
	{
		result[d] = new int[2 * d + 1];//存放位置，以便回溯找到最短路径
		memset(result[d], 0, (2 * d + 1) * sizeof(int));
		result[d] = &result[d][d];//算法需要，将result置于新开空间的中间，使其构成一个[-d,d]的区间
		copyPath(v, result[d], d); //路径记录
		for (k = -d; k <= d; k += 2)//遍历x，y之间的偏移k
		{
			if (k == -d || (k != d && v[k - 1] < v[k + 1]))
				x = v[k + 1];
			else
				x = v[k - 1] + 1;//根据k和递推得到x与y
			y = x - k;
			while (x < sLenA && y < sLenB && customStrcmp(diffSampleA[x], diffSampleB[y], ios) == 0)//x,y相同，可沿斜线走
			{
				x++;
				y++;
			}
			v[k] = x;
			if (x >= sLenA && y >= sLenB)
			{
				goto loopend;//到达位置，退出多层循环
			}
		}
	}
loopend:;
	v = &v[-sLenA - sLenB];//在删除空间之前使已偏移的指针回到原位
	delete[] v;
	v = NULL;
	//4、根据保存的结果回溯路线
	int last_x, last_y, last_k;
	stack<LineRelation> line; //存放行间关系，由于关系是从后向前回溯得到的，故存放栈中，利用栈FILO的特性进行后续分析
	for (; d >= 0; d--)
	{
		k = x - y;
		if (k == -d || (k != d && result[d][k - 1] < result[d][k + 1]))
		{
			last_k = k + 1;
		}
		else
		{
			last_k = k - 1;
		}
		last_x = result[d][last_k];
		last_y = last_x - last_k;
		if (d > 0)
		{
			while (last_x < x && last_y < y)
			{
				line.push({ x,y,'=' });//表示这一步行间相等
				x--;
				y--;
			}
		}
		if (d > 0)
		{
			if (last_x == x)//表示这一步做了新增
				line.push({ x,y,'a' });
			else if (last_y == y)//表示这一步做了删除
				line.push({ x,y,'d' });
		}
		x = last_x;
		y = last_y;
		result[d] = &result[d][-d];//在回收空间之前使已偏移的指针回到原位
		delete[] result[d];
		result[d] = NULL;
	}
	delete[] result;
	bool change = printResult(line, diffSampleA, diffSampleB, sLenA, sLenB, ios);
	if (change && ios.qk) //是否为简洁输出
	{
		sprintf_s(gTerm.strout, "Files %s and %s differ\n", argv[argc - 2], argv[argc - 1]);
	}
	return change;
}