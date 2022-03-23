#include "constant.h"
#include "tools.h"
void doTee(int argc, char** argv) {
	if (strcmp(argv[1], "--help") == 0) {
		strcpy(gTerm.strout, "Usage: tee [OPTION]... [FILE]...\n");
		strcat(gTerm.strout, "Copy standard input to each FILE, and also to standard output.\n");
		strcat(gTerm.strout, "- a, --append        append to the given FILEs, do not overwrite");
	}
	else {
		strcpy(gTerm.strout, gTerm.strin);

		if (strcmp(argv[1], "-a") == 0) {
			for (int i = 2; i < argc; i++) {
				if (strcmp(argv[i], "-") == 0) {
					strcat(gTerm.strout, gTerm.strin);
				}
				else {
					changechar(argv[i]);
					if (check(argv[i]) == 0) {
						cerr << argv[0] << ":" << argv[i] << ":No such file or directory" << endl;
						return;
					}
					ofstream fout;
					fout.open(argv[i], ios::app);
					fout << gTerm.strin << endl;
					fout.close();
				}
			}
		}
		else {
			for (int i = 1; i < argc; i++) {
				if (strcmp(argv[i], "-") == 0) {
					continue;
				}
				changechar(argv[i]);
				ofstream fout;
				fout.open(argv[i]);
				fout << gTerm.strin << endl;
				fout.close();
			}
		}
	}
}
//OK


