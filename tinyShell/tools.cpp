#include "constant.h"
void changechar(char a[MAXLINE]) {
	char r[MAXLINE], w[MAXLINE];
	strcpy(r, gTerm.root);
	strcpy(w, gTerm.wdir);
	if (a[0] == '/') {//�Ը�Ŀ¼��ͷ
		strcat(r, a);
	}
	else {//�����·����ͷ
		strcat(r, "/");
		strcat(r, w);
		strcat(r, "/");
		strcat(r, a);
	}
	strcpy(a, r);
	return;
}

bool check(char a[MAXLINE]) {
	ifstream fin(a);
	if (!fin) {
		cerr << "no";
		fin.close();
		return 0;
	}
	else {
		fin.close();
		return 1;
	}
}

//���ַ�������
void daozhi(char target[]) {
	char tempn[MAXLINE] = {};
	int tn = 0;
	tn = strlen(target);
	for (int i1 = tn - 1, i2 = 0; i1 >= 0; i1--, i2++) {
		tempn[i2] = target[i1];
	}
	strcpy(target, tempn);
}