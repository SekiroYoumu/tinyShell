#pragma once
#include "diffHeader.h"
/*��ʽ�����diff���������з��䣬Ϊ����ֲ�������ջ�ռ������ʹ��ȫ�ֱ���*/
char* diffSampleA[MAX_LINE_COUNT];
char* diffSampleB[MAX_LINE_COUNT];

//��·���ݴ��Ա�����
void copyPath(int* a, int* b, int d)
{
	for (int i = -d; i <= d; i++)
	{
		b[i] = a[i];
	}
	return;
}

//��鼴��ʹ�õĲ�����Ч��
bool checkValidity(char* last, char* check)
{
	if (check == NULL || strcmp(check,"\0") == 0)
	{
		cerr << "diff: missing operand after '" << last << "'" << endl << "diff: Try 'diff --help' for more information." << endl;
		return 0;
	}
	return 1;
}

//diffָ������庯��,return -1����0�޲��1�в��2��--help����
int doDiff(int argc, char** argv)
{
	//1�������ȡ�����
	diffios ios;//�������ģʽ������ʼ��
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
	//���ϱ�ʾ�������ṩ�����Ƿ�
	if (!checkValidity(argv[0], argv[1])) return -1;
	if (strcmp(argv[1], "--help") == 0)
	{
		string help = "diff Usage: diff [OPTION]... FILES\nCompare FILES line by line.\n\nMandatory arguments to long options are mandatory for short options too.\n- q, report only when files differ\n i, ignore case differences in file contents\n - b, ignore changes in the amount of white space\n - w, ignore all white space\n - B, ignore changes where lines are all blank\n - I, ignore changes where all lines match RE\n--help               display this helpand exit\n\n%<  lines from FILE1\n%>  lines from FILE2\n\nFILES are 'FILE1 FILE2' or 'DIR1 DIR2' or 'DIR FILE' or 'FILE DIR'.\nIf a FILE is '-', read standard input.\nExit status is 0 if inputs are the same, 1 if different, -1 if trouble.\n\n";
		strcpy_s(gTerm.strout, help.length() + 1, help.c_str());
		return 2;
	}
	for (int i = 1; i < argc - 2; i++)//����������������Ч��
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
			i++;//��-I�����϶���һ������Ϊ��ָ�����ַ���
			if (i == argc - 2)
			{
				cerr << "diff: missing operand after '" << argv[i-1] << "'" << endl << "diff: Try 'diff --help' for more information." << endl;
				return -1;
			}
			else ios.target = argv[i];
		}
		if (errorFlag)//������϶�ûû�жϵ�����˵��������Ч����
		{
			cerr << "diff: invalid option -- " << argv[i] << endl << "diff: Try 'diff --help' for more information." << endl;
			return -1;
		}
	}
	//2���ļ���ȡ
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
		filenameA.append(argv[argc - 2]);//��root,wdir�ʹ������������������Ϊ��ȡ�ļ���λ��(�ҵ������ǰ���������Ľ�β����/����Ҫ��)����ͬ
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
	//3�����·�����㣺Myers' �㷨����
	int* v = new int[2 * (sLenA + sLenB) + 1];
	memset(v, 0, (2 * (sLenA + sLenB) + 1) * sizeof(int));
	v = &v[sLenA + sLenB];//�㷨��Ҫ����v�����¿��ռ���м䣬ʹ�乹��һ��[-sLenA-sLenB,sLenA+sLenB]������
	int x; //x����ǰ�ж�����A�ĵ�x��
	int y; //y����ǰ�ж�����B�ĵ�y��
	int k; //k=x-y������x��y��ƫ����
	int d; //d�������㵽��ǰλ�õľ��루��Ϊ�༭������
	int** result = new int* [sLenA + sLenB + 1];
	for (d = 0; d <= sLenA + sLenB; d++)//������ʼ�㵽��ǰ��ľ���d��Ѱ���ڸ�d�����ܵ������Զλ��
	{
		result[d] = new int[2 * d + 1];//���λ�ã��Ա�����ҵ����·��
		memset(result[d], 0, (2 * d + 1) * sizeof(int));
		result[d] = &result[d][d];//�㷨��Ҫ����result�����¿��ռ���м䣬ʹ�乹��һ��[-d,d]������
		copyPath(v, result[d], d); //·����¼
		for (k = -d; k <= d; k += 2)//����x��y֮���ƫ��k
		{
			if (k == -d || (k != d && v[k - 1] < v[k + 1]))
				x = v[k + 1];
			else
				x = v[k - 1] + 1;//����k�͵��Ƶõ�x��y
			y = x - k;
			while (x < sLenA && y < sLenB && customStrcmp(diffSampleA[x], diffSampleB[y], ios) == 0)//x,y��ͬ������б����
			{
				x++;
				y++;
			}
			v[k] = x;
			if (x >= sLenA && y >= sLenB)
			{
				goto loopend;//����λ�ã��˳����ѭ��
			}
		}
	}
loopend:;
	v = &v[-sLenA - sLenB];//��ɾ���ռ�֮ǰʹ��ƫ�Ƶ�ָ��ص�ԭλ
	delete[] v;
	v = NULL;
	//4�����ݱ���Ľ������·��
	int last_x, last_y, last_k;
	stack<LineRelation> line; //����м��ϵ�����ڹ�ϵ�ǴӺ���ǰ���ݵõ��ģ��ʴ��ջ�У�����ջFILO�����Խ��к�������
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
				line.push({ x,y,'=' });//��ʾ��һ���м����
				x--;
				y--;
			}
		}
		if (d > 0)
		{
			if (last_x == x)//��ʾ��һ����������
				line.push({ x,y,'a' });
			else if (last_y == y)//��ʾ��һ������ɾ��
				line.push({ x,y,'d' });
		}
		x = last_x;
		y = last_y;
		result[d] = &result[d][-d];//�ڻ��տռ�֮ǰʹ��ƫ�Ƶ�ָ��ص�ԭλ
		delete[] result[d];
		result[d] = NULL;
	}
	delete[] result;
	bool change = printResult(line, diffSampleA, diffSampleB, sLenA, sLenB, ios);
	if (change && ios.qk) //�Ƿ�Ϊ������
	{
		sprintf_s(gTerm.strout, "Files %s and %s differ\n", argv[argc - 2], argv[argc - 1]);
	}
	return change;
}