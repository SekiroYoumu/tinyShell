#pragma once
#include "diffHeader.h"
//通过调用栈获得结果输出
bool printResult(stack<LineRelation> line, char** a, char** b, int n, int m, diffios ios)
{
	memset(gTerm.strout, 0, MAXFILE * sizeof(char));
	string output_buffer; //用string模拟strout的缓冲区
	LineRelation now;//now代表行间关系栈顶
	bool change = 0;
	while (line.size() > 0)
	{
		now = line.top();
		if (ios.bl)//若-B,跳过空行
		{
			while ((now.x > 0 && strlen(a[now.x - 1]) == 0) || (now.y > 0 && strlen(b[now.y - 1]) == 0))
			{
				line.pop();
				if (line.size() == 0) return change;//栈空了，也没有还需要输出的行，故直接结束该函数
				now = line.top();
			}
		}
		/*由算法规则可知，一定先删除再新增，故“更改”情形在首位为d时讨论*/
		if (now.op == 'd') //首先出现的为d
		{
			if (!ios.I)
				change = 1;
			char op = 'd';
			bool dflag = 0, aflag = 0;
			LineRelation start, add_start, end, tmp;
			start = now;
			while (line.size() > 0)
			{
				if (now.op == 'd')
				{
					if (!ios.I)
						change = 1;
					if (now.x != start.x)
						dflag = 1;
				}
				else if (now.op == 'a')//若出现d后出现a，则合并为c
				{
					if (!ios.I)
						change = 1;
					if (aflag == 0)
					{
						op = 'c';
						add_start = now;
						aflag = 1;
					}
				}
				if (now.op == '=')
				{
					end = tmp;
					break;
				}
					line.pop();
					if (line.size() == 0)//如果弹栈后栈已空，代表已没有待判断的行间关系，故退出循环，直接进入输出部分
					{
						end = now;
						break;
					}
					tmp = now;
					now = line.top();
					if (ios.bl && ((now.x > 0 && strlen(a[now.x - 1]) == 0) || (now.y > 0 && strlen(b[now.y - 1]) == 0)))//若为空白行则跳出
					{
							end=tmp;
							break;
					}
			}
			int end_x = start.x;
			int start_y = end.y;
			/*用string.append()函数向缓冲区中依次添加字符（串），并等待输出，后同*/
			output_buffer.append(to_string(start.x));
			if (dflag == 1)
			{
				if (aflag == 1)  end_x = add_start.x;
				else end_x = end.x;
				output_buffer.append(1, ',');
				output_buffer.append(to_string(end_x));
			}
			output_buffer.append(1, op);
			if (aflag == 1 && add_start.y != end.y)
			{
				start_y = add_start.y;
				output_buffer.append(to_string(start_y));
				output_buffer.append(1, ',');
			}
			output_buffer.append(to_string(end.y));
			output_buffer.append(1, '\n');
			for (int i = start.x; i <= end_x; i++)
			{
				output_buffer.append(1, '<');
				output_buffer.append(a[i - 1]);
				output_buffer.append(1, '\n');
			}
			if (end_x == n)
				output_buffer.append("\\ No newline at end of file\n");
			if (op == 'c')
			{
				output_buffer.append("---\n");
				for (int j = start_y; j <= end.y; j++)
				{
					output_buffer.append(1, '>');
					output_buffer.append(b[j - 1]);
					output_buffer.append(1, '\n');
				}
				if (end.y == m)
					output_buffer.append("\\ No newline at end of file\n");
			}
			if (ios.I)//若要求每一行都包含特定字符串，则对已记录变化的行进行遍历
			{
				if (!change) for (int i = start.x; i <= end_x; i++)
				{
					if (strstr(a[i - 1], ios.target.c_str()) == NULL)
					{
						change = 1;//若在此情况下有未出现该特定字符串的区别行，则记change=1，后续按原样输出
						break;
					}
				}
				if (!change) for (int i = start_y; i <= end.y; i++)
				{
					if (strstr(b[i - 1], ios.target.c_str()) == NULL)
					{
						change = 1;
						break;
					}
				}
			}
			if (!ios.qk && !ios.I)//若对输出无要求，则直接输出
			{
				strcat_s(gTerm.strout, output_buffer.c_str());//缓冲区推出至strout
				output_buffer = "";//缓冲区清空
			}
		}
		else if (now.op == 'a')//首先出现的为a，由算法规则可知后续相邻的一定全是a
		{
			if (!ios.I)
				change = 1;
			LineRelation start, tmp;
			start = now;
			tmp = now;
			while (line.size() > 0)
			{
				if (now.op != 'a') break;
				if (!ios.I)
					change = 1;
					tmp = now;
					line.pop();
					if (line.size() == 0) break;//如果弹栈后栈已空，代表已没有待判断的行间关系，故退出循环，直接进入输出部分
					if (ios.bl && ((line.top().x > 0 && strlen(a[line.top().x - 1]) == 0) || (line.top().y > 0 && strlen(b[line.top().y - 1]) == 0)))//若为空白行则跳出
					{
						break;
					}
					now = line.top();
			}
			int end_y = start.y;
			/*用string.append()函数向缓冲区中依次添加字符（串），并等待输出，后同*/
			output_buffer.append(to_string(start.x));
			output_buffer.append(1, 'a');
			output_buffer.append(to_string(start.y));
			if (tmp.y != start.y)
			{
				if (now.op == 'a') end_y = now.y;
				else end_y = tmp.y;
				output_buffer.append(1, ',');
				output_buffer.append(to_string(end_y));
				output_buffer.append(1, '\n');
			}
			else output_buffer.append(1, '\n');
			for (int i = start.y; i <= end_y; i++)
			{
				output_buffer.append(1, '>');
				output_buffer.append(b[i - 1]);
				output_buffer.append(1, '\n');
			}
			if (end_y == m)
				output_buffer.append("\\ No newline at end of file\n");
			if (ios.I)//若要求每一行都包含特定字符串，则对已记录变化的行进行遍历
			{
				if (!change && strstr(a[start.x - 1], ios.target.c_str()) == NULL)
				{
					change = 1;//若在此情况下有未出现该特定字符串的区别行，则记change=1，后续按原样输出
				}
				if (!change) for (int i = start.y; i <= end_y; i++)
				{
					if (strstr(b[i - 1], ios.target.c_str()) == NULL)
					{
						change = 1;
						break;
					}
				}
			}
		}
		if (!ios.qk && !ios.I)
		{
			strcat_s(gTerm.strout, output_buffer.c_str());//缓冲区推出至strout
			output_buffer = "";//缓冲区清空
		}
		if (now.op == '=');//行间相等的关系不做任何操作
		if (line.size() <= 1) break;
		line.pop();
	}
	if (ios.I && change)//记change=1，说明应当按原样输出
	{
		strcat_s(gTerm.strout, output_buffer.c_str());//缓冲区推出至strout
		output_buffer = "";//缓冲区清空
	}
	return change;
}