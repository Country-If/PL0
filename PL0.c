// #define _CRT_SECURE_NO_WARNINGS
// #pragma warning(disable:4996)

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "pl0.h"
#include "string.h"
/*解释执行时使用的栈*/
#define stacksize 500

int main() {
    printf("开始调试时间：");
    print_time();

    printf("信息安全1班\t3119005436\t徐国涛\n\n");

    bool nxtlev[symnum];
    printf("Input pl/0 file?");
    scanf("%s", fname);               /*输入文件名*/
    fin = fopen(fname, "r");
    if (fin) {
        printf("List object code?(Y/N)");            /*是否输出虚拟机代码*/
        scanf("%s", fname);
        listswitch = (fname[0] == 'y' || fname[0] == 'Y');
        printf("List symbol table?(Y/N)");             /*是否输出名字表*/
        scanf("%s", fname);
        tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
        fa1 = fopen("fa1.tmp", "w");
        fprintf(fa1, "input pl/0 file?");
        fprintf(fa1, "%s\n", fname);
        init();
        err = 0;
        cc = cx = ll = 0;
        ch = ' ';
        if (-1 != getsym()) {
            fa = fopen("fa.tmp", "w");
            fas = fopen("fas.tmp", "w");
            addset(nxtlev, declbegsys, statbegsys, symnum);
            nxtlev[period] = true;
            if (-1 == block(0, 0, nxtlev)) /*调用编译程序*/
            {
                fclose(fa);
                fclose(fa1);
                fclose(fas);
                fclose(fin);
                printf("\n");
                return 0;
            }
            fclose(fa);
            fclose(fa1);
            fclose(fas);
            if (sym != period) {
                error(9);
            }
            if (err == 0) {
                fa2 = fopen("fa2.tmp", "w");
                interpret();    /*调用解释执行程序*/
                fclose(fa2);
            }
            else {
                printf("Errors in pl/0 program");
            }
        }
        fclose(fin);
    }
    else {
        printf("Can't Open file!\n");
    }
    printf("\n");

    printf("结束调试时间：");
    print_time();
    printf("\n");

    system("pause");
    return 0;
}

/*
 * 打印当前时间
 */
void print_time() {
    char time_len[20] = {0};

    time_t timep;
    time(&timep);

    struct tm *tmp_pointer;
    tmp_pointer = gmtime(&timep);

    snprintf(time_len, 20, "%d-%d-%d %d:%d:%d", 1900 + tmp_pointer->tm_year, 1 + tmp_pointer->tm_mon,
             tmp_pointer->tm_mday, 8 + tmp_pointer->tm_hour, tmp_pointer->tm_min, tmp_pointer->tm_sec);
    printf("%s\n", time_len);
}

// 初始化
void init() {
    int i;
    for (i = 0; i <= 255; i++) {
        ssym[i] = nul;
    }

    // 设置单字符符号
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['='] = eql;
    ssym[','] = comma;
    ssym['.'] = period;
//    ssym['#'] = neq;
    ssym[';'] = semicolon;

    /* 设置保留字名字,按照字母顺序，便于折半查找 */     /*ASCII中，大写字母在小写字母之前*/
    strcpy(&(word[0][0]), "DOWNTO");
    strcpy(&(word[1][0]), "ELSE");
    strcpy(&(word[2][0]), "FOR");
    strcpy(&(word[3][0]), "RETURN");
    strcpy(&(word[4][0]), "TO");
    strcpy(&(word[5][0]), "begin");
    strcpy(&(word[6][0]), "call");
    strcpy(&(word[7][0]), "char");
    strcpy(&(word[8][0]), "const");
    strcpy(&(word[9][0]), "do");
    strcpy(&(word[10][0]), "end");
    strcpy(&(word[11][0]), "if");
    strcpy(&(word[12][0]), "int");
    strcpy(&(word[13][0]), "odd");
    strcpy(&(word[14][0]), "procedure");
    strcpy(&(word[15][0]), "read");
    strcpy(&(word[16][0]), "real");
    strcpy(&(word[17][0]), "then");
    strcpy(&(word[18][0]), "var");
    strcpy(&(word[19][0]), "while");
    strcpy(&(word[20][0]), "write");

    /* 设置保留字符号 */
    wsym[0] = DOWNTOSYM;
    wsym[1] = ELSESYM;
    wsym[2] = FORSYM;
    wsym[3] = RETURNSYM;
    wsym[4] = TOSYM;
    wsym[5] = beginsym;
    wsym[6] = callsym;
    wsym[7] = charsym;
    wsym[8] = constsym;
    wsym[9] = dosym;
    wsym[10] = endsym;
    wsym[11] = ifsym;
    wsym[12] = intsym;
    wsym[13] = oddsym;
    wsym[14] = procsym;
    wsym[15] = readsym;
    wsym[16] = realsym;
    wsym[17] = thensym;
    wsym[18] = varsym;
    wsym[19] = whilesym;
    wsym[20] = writesym;


    /* 设置指令名称 */
    strcpy(&(mnemonic[lit][0]), "lit");
    strcpy(&(mnemonic[opr][0]), "opr");
    strcpy(&(mnemonic[lod][0]), "lod");
    strcpy(&(mnemonic[sto][0]), "sto");
    strcpy(&(mnemonic[cal][0]), "cal");
    strcpy(&(mnemonic[inte][0]), "int");
    strcpy(&(mnemonic[jmp][0]), "jmp");
    strcpy(&(mnemonic[jpc][0]), "jpc");
    strcpy(&(mnemonic[sta][0]), "sta");
    strcpy(&(mnemonic[lda][0]), "lda");
    strcpy(&(mnemonic[ack][0]), "ack");
    strcpy(&(mnemonic[str][0]), "str");
    strcpy(&(mnemonic[ldr][0]), "ldr");

    /* 设置符号集 */
    for (i = 0; i < symnum; i++) {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /* 设置声明开始符号集 */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[procsym] = true;

    /* 设置语句开始符号集 */
    statbegsys[beginsym] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;


    /* 设置因子开始符号集 */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    facbegsys[intsym] = true;
    facbegsys[charsym] = true;
    facbegsys[realsym] = true;
}

/*
* 用数组实现集合的集合运算
*/
int inset(int e, bool *s) {
    return s[e];
}

int addset(bool *sr, bool *s1, bool *s2, int n) {
    int i;
    for (i = 0; i < n; i++) {
        sr[i] = s1[i] || s2[i];
    }
    return 0;
}

int subset(bool *sr, bool *s1, bool *s2, int n) {
    int i;
    for (i = 0; i < n; i++) {
        sr[i] = s1[i] && (!s2[i]);
    }
    return 0;
}

int mulset(bool *sr, bool *s1, bool *s2, int n) {
    int i;
    for (i = 0; i < n; i++) {
        sr[i] = s1[i] && s2[i];
    }
    return 0;
}

/*
*   出错处理，打印出错位置和错误编码
*/
void error(int n) {
    char space[81];
    memset(space, 32, 81);
    space[cc - 1] = 0;//出错时当前符号已经读完，所以cc-1
    printf("****%s!%d\n", space, n);
    fprintf(fa1, "****%s!%d\n", space, n);
    err++;
}

/*
漏掉空格，读取一个字符。
每次读一行，存入line缓冲区，line被getsym取空后再读一行。
被函数getsym调用。
*/
int getch() {
    if (cc == ll) {
        if (feof(fin)) {
            printf("program incomplete");
            return -1;
        }
        ll = 0;
        cc = 0;
        printf("%d", cx);
        fprintf(fa1, "%d", cx);
        ch = ' ';
        while (ch != 10) {
            //fscanf(fin,"%c",&ch)
            if (EOF == fscanf(fin, "%c", &ch)) {
                line[ll] = 0;
                break;
            }
            printf("%c", ch);
            fprintf(fa1, "%c", ch);
            line[ll] = ch;
            ll++;
        }
        printf("\n");
        fprintf(fa1, "\n");
    }
    ch = line[cc];
    cc++;
    return 0;
}

/*
*词法分析，获取一个符号
*/
int getsym() {
    int i, j, k;
    while (ch == ' ' || ch == 10 || ch == 9)            /*忽略空格、换行和TAB*/
    {
        getchdo;
    }
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {                    /*名字或保留字以a~z开头*/
        k = 0;
        do {
            if (k < al) {
                a[k] = ch;
                k++;
            }
            getchdo;
        } while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'Z');
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do {                /*搜索当前符号是否为保留字*/
            k = (i + j) / 2;
            if (strcmp(id, word[k]) <= 0) {
                j = k - 1;
            }
            if (strcmp(id, word[k]) >= 0) {
                i = k + 1;
            }
        } while (i <= j);
        if (i - 1 > j) {
            sym = wsym[k];
        }
        else {
            sym = ident;        /*搜索失败，则是名字或数字*/
        }
    }
    else {
        if (ch >= '0' && ch <= '9') {                /*检测是否为数字：以0~9开头*/
            k = 0;
            num = 0;
            int_part = 0;
            dec_part = 0;
//            sym = number;
            sym = intsym;
            do {
                if (ch == '.') {
                    getchdo;
                    sym = realsym;
                    while (ch >= '0' && ch <= '9') {
                        dec_part = 10 * dec_part + ch - '0';        // 计算小数部分
                        k++;
                        getchdo;
                    }
                    break;
                }
                else {
                    num = 10 * num + ch - '0';
                    int_part = 10 * int_part + ch - '0';        // 计算整数部分
                    k++;
                    getchdo;
                }
            } while (ch >= '0' && ch <= '9' || ch == '.');/*获取数字的值*/
            k--;
            if (k > nmax) {
                error(30);
            }
        }
        else {
            if ((int) ch == 39) {        // 字符以 ' 开头
                getchdo;
                if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z') {
                    num = (int)ch;
                    getchdo;
                    if ((int)ch == 39) {
                        sym = charsym;
                    }
                    else {
                        num = 0;
                        sym = nul;
                        error(105);         // 自定义报错，字符类型不完整
                    }
                }
                else {
                    sym = nul;
                    error(105);
                }
                getchdo;
            }
            else {
                if (ch == ':') {  /*检测赋值符号*/
                    getchdo;
                    if (ch == '=') {
                        sym = becomes;
                        getchdo;
                    }
                    else {
//                        sym = nul;    /*不能识别的符号*/
                        sym = colon;
                    }
                }
                else {
                    if (ch == '<') {   /*检测小于或小于等于符号*/
                        getchdo;
                        if (ch == '=') {
                            sym = leq;
                            getchdo;
                        }
                        else if (ch == '>') {
                            sym = neq;
                            getchdo;
                        }
                        else {
                            sym = lss;
                        }
                    }
                    else {
                        if (ch == '>')   /*检测大于或大于等于符号*/
                        {
                            getchdo;
                            if (ch == '=') {
                                sym = geq;
                                getchdo;
                            }
                            else {
                                sym = gtr;
                            }
                        }
                        else {
                            if (ch == '+') {
                                getchdo;
                                if (ch == '=') {    // +=
                                    sym = PLUSEQ;
                                    getchdo;
                                }
                                else if (ch == '+') {      // ++
                                    sym = INC;
                                    getchdo;
                                }
                                else {      // +
                                    sym = plus;
                                }
                            }
                            else {
                                if (ch == '-') {
                                    getchdo;
                                    if (ch == '=') {    // -=
                                        sym = MINUSEQ;
                                        getchdo;
                                    }
                                    else if (ch == '-') {      // --
                                        sym = DEC;
                                        getchdo;
                                    }
                                    else {      // -
                                        sym = minus;
                                    }
                                }
                                else {
                                    sym = ssym[ch]; /*当符号不满足上述条件时，全部按照单字符符号处理*/
                                    //getchdo;
                                    //richard;
                                    if (sym != period) {
                                        getchdo;
                                    }
                                    //end richard
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/*
* 生成随机虚拟机代码
* x:instruction.f;
* x:instruction.l;
* x:instruction.a;
*/
int gen(enum fct x, int y, int z) {
    if (cx >= cxmax) {
        printf("Program too long"); /*程序过长*/
        return -1;
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
    return 0;
}

/* 测试当前符号是否合法
 *
 *
 * 在某一部分(如一条语句，一个表达式)将要结束时我们希望下一个符号属于某集合
 * (该部分的后跟符号)，test负责着像检测，并且负责当前检测不通过时的补救措施
 * 程序在需要检测时指定当前需要的符号集合和补救用的集合(如之前未完成部分的后跟
 * 符号)，以及检测不通过时的错误符号
 *
 * s1:我们需要的符号
 * s2:如果不是我们需要的，则需要一个补救用的集合
 * n:错误符号
 *
 */


int test(bool *s1, bool *s2, int n) {
    if (!inset(sym, s1)) {
        error(n);
        /*当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合*/
        while ((!inset(sym, s1)) && (!inset(sym, s2))) {
            getsymdo;
        }
    }
    return 0;
}

/*
 * 编译程序主体
 *
 * lev:当前分程序所在层
 * tx:名字表当前尾指针
 * fsys:当前模块后跟符号集合
 */
int block(int lev, int tx, bool *fsys) {
    int i;
    int dx;                             /*名字分配到的相对地址*/
    int tx0;                            /*保留初始tx*/
    int cx0;                            /*保留初始cx*/
    bool nxtlev[symnum];                /*在下级函数的参数中，符号集合均为值参，但由于使用数组实现，传递进来的是指针，
										  为防止下级函数改变上级函数的集合，开辟新的空间传递给下级函数*/
    dx = 3;
    tx0 = tx;                             /*记录本层名字的初始位置*/
    table[tx].adr = cx;
    gendo(jmp, 0, 0);
    if (lev > levmax) {
        error(32);
    }
    do {
        if (sym == constsym)               /*收到常量声明符号，开始处理常量声明*/
        {
            getsymdo;
            do {
                constdeclarationdo(&tx, lev, &dx);        /*dx的值会被constdeclaration改变，使用指针*/
                while (sym == comma) {
                    getsymdo;
                    constdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);            /*漏掉了逗号或者分号*/
                }
            } while (sym == ident);
        }

        if (sym == varsym)                /*收到变量声明符号，开始处理变量声明*/
        {
            getsymdo;
            do {
                vardeclarationdo(&tx, lev, &dx);
                while (sym == comma) {
                    getsymdo;
                    vardeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);
                }
            } while (sym == ident);
        }

        if (sym == intsym) {
            getsymdo;
            do {
                intdeclarationdo(&tx, lev, &dx);
                while (sym == comma) {
                    getsymdo;
                    intdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);
                }
            } while (sym == ident);
        }

        if (sym == charsym) {
            getsymdo;
            do {
                chardeclarationdo(&tx, lev, &dx);
                while (sym == comma) {
                    getsymdo;
                    chardeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);
                }
            } while (sym == ident);
        }

        if (sym == realsym) {
            getsymdo;
            do {
                realdeclarationdo(&tx, lev, &dx);
                while (sym == comma) {
                    getsymdo;
                    realdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);
                }
            } while (sym == ident);
        }

        while (sym == procsym)                         /*收到过程声明符号，开始处理过程声明*/
        {
            getsymdo;
            if (sym == ident) {
                enter(procedur, &tx, lev, &dx);        /*记录过程名字*/
                getsymdo;
            }
            else {
                error(4);                             /*procedure后应为标识符*/
            }
            if (sym == semicolon) {
                getsymdo;
            }
            else {
                error(5);                              /*漏掉了分号*/
            }
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            if (-1 == block(lev + 1, tx, nxtlev)) {
                return -1; /* 递归调用 */
            }
            if (sym == semicolon) {
                getsymdo;
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                testdo(nxtlev, fsys, 6);
            }
            else {
                error(5); /* 漏掉了分号 */
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[period] = true;
        testdo(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys)); /* 直到没有声明符号 */
    code[table[tx0].adr].a = cx; /* 开始生成当前过程代码 */
    table[tx0].adr = cx; /* 当前过程代码地址 */
    table[tx0].size = dx; /* 声明部分中每增加一条声明都会给dx增加1，声明部分已经结束，dx就是当前过程数据的size */
    cx0 = cx;
    gendo(inte, 0, dx);     /*生成分配内存代码*/
    if (tableswitch)        /*输出名字表*/
    {
        printf("TABLE:\n");
        if (tx0 + 1 > tx) {
            printf("NULL\n");
        }
        for (i = tx0 + 1; i <= tx; i++) {
            switch (table[i].kind) {
                case int_type:
                    printf("%d int %s ", i, table[i].name);
                    printf("lev=%d addr= %d\n", table[i].level, table[i].adr);
                    fprintf(fas, "%d int %s ", i, table[i].name);
                    fprintf(fas, "lev=%d addr= %d\n", table[i].level, table[i].adr);
                    break;
                case char_type:
                    printf("%d char %s ", i, table[i].name);
                    printf("lev=%d addr= %d\n", table[i].level, table[i].adr);
                    fprintf(fas, "%d char %s ", i, table[i].name);
                    fprintf(fas, "lev=%d addr= %d\n", table[i].level, table[i].adr);
                    break;
                case real_type:
                    printf("%d real %s ", i, table[i].name);
                    printf("lev=%d addr= %d\n", table[i].level, table[i].adr);
                    fprintf(fas, "%d real %s ", i, table[i].name);
                    fprintf(fas, "lev=%d addr= %d\n", table[i].level, table[i].adr);
                    break;
                case array_type:
                    printf("%d array %s ", i, table[i].name);
                    printf("lev=%d addr= %d size=%d\n", table[i].level, table[i].adr, table[i].size);
                    fprintf(fas, "%d array %s ", i, table[i].name);
                    fprintf(fas, "lev=%d addr= %d size=%d\n", table[i].level, table[i].adr, table[i].size);
                    break;
                case constant:
                    printf("%d const %s ", i, table[i].name);
                    printf("val= %d\n", table[i].val);
                    fprintf(fas, "%d const %s ", i, table[i].name);
                    fprintf(fas, "val= %d\n", table[i].val);
                    break;
                case variable:
                    printf("%d var %s ", i, table[i].name);
                    printf("lev=%d addr= %d\n", table[i].level, table[i].adr);
                    fprintf(fas, "%d var%s ", i, table[i].name);
                    fprintf(fas, "lev=%d addr= %d\n", table[i].level, table[i].adr);
                    break;
                case procedur:
                    printf("%d proc%s ", i, table[i].name);
                    printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                    fprintf(fas, "%d proc%s ", i, table[i].name);
                    fprintf(fas, "lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                    break;
            }
        }
        printf("\n");
    }
    /* 语句后跟符号为分号或end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);  /* 每个后跟符号集和都包含上层后跟符号集合，以便补救 */
    nxtlev[semicolon] = true;
    nxtlev[endsym] = true;
    statementdo(nxtlev, &tx, lev);
    gendo(opr, 0, 0);             /* 每个过程出口都要使用的释放数据段指令 */
    memset(nxtlev, 0, sizeof(bool) * symnum);     /* 分程序没有补救集合 */
    testdo(fsys, nxtlev, 8);                      /* 检测后跟符号的正确性 */
    listcode(cx0);                              /* 输出代码 */
    return 0;
}

/*
*	在名字表中加入一项
*
*	k：名字种类const、var或procedure
*	ptx：名字表尾指针的指针，为了可以改变名字表尾指针的值
*	lev：名字所在的层次，以后所有的lev都是这样
*	pdx：dx为当前应分配的变量的相对地址，分配后要增加1
*/
void enter(enum object k, int *ptx, int lev, int *pdx) {
    (*ptx)++;
    strcpy(table[(*ptx)].name, id);            /*全局变量id中已存在当前名字的名字*/
    table[(*ptx)].kind = k;
    switch (k) {
        case constant:                        /*常量名字*/
            if (num > amax) {
                error(31);                    /*数越界*/
                num = 0;
            }
            table[(*ptx)].val = num;
            break;
        case int_type:
        case char_type:
        case array_type:
        case variable:                        /*变量名字*/
            table[(*ptx)].level = lev;
            table[(*ptx)].adr = (*pdx);
            (*pdx)++;
            break;
        case real_type:
            table[(*ptx)].level = lev;
            table[(*ptx)].adr = (*pdx);
            (*pdx) += 2;        // 用两个内存空间分别存储整数和小数部分
            break;
        case procedur:                        /*过程名字*/
            table[(*ptx)].level = lev;
            break;
        default:;
    }
}

/*
 *  查找名字的位置
 *  找到则返回在名字表中的位置，否则返回 0
 *
 *  idt:要查找的名字
 *  tx:当前名字表尾指针
 */
int position(char *idt, int tx) {
    int i;
    strcpy(table[0].name, idt);
    i = tx;
    while (strcmp(table[i].name, idt) != 0) {
        i--;
    }
    return i;
}

int intdeclaration(int *ptx, int lev, int *pdx){
    if (sym == ident) {
        enter(int_type, ptx, lev, pdx); //填写名字表
        getsymdo;
    }
    else {
        error(4);   /* int后应是标识 */
    }
    return 0;
}

int chardeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        enter(char_type, ptx, lev, pdx); //填写名字表
        getsymdo;
    }
    else {
        error(4);   /* char后应是标识 */
    }
    return 0;
}

int realdeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        enter(real_type, ptx, lev, pdx); //填写名字表
        getsymdo;
    }
    else {
        error(4);   /* real后应是标识 */
    }
    return 0;
}

/*
* 常量声明处理
*/
int constdeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        getsymdo;
        if (sym == eql || sym == becomes) {
            if (sym == becomes) {
                error(1);   /* 把=写成了:= */
            }
            getsymdo;
            if (sym == number || sym == intsym) {
                enter(constant, ptx, lev, pdx);
                getsymdo;
            }
            else {
                error(2);   /* 常量说明=后应是数字 */
            }
        }
        else {
            error(3);   /* 常量说明标识后应是= */
        }
    }
    else {
        error(4);   /* const后应是标识 */
    }
    return 0;
}

/*
 *  变量声明处理
 */
int vardeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        enter(variable, ptx, lev, pdx); //填写名字表
        getsymdo;
        if (sym == lparen) {
            int begin_id = 0, end_id = 0;
            getsymdo;
            switch (sym) {
                case ident:
                    begin_id = table[position(id, *ptx)].val;
                    break;
                case number:
                case intsym:
                    begin_id = num;
                    break;
                default:
                    error(106);     // 自定义报错，下标类型或范围错误
            }
//            table[(*ptx)].adr -= begin_id;          // 修改数组占用内存空间的起始地址 -> bug
            table[(*ptx)].base_id = begin_id;       // 修改数组的基址(下界)
            getsymdo;
            if (sym != colon) {
                error(107);     // 自定义报错，数组格式错误
            }
            else {
                getsymdo;
                switch (sym) {
                    case ident:
                        end_id = table[position(id, *ptx)].val;
                        break;
                    case number:
                    case intsym:
                        end_id = num;
                        if (end_id < begin_id) {
                            error(106);
                        }
                        break;
                    default:
                        error(106);
                }
                table[(*ptx)].size = end_id - begin_id + 1;         // 设置数组大小
                getsymdo;
                if (sym == rparen) {
                    table[(*ptx)].kind = array_type;            // 修改数据类型为数组
                    (*pdx) += (end_id - begin_id);          // 修改空闲内存起始地址
                }
                else {
                    error(108);     // 自定义报错，缺失左括号或者右括号
                }
                getsymdo;
            }
        }
    }
    else {
        error(4);   /* var后应是标识 */
    }
    return 0;
}

/*
 *  输出目标代码清单
 */
void listcode(int cx0) {
    int i;
    if (listswitch) {
        for (i = cx0; i < cx; i++) {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
            fprintf(fa, "%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}

/*
 *  语句处理
 */
int statement(bool *fsys, int *ptx, int lev) {
    int i, cx1, cx2;
    bool nxtlev[symnum];
    if (sym == ident) {
        i = position(id, *ptx);
        if (i == 0) {
            error(11);
        }
        else {
            if (table[i].kind != variable && table[i].kind != int_type && table[i].kind != char_type
            && table[i].kind != array_type && table[i].kind != real_type) {
                error(12);                            /*赋值语句格式错误*/
                i = 0;
            }
            else {
                getsymdo;
                if (sym == becomes) {
                    getsymdo;
                    if (i != 0 && table[i].kind == real_type && sym != realsym) {
                        if (sym == number || sym == intsym) {
                            sym = realsym;                            // 非real型数据将数据按照real型数据处理
                        }
                        else {
                            error(109);                            // 自定义报错，赋值数据与变量类型不符
                        }
                    }
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);        /*处理赋值符号右侧表达式*/
                    if (i != 0) {
                        if (table[i].kind != real_type) {
                            /*expression将执行一系列指令，但最终结果将会保存在栈顶，执行sto命令完成赋值*/
                            gendo(sto, lev - table[i].level, table[i].adr);
                        }
                        else {
                            gendo(str, lev - table[i].level, table[i].adr);     // 定义新指令，将栈顶和次栈顶的值存入real变量
                        }
                    }
                }
                else if (sym == lparen) {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);                 // 计算下标值并将结果写入栈顶
                    gendo(ack, table[i].base_id, table[i].size);    // 检查数组是否越界
                    gendo(jpc, 0, 0);                               // 对栈顶布尔值进行判断
                    if (sym == rparen) {
                        getsymdo;
                        if (sym == becomes) {
                            getsymdo;       // 获取赋值号右边的值
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            expressiondo(nxtlev, ptx, lev);                 // 计算赋值号右边的表达式，并将结果写入栈顶
                            cur_array_base_id = table[i].base_id;               // 设置当前数组基址
                            gendo(sta, lev - table[i].level, table[i].adr);     // 将栈顶值写入数组对应下标位置
                        }
                        else {
                            error(13);
                        }
                    }
                    else {
                        error(108);
                    }
                }
                else if (sym == PLUSEQ) {
                    gendo(lod, lev - table[i].level, table[i].adr);                 // LOD指令，将变量值取到栈顶
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);                                 // 执行表达式
                    gendo(opr, 0, 2);                                               // OPR指令，栈顶和次栈顶求和，结果写入栈顶
                    if (i != 0) {                                                   // i为待写入变量的位置，i为0说明变量不存在
                        gendo(sto, lev - table[i].level, table[i].adr);             // STO指令，栈顶内容写入变量
                    }
                }
                else if (sym == MINUSEQ) {
                    gendo(lod, lev - table[i].level, table[i].adr);
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    gendo(opr, 0, 3);                                               // OPR指令，次栈顶减去栈顶，结果写入栈顶
                    if (i != 0) {
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == INC) {      // 后++
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);     // 取变量值到栈顶
                        gendo(lit, 0, 1);                                   // 取常数1到栈顶用于自增
                        gendo(opr, 0, 2);                                   // 次栈顶与栈顶相加
                        if (i != 0) {
                            gendo(sto, lev - table[i].level, table[i].adr); // 栈顶结果写入变量
                            getsymdo;
                        }
                        else {
                            error(11);
                        }
                    }
                    else {
                        error(11);
                    }
                }
                else if (sym == DEC) {      // 后--
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);
                        gendo(lit, 0, 1);
                        gendo(opr, 0, 3);                                   // 次栈顶减去栈顶
                        if (i != 0) {
                            gendo(sto, lev - table[i].level, table[i].adr);
                            getsymdo;
                        }
                        else {
                            error(11);
                        }
                    }
                    else {
                        error(11);
                    }
                }
                else {
                    error(13);                        /*没有检测到赋值符号*/
                }
            }
        }
    }
    else {
        if (sym == readsym)                            /* 准备按照 read 语句处理 */
        {
            getsymdo;
            if (sym != lparen) {
                error(34);                            /* 格式错误，应是左括号 */
            }
            else {
                do {
                    getsymdo;
                    if (sym == ident) {
                        i = position(id, *ptx);        /* 查找要读的变量 */
                    }
                    else {
                        i = 0;
                    }
                    if (i == 0) {
                        error(35);                    /* read() 中应是声明过的变量名 */
                    }
                    else {
                        if (table[i].kind == array_type) {
                            getsymdo;
                            if (sym == lparen) {
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[rparen] = true;
                                expressiondo(nxtlev, ptx, lev);                 // 计算数组下标并将下标取到栈顶
                                gendo(ack, table[i].base_id, table[i].size);    // 检查数组是否越界
                                gendo(jpc, 0, 0);                               // 对栈顶布尔值进行判断
                                gendo(opr, 0, 16);                              // 从键盘读数据到栈顶
                                cur_array_base_id = table[i].base_id;
                                gendo(sta, lev - table[i].level, table[i].adr); // 将栈顶数据写入数组对应下标位置
                            }
                            else {
                                error(108);
                            }
                        }
                        else if (table[i].kind == real_type) {
                            gendo(opr, 0, 19);      //  定义新指令，将输入的整数或小数取到栈顶
                            gendo(str, lev - table[i].level, table[i].adr);
                        }
                        else {
                            gendo(opr, 0, 16);            /* 生成输入命令，读取值到栈顶 */
                            gendo(sto, lev - table[i].level, table[i].adr);        /* 储存到变量 */
                        }
                    }
                    getsymdo;
                } while (sym == comma);                /* 一条read语句可读多个变量 */
            }
            if (sym != rparen) {
                error(33);                            /* 格式错误，应该是右括号 */
                while (!inset(sym, fsys))            /*出错补救，直到收到上层函数的后跟符号*/
                {
                    getsymdo;
                }
            }
            else {
                getsymdo;
            }
        }
        else {
            if (sym == writesym)                    /*准备按照write语句处理，与read类似*/
            {
                getsymdo;
                if (sym == lparen) {
                    do {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        nxtlev[comma] = true;    /*write的后跟符号为）or,*/
                        expressiondo(nxtlev, ptx, lev);     /*调用表达式处理，此处与read不同，read为给变量赋值*/
                        i = position(id, *ptx);
                        if (i == 0) {
                            error(11);
                        }
                        else {
                            switch (table[i].kind) {
                                case int_type:
                                    gendo(opr, 0, 14);
                                    break;
                                case char_type:
                                    gendo(opr, 0, 17);
                                    break;
                                case real_type:
                                    gendo(opr, 0, 18);
                                    break;
                                default:
                                    gendo(opr, 0, 14);        /*生成输出指令，输出栈顶的值*/
                            }
                        }
                    } while (sym == comma);
                    if (sym != rparen) {
                        error(33);                /*write（）中应为完整表达式*/
                    }
                    else {
                        getsymdo;
                    }
                }
                gendo(opr, 0, 15);                /*输出换行*/
            }
            else {
                if (sym == callsym)                /*准备按照call语句处理*/
                {
                    getsymdo;
                    if (sym != ident) {
                        error(14);              /* call后应为标识符 */
                    }
                    else {
                        i = position(id, *ptx);
                        if (i == 0) {
                            error(11);          /* 过程未找到 */
                        }
                        else {
                            if (table[i].kind == procedur) {
                                gendo(cal, lev - table[i].level, table[i].adr); /* 生成call指令 */
                            }
                            else {
                                error(15);      /* call后标识符应为过程 */
                            }
                        }
                        getsymdo;
                    }
                }
                else {
                    if (sym == ifsym)                      /* 准备按照if语句处理 */
                    {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;             /* 后跟符号为then或do */
                        conditiondo(nxtlev, ptx, lev);    /* 调用条件处理（逻辑运算）函数*/
                        if (sym == thensym) {
                            getsymdo;
                        }
                        else {
                            error(16);                    /* 缺少then */
                        }
                        cx1 = cx;                         /* 保存当前指令地址 */
                        gendo(jpc, 0, 0);                 /* 生成条件跳转指令，跳转地址暂写0 */
                        statementdo(fsys, ptx, lev);      /* 处理then后的语句 */
                        // add ELSE
                        if (sym == ELSESYM) {
                            getsymdo;
                            cx2 = cx;       //记录jmp指令位置
                            gendo(jmp, 0, 0);   //无条件跳转指令 ,将来直接跳转到else语句后面
                            code[cx1].a = cx;   //地址回填，给jpc指令的地址赋值
                            statementdo(fsys, ptx, lev);    //处理else后面的语句
                            code[cx2].a = cx;   //else后面的语句结束位置，给jmp指令的地址赋值，if顺序执行时会跳转到该位置
                        }
                        else {
                            code[cx1].a = cx;                 /* 经statement do处理后，cx为then后语句执
						    									行完的位置，它正是前面未定的跳转地址*/
                        }
                    }
                    else {
                        if (sym == beginsym)    /*准备按照复合语句处理*/
                        {
                            getsymdo;
                            if (sym == intsym || sym == charsym || sym == realsym) {
                                sym = ident;
                                getsymdo;
                            }
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true;    /*后跟符号为分号或end*/
                            /*循环调用语句处理函数，直到下一个符号不是语句开始符号或收到end*/
                            statementdo(nxtlev, ptx, lev);
                            while (inset(sym, statbegsys) || sym == semicolon) {
                                if (sym == semicolon) {
                                    getsymdo;
                                }
                                else {
                                    error(10);    /*缺少分号*/
                                }
                                statementdo(nxtlev, ptx, lev);
                            }
                            if (sym == endsym) {
                                getsymdo;
                            }
                            else {
                                error(17);    /*缺少end或分号*/
                            }
                        }
                        else {
                            if (sym == whilesym)    /*准备按照while语句处理*/
                            {
                                cx1 = cx;    /*保存判断条件操作的位置*/
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[dosym] = true;    /*后跟符号为do*/
                                conditiondo(nxtlev, ptx, lev);    /*调用条件处理*/
                                cx2 = cx;    /*保存循环体的结柬的下一个位置*/
                                gendo(jpc, 0, 0);    /*生成条件跳转,但跳出循环的地址未知*/
                                if (sym == dosym) {
                                    getsymdo;
                                }
                                else {
                                    error(18);    /*缺少do*/
                                }
                                statementdo(fsys, ptx, lev);    /*循环体*/
                                gendo(jmp, 0, cx1);    /*回头重新判断条件*/
                                code[cx2].a = cx;        /* 反填跳出循环的地址，与if类似 */
                            }
                            else {
                                if (sym == FORSYM) {
//                                    printf("keyword: FOR\n");
                                    getsymdo;
                                    if (sym != ident) {
                                        error(101);                             // 自添加报错，FOR后面需要变量
                                    }
                                    i = position(id, *ptx);                 // 查找变量位置
                                    if (i == 0) {
                                        error(11);
                                    }
                                    else {
                                        if (table[i].kind != variable) {
                                            error(12);                            /*赋值语句格式错误*/
                                            i = 0;
                                        }
                                        else {
                                            getsymdo;
                                            if (sym != becomes) {
                                                error(13);                        /*没有检测到赋值符号*/
                                            }
                                            else {
                                                getsymdo;
                                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                                nxtlev[TOSYM] = true;               // 标记下一层的处理
                                                expressiondo(nxtlev, ptx, lev);     // 执行表达式，结果存放在栈顶
                                                gendo(sto, lev - table[i].level, table[i].adr);     // STO指令保存初值
                                                if (sym == TOSYM) {
                                                    cx1 = cx;           // 保存循环起始位置
                                                    getsymdo;
                                                    gendo(lod, lev - table[i].level, table[i].adr);     // 循环变量取到栈顶
                                                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                                    nxtlev[dosym] = true;       // 标记下一层的处理
                                                    expressiondo(nxtlev, ptx, lev);     // 执行表达式，结果存放在栈顶
                                                    gendo(opr, 0, 13);      // 比较次栈顶是否小于等于栈顶，结果进栈
                                                    cx2 = cx;               // 记录JPC指令的位置，便于后续回填jpc跳转地址
                                                    gendo(jpc, 0, 0);       // 生成JPC跳转指令，地址暂填0
                                                    if (sym == dosym) {     // 检测到 do，进行循环体处理
                                                        getsymdo;
                                                        statementdo(fsys, ptx, lev);    // 循环体表达式执行
                                                        gendo(lod, lev - table[i].level, table[i].adr);     // 循环变量取到栈顶
                                                        gendo(lit, 0, step);            // 步长取到栈顶
                                                        gendo(opr, 0, 2);               // OPR指令执行次栈顶与栈顶相加
                                                        gendo(sto, lev - table[i].level, table[i].adr);     // 结果写入循环变量
                                                        gendo(jmp, 0, cx1);             // 无条件跳转到循环起始位置
                                                        code[cx2].a = cx;               // JPC跳转地址回填至循环结束位置
                                                    }
                                                    else {
                                                        error(103);             // 自添加报错，缺少do
                                                    }
                                                }
                                                else if (sym == DOWNTOSYM) {
                                                    cx1 = cx;
                                                    getsymdo;
                                                    gendo(lod, lev - table[i].level, table[i].adr);
                                                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                                    nxtlev[dosym] = true;
                                                    expressiondo(nxtlev, ptx, lev);
                                                    gendo(opr, 0, 11);      // 比较次栈顶是否大于等于栈顶，结果进栈
                                                    cx2 = cx;
                                                    gendo(jpc, 0, 0);
                                                    if (sym == dosym) {
                                                        getsymdo;
                                                        statementdo(fsys, ptx, lev);
                                                        gendo(lod, lev - table[i].level, table[i].adr);
                                                        gendo(lit, 0, step);
                                                        gendo(opr, 0, 3);               // OPR指令执行次栈顶减去栈顶
                                                        gendo(sto, lev - table[i].level, table[i].adr);
                                                        gendo(jmp, 0, cx1);
                                                        code[cx2].a = cx;
                                                    }
                                                    else {
                                                        error(103);
                                                    }
                                                }
                                                else {
                                                    error(102);                 // 自添加报错，缺少TO或者DOWNTO
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    if (sym == TOSYM) {
                                        printf("keyword: TO\n");
                                        getsymdo;
                                    }
                                    else {
                                        if (sym == DOWNTOSYM) {
                                            printf("keyword: DOWNTO\n");
                                            getsymdo;
                                        }
                                        else {
                                            if (sym == RETURNSYM) {
                                                printf("keyword: RETURN\n");
                                                getsymdo;
                                            }
                                            else {
                                                if (sym == PLUSEQ) {
                                                    printf("keyword: +=\n");
                                                    getsymdo;
                                                }
                                                else {
                                                    if (sym == INC) {       // 前++
//                                                        printf("keyword: ++\n");
                                                        getsymdo;
                                                        if (sym == ident) {
                                                            i = position(id, *ptx);
                                                            if (i == 0) {
                                                                error(11);
                                                            }
                                                            else {
                                                                if (table[i].kind != variable) {
                                                                    error(12);
                                                                    i = 0;
                                                                }
                                                                else {
                                                                    if (i != 0) {
                                                                        // LOD指令取变量值到栈顶
                                                                        gendo(lod, lev - table[i].level, table[i].adr);
                                                                        gendo(lit, 0, 1);              // 取常数1到栈顶
                                                                        gendo(opr, 0,2);               // 次栈顶与栈顶相加
                                                                        if (i != 0) {
                                                                            // 栈顶结果写入变量
                                                                            gendo(sto, lev - table[i].level, table[i].adr);
                                                                            getsymdo;
                                                                        }
                                                                        else {
                                                                            error(11);
                                                                        }
                                                                    }
                                                                    else {
                                                                        error(11);
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        else {
                                                            error(104);             // 自定义报错，++--后缺少变量
                                                        }
                                                    }
                                                    else {
                                                        if (sym == MINUSEQ) {
                                                            printf("keyword: -=\n");
                                                            getsymdo;
                                                        }
                                                        else {
                                                            if (sym == DEC) {       // 前--
//                                                                printf("keyword: --\n");
                                                                getsymdo;
                                                                if (sym == ident) {
                                                                    i = position(id, *ptx);
                                                                    if (i == 0) {
                                                                        error(11);
                                                                    }
                                                                    else {
                                                                        if (table[i].kind != variable) {
                                                                            error(12);
                                                                            i = 0;
                                                                        }
                                                                        else {
                                                                            if (i != 0) {
                                                                                gendo(lod, lev - table[i].level, table[i].adr);
                                                                                gendo(lit, 0, 1);
                                                                                gendo(opr, 0, 3);       // 次栈顶减去栈顶
                                                                                if (i != 0) {
                                                                                    gendo(sto, lev - table[i].level, table[i].adr);
                                                                                    getsymdo;
                                                                                }
                                                                                else {
                                                                                    error(11);
                                                                                }
                                                                            }
                                                                            else {
                                                                                error(11);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                else {
                                                                    error(104);
                                                                }
                                                            }
                                                            else {
                                                                memset(nxtlev, 0,
                                                                       sizeof(bool) * symnum);    /* 语句结束无补救集合 */
                                                                testdo(fsys, nxtlev, 19);    /* 检测语句结束的正确性 */
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/*
 * 表达式处理
 */
int expression(bool *fsys, int *ptx, int lev) {
    enum symbol addop;    /*用于保存正负号*/
    bool nxtlev[symnum];
    int i;
    if (sym == plus || sym == minus)    /*开头的正负号，此时当前表达式被看作一个正的或负的项*/
    {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);    /*处理项*/
        if (addop == minus) {
            gendo(opr, 0, 1);    /*如果开头为负号，生成取负指令*/
        }
    }
    else if (sym == INC) {      // 前++
        getsymdo;
        if (sym == ident) {
            i = position(id, *ptx);                 // 查找变量位置
            if (i == 0) {
                error(11);
            }
            else {
                if (table[i].kind != variable) {            // 判断变量类型
                    error(12);
                    i = 0;
                }
                else {
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);
                        gendo(lit, 0, 1);
                        gendo(opr, 0, 2);       // 加
                        if (i != 0) {
                            gendo(sto, lev - table[i].level, table[i].adr);         // 栈顶结果写入变量
                            gendo(lod, lev - table[i].level, table[i].adr);         // 取变量值到栈顶作为表达式结果
                            getsymdo;
                        }
                        else {
                            error(12);
                        }
                    }
                    else {
                        error(11);
                    }
                }
            }
        }
    }
    else if (sym == DEC) {      // 前--
        getsymdo;
        if (sym == ident) {
            i = position(id, *ptx);
            if (i == 0) {
                error(11);
            }
            else {
                if (table[i].kind != variable) {
                    error(12);
                    i = 0;
                }
                else {
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);
                        gendo(lit, 0, 1);
                        gendo(opr, 0, 3);       // 减
                        if (i != 0) {
                            gendo(sto, lev - table[i].level, table[i].adr);
                            gendo(lod, lev - table[i].level, table[i].adr);
                            getsymdo;
                        }
                        else {
                            error(12);
                        }
                    }
                    else {
                        error(11);
                    }
                }
            }
        }
    }
    else    /*此时表达式被看作项的加减*/
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);    /*处理项*/
    }
    while (sym == plus || sym == minus) {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);             /*处理项*/
        if (addop == plus) {
            gendo(opr, 0, 2);                 /*生成加法指令*/
        }
        else {
            gendo(opr, 0, 3);                 /*生成减法指令*/
        }
    }
    return 0;
}

/*
* 项处理
*/
int term(bool *fsys, int *ptx, int lev) {
    enum symbol mulop;        /*用于保存乘除法符号*/
    bool nxtlev[symnum];
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    factordo(nxtlev, ptx, lev);        /*处理因子*/
    while (sym == times || sym == slash) {
        mulop = sym;
        getsymdo;
        factordo(nxtlev, ptx, lev);
        if (mulop == times) {
            gendo(opr, 0, 4);    /*生成乘法指令*/
        }
        else {
            gendo(opr, 0, 5);    /*生成除法指令*/
        }
    }
    return 0;
}

/*
 * 因子处理
 */
int factor(bool *fsys, int *ptx, int lev) {
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys, fsys, 24);                    /*检测因子的开始符号*/
    while (inset(sym, facbegsys))                    /*循环直到不是因子开始符号*/
    {
        if (sym == ident)                            /*因子为常量或变量*/
        {
            i = position(id, *ptx);                    /*查找名字*/
            if (i == 0) {
                error(11);                            /*标识符未声明*/
            }
            else {
                switch (table[i].kind) {
                    case constant:                    /*名字为常量*/
                        gendo(lit, 0, table[i].val);/*直接把常量的值入栈*/
                        break;
                    case int_type:
                    case char_type:
                    case variable:                    /*名字为变量*/
                        gendo(lod, lev - table[i].level, table[i].adr);/*找到变量地址并将其值入栈*/
                        break;
                    case real_type:
                        gendo(ldr, lev - table[i].level, table[i].adr);     // 定义新指令将real变量的值取到栈顶
                        break;
                    case array_type:
                        getsymdo;
                        if (sym == lparen) {
                            getsymdo;
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[rparen] = true;
                            expressiondo(nxtlev, ptx, lev);                 // 计算数组下标并将下标写入栈顶
                            gendo(ack, table[i].base_id, table[i].size);    // 检查数组是否越界
                            gendo(jpc, 0, 0);                               // 对栈顶布尔值进行判断
                            cur_array_base_id = table[i].base_id;
                            gendo(lda, lev - table[i].level, table[i].adr); // 将数组对应下标的数据取到栈顶
                        }
                        else {
                            error(108);
                        }
                        break;
                    case procedur:                    /*名字为过程*/
                        error(21);                    /*不能为过程*/
                        break;
                }
            }
            getsymdo;
            if (sym == INC || sym == DEC) {         // 后++ --
                gendo(lit, 0, 1);                                       // 取常数1放入栈顶
                if (sym == INC) {
                    gendo(opr, 0, 2);                                   // 次栈顶与栈顶相加
                    gendo(sto, lev - table[i].level, table[i].adr);     // 栈顶内容写入变量单元
                    gendo(lod, lev - table[i].level, table[i].adr);     // 将变量值放入栈顶
                    gendo(lit, 0, 1);                                   // 由于后自增不改变表达式的值
                    gendo(opr, 0, 3);                                   // 故需要减去1
                }
                else if (sym == DEC) {
                    gendo(opr, 0, 3);                                   // 次栈顶减去栈顶
                    gendo(sto, lev - table[i].level, table[i].adr);
                    gendo(lod, lev - table[i].level, table[i].adr);
                    gendo(lit, 0, 1);
                    gendo(opr, 0, 2);                                   // 后自减则加1
                }
                getsymdo;
            }
        }
        else {
            if (sym == number || sym == intsym || sym == charsym || sym == realsym)
            {
                if (num > amax) {
                    error(31);
                    num = 0;
                }
                if (sym == realsym) {
                    gendo(lit, 0, dec_part);        // 先取小数部分到栈顶
                    gendo(lit, 0, int_part);        // 再取整数部分到栈顶
                    getsymdo;
                }
                else {
                    gendo(lit, 0, num);
                    getsymdo;
                }
            }
            else {
                if (sym == lparen)                    /*因子为表达式*/
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expressiondo(nxtlev, ptx, lev);
                    if (sym == rparen) {
                        getsymdo;
                    }
                    else {
                        error(22);                    /*缺少右括号*/
                    }
                }
                testdo(fsys, facbegsys, 23);        /*因子后有非法符号*/
            }
        }
    }
    return 0;
}

/*
*条件处理
*/
int condition(bool *fsys, int *ptx, int lev) {
    enum symbol relop;
    bool nxtlev[symnum];
    if (sym == oddsym)                                /*准备按照 odd 运算处理*/
    {
        getsymdo;
        expressiondo(fsys, ptx, lev);
        gendo(opr, 0, 6);                             /*生成 odd 指令*/
    }
    else {
        /*逻辑表达式处理*/
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;
        expressiondo(nxtlev, ptx, lev);
        if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq) {
            error(20);
        }
        else {
            relop = sym;
            getsymdo;
            expressiondo(fsys, ptx, lev);
            switch (relop) {
                case eql:
                    gendo(opr, 0, 8);
                    break;
                case neq:
                    gendo(opr, 0, 9);
                    break;
                case lss:
                    gendo(opr, 0, 10);
                    break;
                case geq:
                    gendo(opr, 0, 11);
                    break;
                case gtr:
                    gendo(opr, 0, 12);
                    break;
                case leq:
                    gendo(opr, 0, 13);
                    break;
            }
        }
    }
    return 0;
}

/*
* 解释程序
*/
void interpret() {
    int p, b, t;               /*指令指针，指令基址，栈顶指针*/
    struct instruction i;   /*存放当前指令*/
    int s[stacksize];       /*栈*/

    printf("start pl0\n");
    t = 0;
    b = 0;
    p = 0;
    s[0] = s[1] = s[2] = 0;
    do {
        i = code[p];
        p++;
        switch (i.f) {
            case sta:
                t--;
                s[base(i.l, s, b) + i.a + s[t - 1] - cur_array_base_id] = s[t];
                t--;
                break;
            case lda:
                s[t - 1] = s[base(i.l, s, b) + i.a + s[t - 1] - cur_array_base_id];
                break;
            case ack:
                s[t] = i.a;
                if ((s[t - 1] < i.l) || (s[t - 1] > s[t] + i.l - 1)) {
                    printf("Error: Out of the array's size!\n");
                    fprintf(fa2, "Error: Out of the array's size!\n");
                    s[t] = 0;
                }
                else {
                    s[t] = 1;
                }
                t++;
                break;
            case str:
                t--;
                s[i.a + 1] = s[t];
                t--;
                s[i.a] = s[t];
                break;
            case ldr:
                s[t] = s[i.a];
                t++;
                s[t] = s[i.a + 1];
                t++;
                break;
            case lit :
                s[t] = i.a;
                t++;
                break;
            case opr :
                switch (i.a) {
                    case 0:
                        t = b;
                        p = s[t + 2];
                        b = s[t + 1];
                        break;
                    case 1:
                        s[t - 1] = -s[t - 1];
                        break;
                    case 2:
                        t--;
                        s[t - 1] = s[t - 1] + s[t];
                        break;
                    case 3:
                        t--;
                        s[t - 1] = s[t - 1] - s[t];
                        break;
                    case 4:
                        t--;
                        s[t - 1] = s[t - 1] * s[t];
                        break;
                    case 5:
                        t--;
                        s[t - 1] = s[t - 1] / s[t];
                        break;
                    case 6:
                        s[t - 1] = s[t - 1] % 2;
                        break;
                    case 8:
                        t--;
                        s[t - 1] = (s[t - 1] == s[t]);
                        break;
                    case 9:
                        t--;
                        s[t - 1] = (s[t - 1] != s[t]);
                        break;
                    case 10:
                        t--;
                        s[t - 1] = (s[t - 1] < s[t]);
                        break;
                    case 11:
                        t--;
                        s[t - 1] = (s[t - 1] >= s[t]);
                        break;
                    case 12:
                        t--;
                        s[t - 1] = (s[t - 1] > s[t]);
                        break;
                    case 13:
                        t--;
                        s[t - 1] = (s[t - 1] <= s[t]);
                        break;
                    case 14:
                        printf("%d", s[t - 1]);
                        fprintf(fa2, "%d", s[t - 1]);
                        t--;
                        break;
                    case 15:
                        printf("\n");
                        fprintf(fa2, "\n");
                        break;
                    case 16:
                        printf("?");
                        fprintf(fa2, "?");
                        scanf("%d", &(s[t]));
                        fprintf(fa2, "%d\n", s[t]);
                        t++;
                        break;
                    case 17:
                        printf("%c", (char)s[t - 1]);
                        fprintf(fa2, "%c", (char)s[t - 1]);
                        t--;
                        break;
                    case 18:
                        printf("%d.", s[t - 1]);
                        fprintf(fa2, "%d.", s[t - 1]);
                        t--;
                        printf("%d", s[t - 1]);
                        fprintf(fa2, "%d", s[t - 1]);
                        t--;
                        break;
                    case 19:
                        printf("?");
                        fprintf(fa2, "?");

                        char temp[nmax];
                        char* str_p;
                        int before_dot = 0, after_dot = 0;
                        scanf("%s", temp);
                        str_p = strtok(temp, ".");      // 以小数点分割字符串
                        before_dot = atoi(str_p);       // 整数部分字符串转为int型
                        str_p = strtok(NULL, " ");
                        if (str_p != NULL) {            // 小数部分为空则取0，非空则转对应字符串为int型
                            after_dot = atoi(str_p);
                        }
                        s[t] = after_dot;
                        t++;
                        s[t] = before_dot;
                        t++;

                        fprintf(fa2, "%s\n", temp);
                        break;
                }
                break;
            case lod:
                s[t] = s[base(i.l, s, b) + i.a];
                t++;
                break;
            case sto:
                t--;
                s[base(i.l, s, b) + i.a] = s[t];
                break;
            case cal:
                s[t] = base(i.l, s, b);
                s[t + 1] = b;
                s[t + 2] = p;
                b = t;
                p = i.a;
                break;
            case inte:
                t += i.a;
                break;
            case jmp:
                p = i.a;
                break;
            case jpc:
                t--;
                if (s[t] == 0) {
                    p = i.a;
                }
                break;
        }
    } while (p != 0);
}


int base(int l, int *s, int b) {
    int b1;
    b1 = b;
    while (l > 0) {
        b1 = s[b1];
        l--;
    }
    return b1;
}
