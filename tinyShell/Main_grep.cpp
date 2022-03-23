#include"constant.h"
#include"grepHeader.h"

int sin_doGrep(int argc, char* targv[], char* doc, int position)
{
	//更改文件名
	targv[argc - 1] = doc;
	//2.函数主程序grep功能部分
	//这里判断所要用到的函数功能
	bool* func = new bool[7];//存储判断
	int lineA = 0, lineB = 0;
	int* plineA = &lineA;
	int* plineB = &lineB;
	for (int i = 0; i < 7; i++)
		func[i] = 0;
	for (int i = 1; i < argc - 2; i++)
		if (cprfunc(targv[i], func, plineA, plineB))
			return -256;
	//这里对模板串进行分块
	string original;
	int counter_wildchar = 0;
	int* c = &counter_wildchar;
	string* temp = new string[100];
	Pre_Grep(argc, targv, func, original, c, temp);
	//这里接受文件内容，比较并输出
	char File_Temp[256];
	int countline = 0;
	string fileroute = gTerm.root;
	fileroute.append(1, '/');
	fileroute.insert(fileroute.length(), gTerm.wdir);
	fileroute.insert(fileroute.length(), targv[argc - 1]);
	string filename;
	if (fileroute.rfind("/") != fileroute.npos)
	{
		int begin = fileroute.rfind("/");
		filename.insert(0, fileroute, begin + 1, fileroute.length() - begin + 1);
	}
	else
		filename = fileroute;
	string* file;
	int** fileout;
	bool* fcheck;
	ifstream fin1(fileroute);
	if (!fin1.good())
	{
		if (filename != "-")//本部分用于检测错误文件等情况
		{
			cerr << "grep:" << doc << ":" << "No such file or directory. " << endl;
			return -256;
		}
		//本部分用于处理标准输入的情况
		int i = 0;
		char* strin_temp = new char[100];
		for (int m = 0; m < 100; m++)
			strin_temp[m] = '\0';
		char** strin_point = new char* [100];
		strin_point[0] = &strin_temp[0];
		int counter = 0;
		while (gTerm.strin[i] != '\0')
		{
			if (gTerm.strin[i] == '\n')
			{
				counter++;
				countline++;
				strin_point[counter] = &strin_temp[i + 1];
				i++;
			}
			else
			{
				strin_temp[i] = gTerm.strin[i];
				i++;
			}
		}
		file = new string[counter];
		fileout = new int* [counter];
		fcheck = new bool[counter];
		for (int i = 0; i < counter; i++)
		{
			fileout[i] = new int[2];
			file[i].insert(0, strin_point[i]);
		}
	}
	else
	{
		while (fin1.getline(File_Temp, 256))
			countline++;
		fin1.close();
		file = new string[countline];
		fileout = new int* [countline];
		fcheck = new bool[countline];
		ifstream fin2(fileroute);
		for (int i = 0; i < countline; i++)
		{
			fileout[i] = new int[2];
			fin2.getline(File_Temp, 256);
			file[i] = File_Temp;
		}
		fin2.close();
	}
	Main_Grep(counter_wildchar, temp, countline, file, fileout, fcheck, func[1]);
	Terminal* p = &gTerm;
	position += out_grep(file, fileout, fcheck, func, countline, p, filename, lineA, lineB, position);
	delete[] func, temp, file, fileout, fcheck;
	return position;
}

//本函数为判断功能
bool cprfunc(char* argv, bool* func, int* lineA, int* lineB)
{
	bool checker = false;
	if (!strcmp(argv, "-h")) { func[0] = false; return false; }
	if (!strcmp(argv, "-H")) { func[0] = true; return false; }
	if (!strcmp(argv, "-i")) { func[1] = true; return false; }

	if (!strcmp(argv, "-n")) { func[2] = true; return false; }
	string temp = argv;
	if (temp.at(1) == 'A') { func[4] = true; }
	if (temp.at(1) == 'B') { func[3] = true; }
	if (!strcmp(argv, "-c")) { func[5] = true; return false; }
	if (func[4] && !func[3])
	{
		temp.erase(0, 2);
		for (int i = 0; ; i++)
		{
			*lineB += (temp[i] - 48);
			if (i < (int)(temp.length() - 1))
				*lineB *= 10;
			else
				break;
		}
		return false;
	}
	if (func[3])
	{
		temp.erase(0, 2);
		for (int i = 0; ; i++)
		{
			*lineA += (temp[i] - 48);
			if (i < (int)(temp.length() - 1))
				*lineA *= 10;
			else
				break;
		}
		return false;
	}
	cerr << "grep: unknown option -" << argv << endl << "Try 'grep --help' for more information." << endl;
	return true;
}

//本函数为帮助指令
void help(Terminal gTerm)
{
	strncpy_s(gTerm.strout, 500, "grep 指令用于查找内容包含指定的范本样式，并将含有范本样式的那一列显示出来。\n若不指定任何文件名称，或是所给予的文件名为 - ，则 grep 指令会从标准输入设备读取数据。\n选项说明：\n--help:显示帮助\n-c： 计算符合样式的行数。\n-h： 在显示符合样式的那一行之前，不标示该行所属的文件名称。\n-H： 在显示符合样式的那一行之前， 标示该行所属的文件名称。\n-i： 忽略字符大小写的差别\n-n： 在显示符合样式的那一行之前，标示出该行的编号\n-A(-B)[行数]： 除了显示符合范本样式的那一列之外， 同时显示该行之后(之前)的[行数]行内容。\n", 499);
	cout << gTerm.strout;
}

//本函数为处理模板串
void Pre_Grep(int argc, char** targv, bool* func, string original, int* c, string* temp)
{
	original.insert(0, targv[argc - 2]);
	original = ignore_grep(original, func[1]);
	for (int i = 0; i < (int)original.length(); i++)
		if (original[i] == '*' || original[i] == '.')
			(*c)++;
	int counter_wildchar = *c;
	if (counter_wildchar == 0)
	{
		temp[0].insert(0, original);
		return;
	}

	char** wildchar = new char* [counter_wildchar + 1];
	int tmp = 0;
	wildchar[counter_wildchar] = &original[original.length()];
	for (int i = 0; i < (int)original.length(); i++)
	{
		if (original[i] == '*' || original[i] == '.')
		{
			wildchar[tmp] = &original[i];
			tmp++;
		}
	}
	string stop = "!";
	//防止越界，当检查到“！”时结束
	//这里将模式串分为若干段，用于对文件内字段的匹配。
	int tmplen = (int)(wildchar[1] - wildchar[0]);
	int pos = 0;
	int counter = 0;
	for (int i = 0; pos < (int)original.length(); )
	{
		if (original[pos] == '*' || original[pos] == '.')
		{
			temp[i] = original[pos];
			pos++;
			i++;
			counter++;
		}
		else
		{
			tmplen = (int)(wildchar[counter] - &original[0] - pos);
			temp[i].insert(0, original, pos, tmplen);
			pos += tmplen;
			if (original[pos] == '*' || original[pos] == '.')
				i++;
		}
	}
	if (temp[2 * counter_wildchar] == "\0")	temp[2 * counter_wildchar] = stop[0];
	temp[2 * counter_wildchar + 1] = stop[0];
	//这里,temp数组储存了每个需要匹配的字符段，下面就可以对文件进行匹配和输出
}

//本函数对文件内的字符串进行比较
void Main_Grep(int counter_wildchar, string* temp, int filelen, string* file, int** fout, bool* check, bool func)
{

	//对第（i+1）行进行检测
	for (int i = 0; i < filelen; i++)
	{
		//第一种情况：不含通配符的字模式串
		if (counter_wildchar == 0)
		{
			string stemp;
			stemp = ignore_grep(file[i], func);//判断大小写
			string::size_type result = stemp.find(temp[0]);//不含通配符时，temp的第一个字符串即为模式串
			if (result == stemp.npos) { check[i] = false; continue; }
			fout[i][1] = temp[0].length();//字符串长度
			fout[i][0] = result;//第一处出现字符串的位置
			check[i] = true;
			continue;
		}
		//第二种情况：含通配符（一般情况）
		// 检查思路：设置两个活动“指针”表示当前选取区段
		// 初始时将两个指针的位置设置在1处（指向待检测区段）
		// 对于第一个遇到的非通配符字符串，先查找，若找到，将首指针放在字符串首，将尾指针放在字符串尾，
		// 对于第一个遇到的“*”首指针不变，其余同上
		// 对于第一个遇到的“.”将首指针放在字符串首的前一个位置，其余同上
		// 读取模式串的第i个字符串(用counter计数)，
		// 如果字符串是'*'不改变两个指针的位置，直接读取下一个字符串并搜索，找到时将尾指针位置改为搜索到的字符串尾
		// 如果字符串是'.'不改变首指针位置，将尾指针位置改为下一个位置并搜索当前位置对应的区段，相符则将尾指针位置改为搜索到的字符串尾
		// 当遇到没有匹配字符段的情况时，再次进行从当前搜查状态的“第一个匹配字符段的末尾”进行查找，直到找不到匹配的字符段，将判断值设为-1，返回
		// 当检查到最后一个也没有退出（即读取到了“！”）时，将当前情况下的首指针与尾指针存入数组，并将判断值设为-1
		//这种情况下，字符串长度必须大于1
		if (file[i].length() == 1) { check[i] = false; continue; }
		int start = 0, end = 0, first = 0, last = file[i].length(), temp_s, counter = 0;
		int* pstart = &start;
		int* pend = &end;
		int* ptemp_s = &temp_s;
		check[i] = true;
		for (;;)
		{
			//对第一个字符串的判断
			if (temp[0] == ".")
			{
				string stemp;
				stemp = ignore_grep(file[i], func);
				temp_s = stemp.find(temp[1], first + 1);
				if (temp_s == stemp.npos) { check[i] = false; break; }
				start = temp_s - 1; end = temp_s + temp[1].length() - 1;
				first = start + temp[1].length();
				counter = 2;
			}
			else if (temp[0] == "*")
			{
				string stemp;
				stemp = ignore_grep(file[i], func);
				temp_s = stemp.rfind(temp[1], last);
				if (temp_s == stemp.npos) { check[i] = false; break; }
				end = temp_s + temp[1].length() - 1;
				counter = 2;
			}
			else
			{
				string stemp;
				stemp = ignore_grep(file[i], func);
				temp_s = stemp.find(temp[0], first);
				if (temp_s == stemp.npos) { check[i] = false; break; }
				start = temp_s;
				end = temp_s + temp[0].length() - 1;
				first = start + temp[0].length();
				counter = 1;
			}
			//下面执行一般检查
			sec_grep(pstart, pend, ptemp_s, counter, temp, file, check, i, func, counter_wildchar);
			if (check)
			{
				fout[i][0] = start;
				fout[i][1] = end - start + 1;
				break;
			}
		}
	}

}

void sec_grep(int* start, int* end, int* temp_s, int counter, string* temp, string* file, bool* check, int i, bool func, int counter_wildchar)
{
	string stemp;
	if (temp[counter] == "!")
		return;
	if (temp[counter] == "*")
	{
		if (counter + 1 > counter_wildchar && temp[counter + 1] == "!") { *end = file[i].length()-1; return; }
		stemp = ignore_grep(file[i], func);
		*temp_s = stemp.rfind(temp[counter + 1]);
		if (*temp_s == stemp.npos || *temp_s < *start) { check[i] = false; *end = *end - 1; return; }
		*end = *temp_s + temp[counter + 1].length()-1;
	}
	if (temp[counter] == ".")
	{
		if (*end >= file[i].length()-1) { check[i] = false; return; }
		if (counter + 1 > counter_wildchar && temp[counter + 1] == "!") { if (*end != file[i].length()) { *end = *end + 1; return; } }
		stemp = ignore_grep(file[i], func);
		if (stemp.compare(*end + 2, temp[counter + 1].length(), temp[counter + 1]) != 0) { check[i] = false; *end = *end - 1; return; }
		*end += temp[counter + 1].length() + 1;
	}
	counter = counter + 2;
	sec_grep(start, end, temp_s, counter, temp, file, check, i, func, counter_wildchar);
}

string ignore_grep(string file, bool func)
{
	if (func)
	{
		int i = 0;
		char c;
		string temp;
		temp = file;
		while (temp[i])
		{
			c = temp[i];
			temp[i] = toupper(c);
			i++;
		}
		return temp;
	}
	else
		return file;
}