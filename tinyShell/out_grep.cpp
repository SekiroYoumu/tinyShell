#include"constant.h"
#include"grepHeader.h"

//本函数负责输出行
int out_grep(string* file, int** fout, bool* check, bool* func, int countline, Terminal* po, string filename, int lineA, int lineB, int position)
{
	//输出思路
	//1.-c
	if (func[5])
	{
		int sum = 0;
		for (int i = 0; i < countline; i++)
			sum += check[i];
		char* num = new char[10];
		for (int i = 0; i < 10; i++)
			num[i] = '\0';
		int count = 0;
		for (; sum != 0; sum /= 10)
		{
			num[0] = (char)(sum % 10 + 48);
			count++;
		}
		(*po).strout[0] = 'N';
		(*po).strout[1] = 'u';
		(*po).strout[2] = 'm';
		(*po).strout[3] = ':';
		for (int i = 4; i < count+4; i++)
			(*po).strout[i] = num[i-4];
		return count;
	}
	//2.-n,-h/-H,-A,-B
	bool* out = new bool[countline];
	for (int i = 0; i < countline; i++)
		out[i] = check[i];
	for (int i = 0; i < countline; i++)
	{
		if (check[i])
		{
			if (func[3])
			{
				for (int j = 1; j <= lineA; j++)
					if (i >= j)
						out[i - j] = true;
			}
			if (func[4])
			{
				for (int m = 1; m <= lineB; m++)
					if (i + m < countline)
					{
						int temp = i + m;
						out[temp] = true;
					}
			}
		}
	}
	int temppos = position;
	for (int i = 0; i < countline; i++)
	{
		if (out[i] == false)
			continue;
		if (check[i] == false)
		{
			string normal = "\033[0m";
			if (func[0])//输出文件名
			{
				for (int j = 0; j < (int)filename.length(); j++, temppos++)
					(*po).strout[temppos] = filename[j];
				string blue = "\033[94;1m";
				for (int p = 0; p < (int)blue.length(); p++, temppos++)
					(*po).strout[temppos] = blue.at(p);
				(*po).strout[temppos] = '-';
				temppos++;
				string normal = "\033[0m";
				for (int p = 0; p < (int)normal.length(); p++, temppos++)
					(*po).strout[temppos] = normal.at(p);
			}
			if (func[2])//输出行号
			{
				string green = "\033[92;1m";
				for (int p = 0; p < (int)green.length(); p++, temppos++)
					(*po).strout[temppos] = green.at(p);
				char* num = new char[10];
				int templine = i + 1;
				for (int m = 0; m < 10; m++)
					num[m] = '0';
				int count = 0;
				for (; templine != 0; templine /= 10)
				{
					num[0] = (char)(templine % 10 + 48);
					count++;
				}
				for (int i = 0; i < count; i++, temppos++)
					(*po).strout[temppos] = num[i];
				for (int p = 0; p < (int)normal.length(); p++, temppos++)
					(*po).strout[temppos] = normal.at(p);
				string blue = "\033[94;1m";
				for (int p = 0; p < (int)blue.length(); p++, temppos++)
					(*po).strout[temppos] = blue.at(p);
				(*po).strout[temppos] = '-';
				temppos++;
				for (int p = 0; p < (int)normal.length(); p++, temppos++)
					(*po).strout[temppos] = normal.at(p);
			}
			for (int p = 0; p < (int)file[i].length(); p++, temppos++)
				(*po).strout[temppos] = file[i].at(p);
			(*po).strout[temppos] = '\n';
			temppos++;
			continue;
		}
		if (check[i] == true)
		{
			string normal = "\033[0m";
			if (func[0])//输出文件名
			{
				for (int j = 0; j < (int)filename.length(); j++, temppos++)
					(*po).strout[temppos] = filename[j];
				string blue = "\033[94;1m";
				for (int p = 0; p < (int)blue.length(); p++, temppos++)
					(*po).strout[temppos] = blue.at(p);
				(*po).strout[temppos] = '-';
				temppos++;
				string normal = "\033[0m";
				for (int p = 0; p < (int)normal.length(); p++, temppos++)
					(*po).strout[temppos] = normal.at(p);
			}
			if (func[2])//输出行号
			{
				string green = "\033[92;1m";
				for (int p = 0; p < (int)green.length(); p++, temppos++)
					(*po).strout[temppos] = green.at(p);
				char* num = new char[10];
				int templine = i + 1;
				for (int m = 0; m < 10; m++)
					num[m] = '0';
				int count = 0;
				for (; templine != 0; templine /= 10)
				{
					num[0] = (char)(templine % 10 + 48);
					count++;
				}
				for (int i = 0; i < count; i++, temppos++)
					(*po).strout[temppos] = num[i];
				string normal = "\033[0m";
				for (int p = 0; p < (int)normal.length(); p++, temppos++)
					(*po).strout[temppos] = normal.at(p);
				string blue = "\033[94;1m";
				for (int p = 0; p < (int)blue.length(); p++, temppos++)
					(*po).strout[temppos] = blue.at(p);
				(*po).strout[temppos] = '-';
				temppos++;
				for (int p = 0; p < (int)normal.length(); p++, temppos++)
					(*po).strout[temppos] = normal.at(p);
			}
			for (int p = 0; p < fout[i][0]; p++, temppos++)//输出匹配位置前面的字符
				(*po).strout[temppos] = file[i].at(p);
			string red = "\033[91;1m";
			for (int p = 0; p < (int)red.length(); p++, temppos++)//变为红色
				(*po).strout[temppos] = red.at(p);
			for (int p = fout[i][0]; p < fout[i][1] + fout[i][0]; p++, temppos++)
				(*po).strout[temppos] = file[i].at(p);
			for (int p = 0; p < (int)normal.length(); p++, temppos++)//变为白色
				(*po).strout[temppos] = normal.at(p);
			for (int p = fout[i][1] + fout[i][0]; p < (int)file[i].length(); p++, temppos++)
				(*po).strout[temppos] = file[i].at(p);
			(*po).strout[temppos] = '\n';
			temppos++;

		}
	}
	(*po).strout[temppos] = '\0';
	return temppos;
}
