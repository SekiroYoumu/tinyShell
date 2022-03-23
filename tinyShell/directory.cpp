#include "constant.h"
#include "tools.h"
void doCd(int argc, char** argv) {
	if (strcmp(argv[1], "--help") == 0) {
		strcpy(gTerm.strout,"Usage: cd [directory]\n");
		strcat(gTerm.strout, "Where, the [path] starts with '/'as the absolute path, otherwise as the relative path.in addition, '.' Represents the same directory, '..'indicates the previous directory.\n");
		strcat(gTerm.strout, "Change the shell working directory.\n");
		strcat(gTerm.strout, "Change the current directory to DIR. The default DIR is the value of theHOME shell variable.");
	}
	else {
		if (argv[1][0] == '/') {
			strcpy(gTerm.wdir, argv[1]);
		}
		else {
			if (strcmp(gTerm.wdir, "/") == 0) {//无了
				strcat(gTerm.wdir, argv[1]);
			}
			else {
				strcat(gTerm.wdir, "/");
				strcat(gTerm.wdir, argv[1]);
			}
		}
		char stack[100][100] = {};//无了
		int p = 0;//无了
		int gangshu = 0;//无了
		int gangweizhi[1000] = { 0 };
		for (int i = 0; i < strlen(gTerm.wdir); i++) {
			if (gTerm.wdir[i] == '/') {
				gangweizhi[gangshu] = i;
				gangshu++;
			}
		}
		for (int i = 0; i < gangshu - 1; i++) {
			//无了
			for (int j = 0; j < (gangweizhi[i + 1] - gangweizhi[i]); j++) {
				stack[p][j] = gTerm.wdir[gangweizhi[i] + j];
			}
			p++;
			if (strcmp(stack[p - 1], "/..") == 0) {
				if (p - 2 <= 0) {
					strcpy(gTerm.wdir, "/");
					return;
				}
				memset(stack[p - 1], '\0', sizeof(stack[p - 1]));
				memset(stack[p - 2], '\0', sizeof(stack[p - 2]));//无了
				p -= 2;
			}
			if (strcmp(stack[p - 1], "/.") == 0) {
				memset(stack[p - 1], '\0', sizeof(stack[p - 1]));
				p -= 1;
			}
		}
		for (int i = 0; i < strlen(gTerm.wdir) - gangweizhi[gangshu - 1]; i++) {
			stack[p][i] = gTerm.wdir[gangweizhi[gangshu - 1] + i];
		}
		p++;
		if (strcmp(stack[p - 1], "/..") == 0) {
			if (p - 2 <= 0) {
				strcpy(gTerm.wdir, "/");
				return;
			}
			strcpy(stack[p - 1], "");
			strcpy(stack[p - 2], "");
			p -= 2;
		}
		if (strcmp(stack[p - 1], "/.") == 0) {
			memset(stack[p - 1], '\0', sizeof(stack[p - 1]));
			p -= 1;
		}
		if (p == 0) {
			strcpy(gTerm.wdir, "/");
		}
		else {
			for (int i = 0; i < p; i++) {
				if (i == 0) {
					strcpy(gTerm.wdir, stack[i]);
				}
				else {
					strcat(gTerm.wdir, stack[i]);
				}
			}
		}
	}

}//OK

void doPwd(int argc, char** argv) {//
	if (argc==2) {
		strcpy(gTerm.strout, "Usage: pwd\n");
		strcat(gTerm.strout, "Print the name of the current working directory.");
	}
	else {
		strcpy(gTerm.strout, gTerm.wdir);
	}
}//OK