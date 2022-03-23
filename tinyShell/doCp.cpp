#include "constant.h"
#include "tools.h"
void doCp(int argc, char** argv) {
	char linshi[1000] = {};
	if (strcmp(argv[1], "--help") == 0) {
		strcpy(gTerm.strout, "Usage: cp [OPTION]... [FILE]...\n");
		strcat(gTerm.strout, "cp is used to copy files\n");
		strcat(gTerm.strout, "-n, --no-clobber   do not overwrite an existing file (overridesa previous - i option)");
	}
	else if (strcmp(argv[1], "-n") == 0) {

		if (strcmp(argv[2], "-") == 0 && strcmp(argv[3], "-") == 0) {
			strcat(gTerm.strout, gTerm.strin);
		}
		else if (strcmp(argv[2], "-") == 0 && strcmp(argv[3], "-") != 0) {
			changechar(argv[3]);
			if (check(argv[3]) == 0) {
				cerr << argv[0] << ":" << argv[3] << ":No such file or directory" << endl;
				return;
			}
			ofstream fout(argv[3], ios::app);
			fout << "\n";
			fout << gTerm.strin;
			fout.close();
		}
		else if (strcmp(argv[2], "-") != 0 && strcmp(argv[3], "-") == 0) {
			changechar(argv[2]);
			if (check(argv[2]) == 0) {
				cerr << argv[0] << ":" << argv[2] << ":No such file or directory" << endl;
				return;
			}
			ifstream fin(argv[2]);
			while (fin.getline(linshi, sizeof(linshi), '\n')) {
				strcat(gTerm.strout, linshi);
			}
			fin.close();
		}
		else {
			changechar(argv[2]);
			if (check(argv[2]) == 0) {
				cerr << argv[0] << ":" << argv[2] << ":No such file or directory" << endl;
				return;
			}
			changechar(argv[3]);
			if (check(argv[3]) == 0) {
				cerr << argv[0] << ":" << argv[3] << ":No such file or directory" << endl;
				return;
			}
			ifstream fin(argv[2]);
			ofstream fout(argv[3], ios::app);
			while (fin.getline(linshi, sizeof(linshi), '\n')) {
				fout << linshi << endl;
			}fout.close();
			fin.close();
		}
	}
	else {
		if (strcmp(argv[1], "-") == 0 && strcmp(argv[2], "-") == 0) {
			strcpy(gTerm.strout, gTerm.strin);
		}
		else if (strcmp(argv[1], "-") == 0 && strcmp(argv[2], "-") != 0) {//Ö»ÄÜÓÃ£¡=0
			changechar(argv[2]);
			if (check(argv[2]) == 0) {
				cerr << argv[0] << ":" << argv[2] << ":No such file or directory" << endl;
				return;
			}
			ofstream fout(argv[2]);
			fout << gTerm.strin;
			fout.close();
		}
		else if (strcmp(argv[1], "-") != 0 && strcmp(argv[2], "-") == 0) {
			changechar(argv[1]);
			if (check(argv[1]) == 0) {
				cerr << argv[0] << ":" << argv[1] << ":No such file or directory" << endl;
				return;
			}
			ifstream fin(argv[1]);
			while (fin.getline(linshi, sizeof(linshi), '\n')) {
				strcpy(gTerm.strout, linshi); strcat(gTerm.strout, linshi);
			}
			fin.close();
		}
		else {
			changechar(argv[1]);
			if (check(argv[1]) == 0) {
				cerr << argv[0] << ":" << argv[1] << ":No such file or directory" << endl;
				return;
			}
			changechar(argv[2]);
			if (check(argv[2]) == 0) {
				cerr << argv[0] << ":" << argv[2] << ":No such file or directory" << endl;
				return;
			}
			ifstream fin(argv[1]);
			ofstream fout(argv[2]);
			while (fin.getline(linshi, sizeof(linshi))) {
				fout << linshi << endl;
			}
			fout.close();
			fin.close();
		}
	}
}//OK
