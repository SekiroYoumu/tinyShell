/*Grep ָ�� ���ߣ������ 2021010742
*        �����飺������ΪTinyShell�����һ���֡���grepָ���ָ�����ڲ����ļ������
* �������ַ��������Ѻ��и��ַ�������һ����ʾ�����������������ַ����ԼӴ�����ɫ��ʾ��
*        ����ָ��˵���μ�����ҵ˵���ĵ�
* ������־��
* 2021/12/09 ver alpha 1.0 :��ɺ��������ⲿ�֣���ȱ�ٱ�׼���빦��
* 2021/12/13 ver alpha 1.1 :�����˱�׼���빦�ܺʹ�����ʾ����
* 2021/12/17 ver alpha 1.2 :���������������
* 2022/01/04 ver alpha 1.3 :�޸��˴�������Ĵ������
*/
//����Ϊ������main����
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
	
	//0.������ܵ������1.���������ļ��д����޷��򿪵��ļ���ֱ�ӷ��ش���2.û���ҵ��ļ�������ָ���ȫ������ָ���ȫ�����
	if (argc == 1)
	{
		cerr << "grep:missing operand"<<endl<<"Usage: grep [OPTION]... PATTERNS [FILE]..." << endl << "Try 'grep --help' for more information." << endl;
		return;
	}
	//1.help���ܲ���
	if (!strcmp(argv[1], "--help"))
	{
		help(gTerm);
		return;
	}
	//���ļ��������׼����ת������
	int doccount = -1;
	int position=0;
	bool check = true;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-'&&argv[i][0]!='\0')
		{
			doccount++;//ͳ���ļ���
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