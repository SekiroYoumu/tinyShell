#pragma once
#include "diffHeader.h"
//ͨ������ջ��ý�����
bool printResult(stack<LineRelation> line, char** a, char** b, int n, int m, diffios ios)
{
	memset(gTerm.strout, 0, MAXFILE * sizeof(char));
	string output_buffer; //��stringģ��strout�Ļ�����
	LineRelation now;//now�����м��ϵջ��
	bool change = 0;
	while (line.size() > 0)
	{
		now = line.top();
		if (ios.bl)//��-B,��������
		{
			while ((now.x > 0 && strlen(a[now.x - 1]) == 0) || (now.y > 0 && strlen(b[now.y - 1]) == 0))
			{
				line.pop();
				if (line.size() == 0) return change;//ջ���ˣ�Ҳû�л���Ҫ������У���ֱ�ӽ����ú���
				now = line.top();
			}
		}
		/*���㷨�����֪��һ����ɾ�����������ʡ����ġ���������λΪdʱ����*/
		if (now.op == 'd') //���ȳ��ֵ�Ϊd
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
				else if (now.op == 'a')//������d�����a����ϲ�Ϊc
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
					if (line.size() == 0)//�����ջ��ջ�ѿգ�������û�д��жϵ��м��ϵ�����˳�ѭ����ֱ�ӽ����������
					{
						end = now;
						break;
					}
					tmp = now;
					now = line.top();
					if (ios.bl && ((now.x > 0 && strlen(a[now.x - 1]) == 0) || (now.y > 0 && strlen(b[now.y - 1]) == 0)))//��Ϊ�հ���������
					{
							end=tmp;
							break;
					}
			}
			int end_x = start.x;
			int start_y = end.y;
			/*��string.append()�����򻺳�������������ַ������������ȴ��������ͬ*/
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
			if (ios.I)//��Ҫ��ÿһ�ж������ض��ַ���������Ѽ�¼�仯���н��б���
			{
				if (!change) for (int i = start.x; i <= end_x; i++)
				{
					if (strstr(a[i - 1], ios.target.c_str()) == NULL)
					{
						change = 1;//���ڴ��������δ���ָ��ض��ַ����������У����change=1��������ԭ�����
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
			if (!ios.qk && !ios.I)//���������Ҫ����ֱ�����
			{
				strcat_s(gTerm.strout, output_buffer.c_str());//�������Ƴ���strout
				output_buffer = "";//���������
			}
		}
		else if (now.op == 'a')//���ȳ��ֵ�Ϊa�����㷨�����֪�������ڵ�һ��ȫ��a
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
					if (line.size() == 0) break;//�����ջ��ջ�ѿգ�������û�д��жϵ��м��ϵ�����˳�ѭ����ֱ�ӽ����������
					if (ios.bl && ((line.top().x > 0 && strlen(a[line.top().x - 1]) == 0) || (line.top().y > 0 && strlen(b[line.top().y - 1]) == 0)))//��Ϊ�հ���������
					{
						break;
					}
					now = line.top();
			}
			int end_y = start.y;
			/*��string.append()�����򻺳�������������ַ������������ȴ��������ͬ*/
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
			if (ios.I)//��Ҫ��ÿһ�ж������ض��ַ���������Ѽ�¼�仯���н��б���
			{
				if (!change && strstr(a[start.x - 1], ios.target.c_str()) == NULL)
				{
					change = 1;//���ڴ��������δ���ָ��ض��ַ����������У����change=1��������ԭ�����
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
			strcat_s(gTerm.strout, output_buffer.c_str());//�������Ƴ���strout
			output_buffer = "";//���������
		}
		if (now.op == '=');//�м���ȵĹ�ϵ�����κβ���
		if (line.size() <= 1) break;
		line.pop();
	}
	if (ios.I && change)//��change=1��˵��Ӧ����ԭ�����
	{
		strcat_s(gTerm.strout, output_buffer.c_str());//�������Ƴ���strout
		output_buffer = "";//���������
	}
	return change;
}