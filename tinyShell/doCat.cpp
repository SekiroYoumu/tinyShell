#include "constant.h"
#include "tools.h"
void doCat(int argc, char** argv) {
	if (strcmp(argv[1], "--help") == 0) {
		strcpy(gTerm.strout, "Usage: cat [OPTION]... [FILE]...\n");
		strcat(gTerm.strout, "Concatenate FILE(s) to standard output.\n");
		strcat(gTerm.strout, "-n, --number      number all output lines\n");
		strcat(gTerm.strout, "-b, --number-nonblank    number nonempty output lines, overrides -n\n");
		strcat(gTerm.strout, "-s, --squeeze-blank      suppress repeated empty output lines\n");
		strcat(gTerm.strout, "-E, --show-ends          display $ at end of each line");
	}
	else {
		int n = 0, b = 0, e = 0, s = 0, start = 0, tempnum = 0;
		for (int i = 0; i < argc; i++) {
			if (strcmp(argv[i], "-n") == 0) { n = 1; }
			if (strcmp(argv[i], "-b") == 0) { b = 1; }
			if (strcmp(argv[i], "-E") == 0) { e = 1; }
			if (strcmp(argv[i], "-s") == 0) { s = 1; }
		}
		start = n + b + e + s;
		if (b + n == 2) {
			cerr << " '-n' and '-b'can exist at the same time " << endl;
			return;
		}
		char temp[1000][1000];
		for (int i = start + 1; i < argc; i++) {
			if (strcmp(argv[i], "-") == 0) {
				strcpy(temp[tempnum], gTerm.strin);
				tempnum++;
				continue;
			}
			changechar(argv[i]);
			if (check(argv[i]) == 0) {
				cerr << argv[0] << ":" << argv[i] << ":No such file or directory" << endl;
				return;
			}
			ifstream fin(argv[i]);
			while (fin.getline(temp[tempnum], sizeof(temp[tempnum]), '\n')) {
				tempnum++;
			}
			fin.close();
		}
		if (s == 1) {
			for (int i = 1; i < tempnum; i++) {
				if (strcmp(temp[i], temp[i - 1]) == 0 && strlen(temp[i]) == 0) {
					for (int j = 0; i + j + 1 < tempnum; j++) {
						strcpy(temp[i + j], temp[i + j + 1]);
					}tempnum--;
					i--;
				}
			}
		}
		if (n == 1) {
			for (int i = 0; i < tempnum; i++) {
				daozhi(temp[i]);
				strcat(temp[i], "  ");
				char yici[1000] = {};
				sprintf(yici, "%d", i + 1);
				daozhi(yici);
				strcat(temp[i], yici);
				int yici1 = strlen(yici);
				for (int j = 0; j < 6 - yici1; j++) {
					strcat(temp[i], " ");
				}
				daozhi(temp[i]);
			}
		}
		if (b == 1) {
			int bianhao = 1;
			for (int i = 0; i < tempnum; i++) {
				if (strlen(temp[i]) == 0) {
					continue;
				}
				else {
					daozhi(temp[i]);
					strcat(temp[i], "  ");
					char yici[1000] = {};
					sprintf(yici, "%d", bianhao);
					bianhao++;
					daozhi(yici);
					strcat(temp[i], yici);
					int yici1 = strlen(yici);
					for (int j = 0; j < 6 - yici1; j++) {
						strcat(temp[i], " ");
					}
					daozhi(temp[i]);
				}
			}
		}
		if (e == 1) {
			for (int i = 0; i < tempnum; i++) {
				strcat(temp[i], "$");
			}
		}
		for (int i = 0; i < tempnum; i++) {
			strcat(temp[i], "\n");
		}
		strcpy(gTerm.strout, temp[0]);
		for (int i = 1; i < tempnum; i++) {
			strcat(gTerm.strout, temp[i]);
		}
	}
}
