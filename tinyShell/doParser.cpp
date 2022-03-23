#include "parserHeader.h"
#include "constant.h"
int argc0 = 0;
char** argv0;
int cnt = 0;
int argc = 0;
char** argv;
void clearStr(char* a) {
    memset(a, 0, MAXFILE * sizeof(char));
}
//输出较长strout使用该函数
void cout_alter(char* out)
{
    int i = 0;
    while (true)
    {
        if (out[i] == '\0') break;
        putchar(out[i]);
        i++;
    }
}
void clearArg(char* a) {
    memset(a, 0, MAXARG * sizeof(char));
}
//复合指令的动作
int findCom() {//找到目前argv[][]中最靠前的 复合指令的位置
    int temp = 0;
    for (int i = 0; i < argc0; i++) {
        if (!strcmp(argv0[i], "|")) {
            temp = i;
            break;
        }
    }
    return temp;
}

int findSpace() {
    for (int i = 0; i < MAXLINE; i++) {
        if (gTerm.strin[i] == ' ') {
            return i;
        }
    }
    return -1;
}

bool doEcho(int argc, char** argv) {
    //如果没有复合指令
    clearStr(gTerm.strout);
    int y = 0;
    for (int i = 1; i < argc - 1; i++) {
        for (int j = 0; j < strlen(argv[i]); j++) {
            gTerm.strout[y] = argv[i][j];
            y++;
        }
        gTerm.strout[y] = ' ';
        y++;
    }
    //考虑最后一个argv
    for (int j = 0; j < strlen(argv[argc - 1]); j++) {
        gTerm.strout[y] = argv[argc - 1][j];
        y++;
    }
    gTerm.strout[y] = '\n';
    y++;
    gTerm.strout[y] = '\0';
    return true;
}


void doInit() {
    cout << "Machine Name:";
    cin >> gTerm.mach;
    cout << "Root Directory:";
    cin >> gTerm.root;
    gTerm.wdir[0] = '/';
    cout << "Login:";
    cin >> gTerm.user;
    system("cls");
}

void info() {
    if (cnt > 0) {
        cout << "\033[92;1m" << gTerm.user << "@" << gTerm.mach << ":";
        cout << "\033[94;1m" << gTerm.wdir << "\033[0m" << "$";
        //cout << address[0];
    }

}

void read() {
    char  buf[MAXLINE + 1];
    cin.getline(buf, MAXLINE);

    int x = 0;
    int y = 0;

    for (int i = 0; i <= strlen(buf); i++) {
        if ((buf[i] == 32) || (buf[i] == '\0')) {
            argv0[argc0][x] = '\0';
            x = 0;
            y++;
            argc0++;
        }
        else {
            argv0[argc0][x] = buf[y];
            x++;
            y++;
        }
    }
    for (int i = argc0; i < 100; i++) {
        clearArg(argv0[i]);
    }
    //复合指令怎么写
}

void analyze(int argc, char** argv) {//分析argv中的字符串
    //指令有diff,grep,tee,cat,cp,cd,pwd,echo

    int temp = findCom();//这是复合指令的位置，如果没有复合指令，结果为0
    //如果有复合指令
    if (temp != 0) {
        for (int i = 0; argv0[i][0] != '\0' && i < temp; i++) {
            strcpy_s(argv[i], MAXARG, argv0[i]);
        }
        argc = temp;
    }
    else {
        //如果没有复合指令
        for (int i = 0; argv0[i][0] != '\0' && i < 100; i++) {
            strcpy_s(argv[i], MAXARG, argv0[i]);
        }
        argc = argc0;
    }
    //清空不用的argv
    for (int i = argc; i < 100; i++) {
        memset(argv[i], 0, sizeof(char) * MAXARG);
    }

    if (!strcmp(argv0[0], "diff")) {
        doDiff(argc, argv);
    }
    else if (!strcmp(argv0[0], "grep")) {
        doGrep(argc, argv);
    }
    else if (!strcmp(argv0[0], "tee")) {
        doTee(argc, argv);
    }
    else if (!strcmp(argv0[0], "cat")) {
        doCat(argc, argv);
    }
    else if (!strcmp(argv0[0], "cp")) {
        doCp(argc, argv);
    }
    else if (!strcmp(argv0[0], "cd")) {
        doCd(argc, argv);
    }
    else if (!strcmp(argv0[0], "pwd")) {
        doPwd(argc, argv);
    }
    else if (!strcmp(argv0[0], "echo")) {
        doEcho(argc, argv);
    }
    else if (!strcmp(argv0[0], "cls")) {
        system("cls");
    }
    else if (cnt != 0) {
        cout << "Command '" << argv0[0] << "' not found" << endl;
        clearStr(gTerm.strin);
    }

    //复合指令部分

    if (temp) {
        clearArg(argv0[temp]);
        int i1 = 0;
        //cout << "复合指令在第" << temp << "个位置" << endl;
        for (; i1 + temp + 1 < argc0; i1++) {
            strcpy_s(argv0[i1], MAXARG, argv0[i1 + temp + 1]);
            clearArg(argv0[i1 + temp + 1]);
        }
        argc0 = i1;
        clearStr(gTerm.strin);
        strcpy_s(gTerm.strin, MAXFILE, gTerm.strout);
        clearStr(gTerm.strout);
        analyze(argc, argv);
    }

}
void doParser()
{
    argv0 = new char* [100];
    for (int i = 0; i < 100; i++) {
        argv0[i] = new char[MAXARG];
    }

    argv = new char* [100];
    for (int i = 0; i < 100; i++) {
        argv[i] = new char[MAXARG];
    }

    doInit();
    do {
        clearStr(gTerm.strin);
        clearStr(gTerm.strout);
        info();
        read();

        if (!strcmp(argv0[0], "exit")) {
            break;
        }

        analyze(argc, argv);


        int outLen = strlen(gTerm.strout);


        //if (cnt > 0 && gTerm.strout[0] != '\0' && gTerm.strout[1] != '\0' && gTerm.strout[2] != '\0') {// 如果标准输出的前三个字符都不是空字符
        //    cout << gTerm.strout;
        //    // cout << endl;
        //}

        if (cnt > 0 && gTerm.strout[0] != 0 && gTerm.strout[1] != 0 && gTerm.strout[2] != 0) {
            if (gTerm.strout[outLen - 1] == '\n') {
                if(outLen < 2048) cout << gTerm.strout;
                else cout_alter(gTerm.strout);
            }
            else {
                if (outLen < 2048) cout << gTerm.strout << endl;
                else {
                    cout_alter(gTerm.strout);
                    putchar('\n');
                }
            }
        }



        clearStr(gTerm.strout);
        for (int i = 0; i < argc0; i++) {
            clearArg(argv0[i]);
            clearArg(argv[i]);
        }



        argc0 = 0;
        argc = 0;
        //重置argc，argv;
        cnt++;
    } while (true);
    return;
}
