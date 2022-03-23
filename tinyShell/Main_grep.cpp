#include"constant.h"
#include"grepHeader.h"

int sin_doGrep(int argc, char* targv[], char* doc, int position)
{
	//�����ļ���
	targv[argc - 1] = doc;
	//2.����������grep���ܲ���
	//�����ж���Ҫ�õ��ĺ�������
	bool* func = new bool[7];//�洢�ж�
	int lineA = 0, lineB = 0;
	int* plineA = &lineA;
	int* plineB = &lineB;
	for (int i = 0; i < 7; i++)
		func[i] = 0;
	for (int i = 1; i < argc - 2; i++)
		if (cprfunc(targv[i], func, plineA, plineB))
			return -256;
	//�����ģ�崮���зֿ�
	string original;
	int counter_wildchar = 0;
	int* c = &counter_wildchar;
	string* temp = new string[100];
	Pre_Grep(argc, targv, func, original, c, temp);
	//��������ļ����ݣ��Ƚϲ����
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
		if (filename != "-")//���������ڼ������ļ������
		{
			cerr << "grep:" << doc << ":" << "No such file or directory. " << endl;
			return -256;
		}
		//���������ڴ����׼��������
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

//������Ϊ�жϹ���
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

//������Ϊ����ָ��
void help(Terminal gTerm)
{
	strncpy_s(gTerm.strout, 500, "grep ָ�����ڲ������ݰ���ָ���ķ�����ʽ���������з�����ʽ����һ����ʾ������\n����ָ���κ��ļ����ƣ�������������ļ���Ϊ - ���� grep ָ���ӱ�׼�����豸��ȡ���ݡ�\nѡ��˵����\n--help:��ʾ����\n-c�� ���������ʽ��������\n-h�� ����ʾ������ʽ����һ��֮ǰ������ʾ�����������ļ����ơ�\n-H�� ����ʾ������ʽ����һ��֮ǰ�� ��ʾ�����������ļ����ơ�\n-i�� �����ַ���Сд�Ĳ��\n-n�� ����ʾ������ʽ����һ��֮ǰ����ʾ�����еı��\n-A(-B)[����]�� ������ʾ���Ϸ�����ʽ����һ��֮�⣬ ͬʱ��ʾ����֮��(֮ǰ)��[����]�����ݡ�\n", 499);
	cout << gTerm.strout;
}

//������Ϊ����ģ�崮
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
	//��ֹԽ�磬����鵽������ʱ����
	//���ｫģʽ����Ϊ���ɶΣ����ڶ��ļ����ֶε�ƥ�䡣
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
	//����,temp���鴢����ÿ����Ҫƥ����ַ��Σ�����Ϳ��Զ��ļ�����ƥ������
}

//���������ļ��ڵ��ַ������бȽ�
void Main_Grep(int counter_wildchar, string* temp, int filelen, string* file, int** fout, bool* check, bool func)
{

	//�Եڣ�i+1���н��м��
	for (int i = 0; i < filelen; i++)
	{
		//��һ�����������ͨ�������ģʽ��
		if (counter_wildchar == 0)
		{
			string stemp;
			stemp = ignore_grep(file[i], func);//�жϴ�Сд
			string::size_type result = stemp.find(temp[0]);//����ͨ���ʱ��temp�ĵ�һ���ַ�����Ϊģʽ��
			if (result == stemp.npos) { check[i] = false; continue; }
			fout[i][1] = temp[0].length();//�ַ�������
			fout[i][0] = result;//��һ�������ַ�����λ��
			check[i] = true;
			continue;
		}
		//�ڶ����������ͨ�����һ�������
		// ���˼·�������������ָ�롱��ʾ��ǰѡȡ����
		// ��ʼʱ������ָ���λ��������1����ָ���������Σ�
		// ���ڵ�һ�������ķ�ͨ����ַ������Ȳ��ң����ҵ�������ָ������ַ����ף���βָ������ַ���β��
		// ���ڵ�һ�������ġ�*����ָ�벻�䣬����ͬ��
		// ���ڵ�һ�������ġ�.������ָ������ַ����׵�ǰһ��λ�ã�����ͬ��
		// ��ȡģʽ���ĵ�i���ַ���(��counter����)��
		// ����ַ�����'*'���ı�����ָ���λ�ã�ֱ�Ӷ�ȡ��һ���ַ������������ҵ�ʱ��βָ��λ�ø�Ϊ���������ַ���β
		// ����ַ�����'.'���ı���ָ��λ�ã���βָ��λ�ø�Ϊ��һ��λ�ò�������ǰλ�ö�Ӧ�����Σ������βָ��λ�ø�Ϊ���������ַ���β
		// ������û��ƥ���ַ��ε����ʱ���ٴν��дӵ�ǰ�Ѳ�״̬�ġ���һ��ƥ���ַ��ε�ĩβ�����в��ң�ֱ���Ҳ���ƥ����ַ��Σ����ж�ֵ��Ϊ-1������
		// ����鵽���һ��Ҳû���˳�������ȡ���ˡ�������ʱ������ǰ����µ���ָ����βָ��������飬�����ж�ֵ��Ϊ-1
		//��������£��ַ������ȱ������1
		if (file[i].length() == 1) { check[i] = false; continue; }
		int start = 0, end = 0, first = 0, last = file[i].length(), temp_s, counter = 0;
		int* pstart = &start;
		int* pend = &end;
		int* ptemp_s = &temp_s;
		check[i] = true;
		for (;;)
		{
			//�Ե�һ���ַ������ж�
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
			//����ִ��һ����
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