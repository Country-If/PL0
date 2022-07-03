// #define _CRT_SECURE_NO_WARNINGS
// #pragma warning(disable:4996)

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "pl0.h"
#include "string.h"
/*����ִ��ʱʹ�õ�ջ*/
#define stacksize 500

int main() {
    printf("��ʼ����ʱ�䣺");
    print_time();

    printf("��Ϣ��ȫ1��\t3119005436\t�����\n\n");

    bool nxtlev[symnum];
    printf("Input pl/0 file?");
    scanf("%s", fname);               /*�����ļ���*/
    fin = fopen(fname, "r");
    if (fin) {
        printf("List object code?(Y/N)");            /*�Ƿ�������������*/
        scanf("%s", fname);
        listswitch = (fname[0] == 'y' || fname[0] == 'Y');
        printf("List symbol table?(Y/N)");             /*�Ƿ�������ֱ�*/
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
            if (-1 == block(0, 0, nxtlev)) /*���ñ������*/
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
                interpret();    /*���ý���ִ�г���*/
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

    printf("��������ʱ�䣺");
    print_time();
    printf("\n");

    system("pause");
    return 0;
}

/*
 * ��ӡ��ǰʱ��
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

// ��ʼ��
void init() {
    int i;
    for (i = 0; i <= 255; i++) {
        ssym[i] = nul;
    }

    // ���õ��ַ�����
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

    /* ���ñ���������,������ĸ˳�򣬱����۰���� */     /*ASCII�У���д��ĸ��Сд��ĸ֮ǰ*/
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

    /* ���ñ����ַ��� */
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


    /* ����ָ������ */
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

    /* ���÷��ż� */
    for (i = 0; i < symnum; i++) {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /* ����������ʼ���ż� */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[procsym] = true;

    /* ������俪ʼ���ż� */
    statbegsys[beginsym] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;


    /* �������ӿ�ʼ���ż� */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    facbegsys[intsym] = true;
    facbegsys[charsym] = true;
    facbegsys[realsym] = true;
}

/*
* ������ʵ�ּ��ϵļ�������
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
*   ��������ӡ����λ�úʹ������
*/
void error(int n) {
    char space[81];
    memset(space, 32, 81);
    space[cc - 1] = 0;//����ʱ��ǰ�����Ѿ����꣬����cc-1
    printf("****%s!%d\n", space, n);
    fprintf(fa1, "****%s!%d\n", space, n);
    err++;
}

/*
©���ո񣬶�ȡһ���ַ���
ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ�С�
������getsym���á�
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
*�ʷ���������ȡһ������
*/
int getsym() {
    int i, j, k;
    while (ch == ' ' || ch == 10 || ch == 9)            /*���Կո񡢻��к�TAB*/
    {
        getchdo;
    }
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {                    /*���ֻ�������a~z��ͷ*/
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
        do {                /*������ǰ�����Ƿ�Ϊ������*/
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
            sym = ident;        /*����ʧ�ܣ��������ֻ�����*/
        }
    }
    else {
        if (ch >= '0' && ch <= '9') {                /*����Ƿ�Ϊ���֣���0~9��ͷ*/
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
                        dec_part = 10 * dec_part + ch - '0';        // ����С������
                        k++;
                        getchdo;
                    }
                    break;
                }
                else {
                    num = 10 * num + ch - '0';
                    int_part = 10 * int_part + ch - '0';        // ������������
                    k++;
                    getchdo;
                }
            } while (ch >= '0' && ch <= '9' || ch == '.');/*��ȡ���ֵ�ֵ*/
            k--;
            if (k > nmax) {
                error(30);
            }
        }
        else {
            if ((int) ch == 39) {        // �ַ��� ' ��ͷ
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
                        error(105);         // �Զ��屨���ַ����Ͳ�����
                    }
                }
                else {
                    sym = nul;
                    error(105);
                }
                getchdo;
            }
            else {
                if (ch == ':') {  /*��⸳ֵ����*/
                    getchdo;
                    if (ch == '=') {
                        sym = becomes;
                        getchdo;
                    }
                    else {
//                        sym = nul;    /*����ʶ��ķ���*/
                        sym = colon;
                    }
                }
                else {
                    if (ch == '<') {   /*���С�ڻ�С�ڵ��ڷ���*/
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
                        if (ch == '>')   /*�����ڻ���ڵ��ڷ���*/
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
                                    sym = ssym[ch]; /*�����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ���*/
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
* ����������������
* x:instruction.f;
* x:instruction.l;
* x:instruction.a;
*/
int gen(enum fct x, int y, int z) {
    if (cx >= cxmax) {
        printf("Program too long"); /*�������*/
        return -1;
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
    return 0;
}

/* ���Ե�ǰ�����Ƿ�Ϸ�
 *
 *
 * ��ĳһ����(��һ����䣬һ�����ʽ)��Ҫ����ʱ����ϣ����һ����������ĳ����
 * (�ò��ֵĺ������)��test���������⣬���Ҹ���ǰ��ⲻͨ��ʱ�Ĳ��ȴ�ʩ
 * ��������Ҫ���ʱָ����ǰ��Ҫ�ķ��ż��ϺͲ����õļ���(��֮ǰδ��ɲ��ֵĺ��
 * ����)���Լ���ⲻͨ��ʱ�Ĵ������
 *
 * s1:������Ҫ�ķ���
 * s2:�������������Ҫ�ģ�����Ҫһ�������õļ���
 * n:�������
 *
 */


int test(bool *s1, bool *s2, int n) {
    if (!inset(sym, s1)) {
        error(n);
        /*����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ���*/
        while ((!inset(sym, s1)) && (!inset(sym, s2))) {
            getsymdo;
        }
    }
    return 0;
}

/*
 * �����������
 *
 * lev:��ǰ�ֳ������ڲ�
 * tx:���ֱ�ǰβָ��
 * fsys:��ǰģ�������ż���
 */
int block(int lev, int tx, bool *fsys) {
    int i;
    int dx;                             /*���ַ��䵽����Ե�ַ*/
    int tx0;                            /*������ʼtx*/
    int cx0;                            /*������ʼcx*/
    bool nxtlev[symnum];                /*���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������ʵ�֣����ݽ�������ָ�룬
										  Ϊ��ֹ�¼������ı��ϼ������ļ��ϣ������µĿռ䴫�ݸ��¼�����*/
    dx = 3;
    tx0 = tx;                             /*��¼�������ֵĳ�ʼλ��*/
    table[tx].adr = cx;
    gendo(jmp, 0, 0);
    if (lev > levmax) {
        error(32);
    }
    do {
        if (sym == constsym)               /*�յ������������ţ���ʼ����������*/
        {
            getsymdo;
            do {
                constdeclarationdo(&tx, lev, &dx);        /*dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ��*/
                while (sym == comma) {
                    getsymdo;
                    constdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);            /*©���˶��Ż��߷ֺ�*/
                }
            } while (sym == ident);
        }

        if (sym == varsym)                /*�յ������������ţ���ʼ�����������*/
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

        while (sym == procsym)                         /*�յ������������ţ���ʼ�����������*/
        {
            getsymdo;
            if (sym == ident) {
                enter(procedur, &tx, lev, &dx);        /*��¼��������*/
                getsymdo;
            }
            else {
                error(4);                             /*procedure��ӦΪ��ʶ��*/
            }
            if (sym == semicolon) {
                getsymdo;
            }
            else {
                error(5);                              /*©���˷ֺ�*/
            }
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            if (-1 == block(lev + 1, tx, nxtlev)) {
                return -1; /* �ݹ���� */
            }
            if (sym == semicolon) {
                getsymdo;
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                testdo(nxtlev, fsys, 6);
            }
            else {
                error(5); /* ©���˷ֺ� */
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[period] = true;
        testdo(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys)); /* ֱ��û���������� */
    code[table[tx0].adr].a = cx; /* ��ʼ���ɵ�ǰ���̴��� */
    table[tx0].adr = cx; /* ��ǰ���̴����ַ */
    table[tx0].size = dx; /* ����������ÿ����һ�����������dx����1�����������Ѿ�������dx���ǵ�ǰ�������ݵ�size */
    cx0 = cx;
    gendo(inte, 0, dx);     /*���ɷ����ڴ����*/
    if (tableswitch)        /*������ֱ�*/
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
    /* ���������Ϊ�ֺŻ�end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);  /* ÿ��������ż��Ͷ������ϲ������ż��ϣ��Ա㲹�� */
    nxtlev[semicolon] = true;
    nxtlev[endsym] = true;
    statementdo(nxtlev, &tx, lev);
    gendo(opr, 0, 0);             /* ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�ָ�� */
    memset(nxtlev, 0, sizeof(bool) * symnum);     /* �ֳ���û�в��ȼ��� */
    testdo(fsys, nxtlev, 8);                      /* ��������ŵ���ȷ�� */
    listcode(cx0);                              /* ������� */
    return 0;
}

/*
*	�����ֱ��м���һ��
*
*	k����������const��var��procedure
*	ptx�����ֱ�βָ���ָ�룬Ϊ�˿��Ըı����ֱ�βָ���ֵ
*	lev���������ڵĲ�Σ��Ժ����е�lev��������
*	pdx��dxΪ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
*/
void enter(enum object k, int *ptx, int lev, int *pdx) {
    (*ptx)++;
    strcpy(table[(*ptx)].name, id);            /*ȫ�ֱ���id���Ѵ��ڵ�ǰ���ֵ�����*/
    table[(*ptx)].kind = k;
    switch (k) {
        case constant:                        /*��������*/
            if (num > amax) {
                error(31);                    /*��Խ��*/
                num = 0;
            }
            table[(*ptx)].val = num;
            break;
        case int_type:
        case char_type:
        case array_type:
        case variable:                        /*��������*/
            table[(*ptx)].level = lev;
            table[(*ptx)].adr = (*pdx);
            (*pdx)++;
            break;
        case real_type:
            table[(*ptx)].level = lev;
            table[(*ptx)].adr = (*pdx);
            (*pdx) += 2;        // �������ڴ�ռ�ֱ�洢������С������
            break;
        case procedur:                        /*��������*/
            table[(*ptx)].level = lev;
            break;
        default:;
    }
}

/*
 *  �������ֵ�λ��
 *  �ҵ��򷵻������ֱ��е�λ�ã����򷵻� 0
 *
 *  idt:Ҫ���ҵ�����
 *  tx:��ǰ���ֱ�βָ��
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
        enter(int_type, ptx, lev, pdx); //��д���ֱ�
        getsymdo;
    }
    else {
        error(4);   /* int��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

int chardeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        enter(char_type, ptx, lev, pdx); //��д���ֱ�
        getsymdo;
    }
    else {
        error(4);   /* char��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

int realdeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        enter(real_type, ptx, lev, pdx); //��д���ֱ�
        getsymdo;
    }
    else {
        error(4);   /* real��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

/*
* ������������
*/
int constdeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        getsymdo;
        if (sym == eql || sym == becomes) {
            if (sym == becomes) {
                error(1);   /* ��=д����:= */
            }
            getsymdo;
            if (sym == number || sym == intsym) {
                enter(constant, ptx, lev, pdx);
                getsymdo;
            }
            else {
                error(2);   /* ����˵��=��Ӧ������ */
            }
        }
        else {
            error(3);   /* ����˵����ʶ��Ӧ��= */
        }
    }
    else {
        error(4);   /* const��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

/*
 *  ������������
 */
int vardeclaration(int *ptx, int lev, int *pdx) {
    if (sym == ident) {
        enter(variable, ptx, lev, pdx); //��д���ֱ�
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
                    error(106);     // �Զ��屨���±����ͻ�Χ����
            }
//            table[(*ptx)].adr -= begin_id;          // �޸�����ռ���ڴ�ռ����ʼ��ַ -> bug
            table[(*ptx)].base_id = begin_id;       // �޸�����Ļ�ַ(�½�)
            getsymdo;
            if (sym != colon) {
                error(107);     // �Զ��屨�������ʽ����
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
                table[(*ptx)].size = end_id - begin_id + 1;         // ���������С
                getsymdo;
                if (sym == rparen) {
                    table[(*ptx)].kind = array_type;            // �޸���������Ϊ����
                    (*pdx) += (end_id - begin_id);          // �޸Ŀ����ڴ���ʼ��ַ
                }
                else {
                    error(108);     // �Զ��屨��ȱʧ�����Ż���������
                }
                getsymdo;
            }
        }
    }
    else {
        error(4);   /* var��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

/*
 *  ���Ŀ������嵥
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
 *  ��䴦��
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
                error(12);                            /*��ֵ����ʽ����*/
                i = 0;
            }
            else {
                getsymdo;
                if (sym == becomes) {
                    getsymdo;
                    if (i != 0 && table[i].kind == real_type && sym != realsym) {
                        if (sym == number || sym == intsym) {
                            sym = realsym;                            // ��real�����ݽ����ݰ���real�����ݴ���
                        }
                        else {
                            error(109);                            // �Զ��屨����ֵ������������Ͳ���
                        }
                    }
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);        /*����ֵ�����Ҳ���ʽ*/
                    if (i != 0) {
                        if (table[i].kind != real_type) {
                            /*expression��ִ��һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ*/
                            gendo(sto, lev - table[i].level, table[i].adr);
                        }
                        else {
                            gendo(str, lev - table[i].level, table[i].adr);     // ������ָ���ջ���ʹ�ջ����ֵ����real����
                        }
                    }
                }
                else if (sym == lparen) {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);                 // �����±�ֵ�������д��ջ��
                    gendo(ack, table[i].base_id, table[i].size);    // ��������Ƿ�Խ��
                    gendo(jpc, 0, 0);                               // ��ջ������ֵ�����ж�
                    if (sym == rparen) {
                        getsymdo;
                        if (sym == becomes) {
                            getsymdo;       // ��ȡ��ֵ���ұߵ�ֵ
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            expressiondo(nxtlev, ptx, lev);                 // ���㸳ֵ���ұߵı��ʽ���������д��ջ��
                            cur_array_base_id = table[i].base_id;               // ���õ�ǰ�����ַ
                            gendo(sta, lev - table[i].level, table[i].adr);     // ��ջ��ֵд�������Ӧ�±�λ��
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
                    gendo(lod, lev - table[i].level, table[i].adr);                 // LODָ�������ֵȡ��ջ��
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);                                 // ִ�б��ʽ
                    gendo(opr, 0, 2);                                               // OPRָ�ջ���ʹ�ջ����ͣ����д��ջ��
                    if (i != 0) {                                                   // iΪ��д�������λ�ã�iΪ0˵������������
                        gendo(sto, lev - table[i].level, table[i].adr);             // STOָ�ջ������д�����
                    }
                }
                else if (sym == MINUSEQ) {
                    gendo(lod, lev - table[i].level, table[i].adr);
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    gendo(opr, 0, 3);                                               // OPRָ���ջ����ȥջ�������д��ջ��
                    if (i != 0) {
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == INC) {      // ��++
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);     // ȡ����ֵ��ջ��
                        gendo(lit, 0, 1);                                   // ȡ����1��ջ����������
                        gendo(opr, 0, 2);                                   // ��ջ����ջ�����
                        if (i != 0) {
                            gendo(sto, lev - table[i].level, table[i].adr); // ջ�����д�����
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
                else if (sym == DEC) {      // ��--
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);
                        gendo(lit, 0, 1);
                        gendo(opr, 0, 3);                                   // ��ջ����ȥջ��
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
                    error(13);                        /*û�м�⵽��ֵ����*/
                }
            }
        }
    }
    else {
        if (sym == readsym)                            /* ׼������ read ��䴦�� */
        {
            getsymdo;
            if (sym != lparen) {
                error(34);                            /* ��ʽ����Ӧ�������� */
            }
            else {
                do {
                    getsymdo;
                    if (sym == ident) {
                        i = position(id, *ptx);        /* ����Ҫ���ı��� */
                    }
                    else {
                        i = 0;
                    }
                    if (i == 0) {
                        error(35);                    /* read() ��Ӧ���������ı����� */
                    }
                    else {
                        if (table[i].kind == array_type) {
                            getsymdo;
                            if (sym == lparen) {
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[rparen] = true;
                                expressiondo(nxtlev, ptx, lev);                 // ���������±겢���±�ȡ��ջ��
                                gendo(ack, table[i].base_id, table[i].size);    // ��������Ƿ�Խ��
                                gendo(jpc, 0, 0);                               // ��ջ������ֵ�����ж�
                                gendo(opr, 0, 16);                              // �Ӽ��̶����ݵ�ջ��
                                cur_array_base_id = table[i].base_id;
                                gendo(sta, lev - table[i].level, table[i].adr); // ��ջ������д�������Ӧ�±�λ��
                            }
                            else {
                                error(108);
                            }
                        }
                        else if (table[i].kind == real_type) {
                            gendo(opr, 0, 19);      //  ������ָ��������������С��ȡ��ջ��
                            gendo(str, lev - table[i].level, table[i].adr);
                        }
                        else {
                            gendo(opr, 0, 16);            /* �������������ȡֵ��ջ�� */
                            gendo(sto, lev - table[i].level, table[i].adr);        /* ���浽���� */
                        }
                    }
                    getsymdo;
                } while (sym == comma);                /* һ��read���ɶ�������� */
            }
            if (sym != rparen) {
                error(33);                            /* ��ʽ����Ӧ���������� */
                while (!inset(sym, fsys))            /*�����ȣ�ֱ���յ��ϲ㺯���ĺ������*/
                {
                    getsymdo;
                }
            }
            else {
                getsymdo;
            }
        }
        else {
            if (sym == writesym)                    /*׼������write��䴦����read����*/
            {
                getsymdo;
                if (sym == lparen) {
                    do {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        nxtlev[comma] = true;    /*write�ĺ������Ϊ��or,*/
                        expressiondo(nxtlev, ptx, lev);     /*���ñ��ʽ�����˴���read��ͬ��readΪ��������ֵ*/
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
                                    gendo(opr, 0, 14);        /*�������ָ����ջ����ֵ*/
                            }
                        }
                    } while (sym == comma);
                    if (sym != rparen) {
                        error(33);                /*write������ӦΪ�������ʽ*/
                    }
                    else {
                        getsymdo;
                    }
                }
                gendo(opr, 0, 15);                /*�������*/
            }
            else {
                if (sym == callsym)                /*׼������call��䴦��*/
                {
                    getsymdo;
                    if (sym != ident) {
                        error(14);              /* call��ӦΪ��ʶ�� */
                    }
                    else {
                        i = position(id, *ptx);
                        if (i == 0) {
                            error(11);          /* ����δ�ҵ� */
                        }
                        else {
                            if (table[i].kind == procedur) {
                                gendo(cal, lev - table[i].level, table[i].adr); /* ����callָ�� */
                            }
                            else {
                                error(15);      /* call���ʶ��ӦΪ���� */
                            }
                        }
                        getsymdo;
                    }
                }
                else {
                    if (sym == ifsym)                      /* ׼������if��䴦�� */
                    {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;             /* �������Ϊthen��do */
                        conditiondo(nxtlev, ptx, lev);    /* �������������߼����㣩����*/
                        if (sym == thensym) {
                            getsymdo;
                        }
                        else {
                            error(16);                    /* ȱ��then */
                        }
                        cx1 = cx;                         /* ���浱ǰָ���ַ */
                        gendo(jpc, 0, 0);                 /* ����������תָ���ת��ַ��д0 */
                        statementdo(fsys, ptx, lev);      /* ����then������ */
                        // add ELSE
                        if (sym == ELSESYM) {
                            getsymdo;
                            cx2 = cx;       //��¼jmpָ��λ��
                            gendo(jmp, 0, 0);   //��������תָ�� ,����ֱ����ת��else������
                            code[cx1].a = cx;   //��ַ�����jpcָ��ĵ�ַ��ֵ
                            statementdo(fsys, ptx, lev);    //����else��������
                            code[cx2].a = cx;   //else�����������λ�ã���jmpָ��ĵ�ַ��ֵ��if˳��ִ��ʱ����ת����λ��
                        }
                        else {
                            code[cx1].a = cx;                 /* ��statement do�����cxΪthen�����ִ
						    									�����λ�ã�������ǰ��δ������ת��ַ*/
                        }
                    }
                    else {
                        if (sym == beginsym)    /*׼�����ո�����䴦��*/
                        {
                            getsymdo;
                            if (sym == intsym || sym == charsym || sym == realsym) {
                                sym = ident;
                                getsymdo;
                            }
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true;    /*�������Ϊ�ֺŻ�end*/
                            /*ѭ��������䴦������ֱ����һ�����Ų�����俪ʼ���Ż��յ�end*/
                            statementdo(nxtlev, ptx, lev);
                            while (inset(sym, statbegsys) || sym == semicolon) {
                                if (sym == semicolon) {
                                    getsymdo;
                                }
                                else {
                                    error(10);    /*ȱ�ٷֺ�*/
                                }
                                statementdo(nxtlev, ptx, lev);
                            }
                            if (sym == endsym) {
                                getsymdo;
                            }
                            else {
                                error(17);    /*ȱ��end��ֺ�*/
                            }
                        }
                        else {
                            if (sym == whilesym)    /*׼������while��䴦��*/
                            {
                                cx1 = cx;    /*�����ж�����������λ��*/
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[dosym] = true;    /*�������Ϊdo*/
                                conditiondo(nxtlev, ptx, lev);    /*������������*/
                                cx2 = cx;    /*����ѭ����Ľ�����һ��λ��*/
                                gendo(jpc, 0, 0);    /*����������ת,������ѭ���ĵ�ַδ֪*/
                                if (sym == dosym) {
                                    getsymdo;
                                }
                                else {
                                    error(18);    /*ȱ��do*/
                                }
                                statementdo(fsys, ptx, lev);    /*ѭ����*/
                                gendo(jmp, 0, cx1);    /*��ͷ�����ж�����*/
                                code[cx2].a = cx;        /* ��������ѭ���ĵ�ַ����if���� */
                            }
                            else {
                                if (sym == FORSYM) {
//                                    printf("keyword: FOR\n");
                                    getsymdo;
                                    if (sym != ident) {
                                        error(101);                             // ����ӱ���FOR������Ҫ����
                                    }
                                    i = position(id, *ptx);                 // ���ұ���λ��
                                    if (i == 0) {
                                        error(11);
                                    }
                                    else {
                                        if (table[i].kind != variable) {
                                            error(12);                            /*��ֵ����ʽ����*/
                                            i = 0;
                                        }
                                        else {
                                            getsymdo;
                                            if (sym != becomes) {
                                                error(13);                        /*û�м�⵽��ֵ����*/
                                            }
                                            else {
                                                getsymdo;
                                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                                nxtlev[TOSYM] = true;               // �����һ��Ĵ���
                                                expressiondo(nxtlev, ptx, lev);     // ִ�б��ʽ����������ջ��
                                                gendo(sto, lev - table[i].level, table[i].adr);     // STOָ����ֵ
                                                if (sym == TOSYM) {
                                                    cx1 = cx;           // ����ѭ����ʼλ��
                                                    getsymdo;
                                                    gendo(lod, lev - table[i].level, table[i].adr);     // ѭ������ȡ��ջ��
                                                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                                    nxtlev[dosym] = true;       // �����һ��Ĵ���
                                                    expressiondo(nxtlev, ptx, lev);     // ִ�б��ʽ����������ջ��
                                                    gendo(opr, 0, 13);      // �Ƚϴ�ջ���Ƿ�С�ڵ���ջ���������ջ
                                                    cx2 = cx;               // ��¼JPCָ���λ�ã����ں�������jpc��ת��ַ
                                                    gendo(jpc, 0, 0);       // ����JPC��תָ���ַ����0
                                                    if (sym == dosym) {     // ��⵽ do������ѭ���崦��
                                                        getsymdo;
                                                        statementdo(fsys, ptx, lev);    // ѭ������ʽִ��
                                                        gendo(lod, lev - table[i].level, table[i].adr);     // ѭ������ȡ��ջ��
                                                        gendo(lit, 0, step);            // ����ȡ��ջ��
                                                        gendo(opr, 0, 2);               // OPRָ��ִ�д�ջ����ջ�����
                                                        gendo(sto, lev - table[i].level, table[i].adr);     // ���д��ѭ������
                                                        gendo(jmp, 0, cx1);             // ��������ת��ѭ����ʼλ��
                                                        code[cx2].a = cx;               // JPC��ת��ַ������ѭ������λ��
                                                    }
                                                    else {
                                                        error(103);             // ����ӱ���ȱ��do
                                                    }
                                                }
                                                else if (sym == DOWNTOSYM) {
                                                    cx1 = cx;
                                                    getsymdo;
                                                    gendo(lod, lev - table[i].level, table[i].adr);
                                                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                                    nxtlev[dosym] = true;
                                                    expressiondo(nxtlev, ptx, lev);
                                                    gendo(opr, 0, 11);      // �Ƚϴ�ջ���Ƿ���ڵ���ջ���������ջ
                                                    cx2 = cx;
                                                    gendo(jpc, 0, 0);
                                                    if (sym == dosym) {
                                                        getsymdo;
                                                        statementdo(fsys, ptx, lev);
                                                        gendo(lod, lev - table[i].level, table[i].adr);
                                                        gendo(lit, 0, step);
                                                        gendo(opr, 0, 3);               // OPRָ��ִ�д�ջ����ȥջ��
                                                        gendo(sto, lev - table[i].level, table[i].adr);
                                                        gendo(jmp, 0, cx1);
                                                        code[cx2].a = cx;
                                                    }
                                                    else {
                                                        error(103);
                                                    }
                                                }
                                                else {
                                                    error(102);                 // ����ӱ���ȱ��TO����DOWNTO
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
                                                    if (sym == INC) {       // ǰ++
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
                                                                        // LODָ��ȡ����ֵ��ջ��
                                                                        gendo(lod, lev - table[i].level, table[i].adr);
                                                                        gendo(lit, 0, 1);              // ȡ����1��ջ��
                                                                        gendo(opr, 0,2);               // ��ջ����ջ�����
                                                                        if (i != 0) {
                                                                            // ջ�����д�����
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
                                                            error(104);             // �Զ��屨��++--��ȱ�ٱ���
                                                        }
                                                    }
                                                    else {
                                                        if (sym == MINUSEQ) {
                                                            printf("keyword: -=\n");
                                                            getsymdo;
                                                        }
                                                        else {
                                                            if (sym == DEC) {       // ǰ--
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
                                                                                gendo(opr, 0, 3);       // ��ջ����ȥջ��
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
                                                                       sizeof(bool) * symnum);    /* �������޲��ȼ��� */
                                                                testdo(fsys, nxtlev, 19);    /* �������������ȷ�� */
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
 * ���ʽ����
 */
int expression(bool *fsys, int *ptx, int lev) {
    enum symbol addop;    /*���ڱ���������*/
    bool nxtlev[symnum];
    int i;
    if (sym == plus || sym == minus)    /*��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵���*/
    {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);    /*������*/
        if (addop == minus) {
            gendo(opr, 0, 1);    /*�����ͷΪ���ţ�����ȡ��ָ��*/
        }
    }
    else if (sym == INC) {      // ǰ++
        getsymdo;
        if (sym == ident) {
            i = position(id, *ptx);                 // ���ұ���λ��
            if (i == 0) {
                error(11);
            }
            else {
                if (table[i].kind != variable) {            // �жϱ�������
                    error(12);
                    i = 0;
                }
                else {
                    if (i != 0) {
                        gendo(lod, lev - table[i].level, table[i].adr);
                        gendo(lit, 0, 1);
                        gendo(opr, 0, 2);       // ��
                        if (i != 0) {
                            gendo(sto, lev - table[i].level, table[i].adr);         // ջ�����д�����
                            gendo(lod, lev - table[i].level, table[i].adr);         // ȡ����ֵ��ջ����Ϊ���ʽ���
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
    else if (sym == DEC) {      // ǰ--
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
                        gendo(opr, 0, 3);       // ��
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
    else    /*��ʱ���ʽ��������ļӼ�*/
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);    /*������*/
    }
    while (sym == plus || sym == minus) {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);             /*������*/
        if (addop == plus) {
            gendo(opr, 0, 2);                 /*���ɼӷ�ָ��*/
        }
        else {
            gendo(opr, 0, 3);                 /*���ɼ���ָ��*/
        }
    }
    return 0;
}

/*
* ���
*/
int term(bool *fsys, int *ptx, int lev) {
    enum symbol mulop;        /*���ڱ���˳�������*/
    bool nxtlev[symnum];
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    factordo(nxtlev, ptx, lev);        /*��������*/
    while (sym == times || sym == slash) {
        mulop = sym;
        getsymdo;
        factordo(nxtlev, ptx, lev);
        if (mulop == times) {
            gendo(opr, 0, 4);    /*���ɳ˷�ָ��*/
        }
        else {
            gendo(opr, 0, 5);    /*���ɳ���ָ��*/
        }
    }
    return 0;
}

/*
 * ���Ӵ���
 */
int factor(bool *fsys, int *ptx, int lev) {
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys, fsys, 24);                    /*������ӵĿ�ʼ����*/
    while (inset(sym, facbegsys))                    /*ѭ��ֱ���������ӿ�ʼ����*/
    {
        if (sym == ident)                            /*����Ϊ���������*/
        {
            i = position(id, *ptx);                    /*��������*/
            if (i == 0) {
                error(11);                            /*��ʶ��δ����*/
            }
            else {
                switch (table[i].kind) {
                    case constant:                    /*����Ϊ����*/
                        gendo(lit, 0, table[i].val);/*ֱ�Ӱѳ�����ֵ��ջ*/
                        break;
                    case int_type:
                    case char_type:
                    case variable:                    /*����Ϊ����*/
                        gendo(lod, lev - table[i].level, table[i].adr);/*�ҵ�������ַ������ֵ��ջ*/
                        break;
                    case real_type:
                        gendo(ldr, lev - table[i].level, table[i].adr);     // ������ָ�real������ֵȡ��ջ��
                        break;
                    case array_type:
                        getsymdo;
                        if (sym == lparen) {
                            getsymdo;
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[rparen] = true;
                            expressiondo(nxtlev, ptx, lev);                 // ���������±겢���±�д��ջ��
                            gendo(ack, table[i].base_id, table[i].size);    // ��������Ƿ�Խ��
                            gendo(jpc, 0, 0);                               // ��ջ������ֵ�����ж�
                            cur_array_base_id = table[i].base_id;
                            gendo(lda, lev - table[i].level, table[i].adr); // �������Ӧ�±������ȡ��ջ��
                        }
                        else {
                            error(108);
                        }
                        break;
                    case procedur:                    /*����Ϊ����*/
                        error(21);                    /*����Ϊ����*/
                        break;
                }
            }
            getsymdo;
            if (sym == INC || sym == DEC) {         // ��++ --
                gendo(lit, 0, 1);                                       // ȡ����1����ջ��
                if (sym == INC) {
                    gendo(opr, 0, 2);                                   // ��ջ����ջ�����
                    gendo(sto, lev - table[i].level, table[i].adr);     // ջ������д�������Ԫ
                    gendo(lod, lev - table[i].level, table[i].adr);     // ������ֵ����ջ��
                    gendo(lit, 0, 1);                                   // ���ں��������ı���ʽ��ֵ
                    gendo(opr, 0, 3);                                   // ����Ҫ��ȥ1
                }
                else if (sym == DEC) {
                    gendo(opr, 0, 3);                                   // ��ջ����ȥջ��
                    gendo(sto, lev - table[i].level, table[i].adr);
                    gendo(lod, lev - table[i].level, table[i].adr);
                    gendo(lit, 0, 1);
                    gendo(opr, 0, 2);                                   // ���Լ����1
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
                    gendo(lit, 0, dec_part);        // ��ȡС�����ֵ�ջ��
                    gendo(lit, 0, int_part);        // ��ȡ�������ֵ�ջ��
                    getsymdo;
                }
                else {
                    gendo(lit, 0, num);
                    getsymdo;
                }
            }
            else {
                if (sym == lparen)                    /*����Ϊ���ʽ*/
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expressiondo(nxtlev, ptx, lev);
                    if (sym == rparen) {
                        getsymdo;
                    }
                    else {
                        error(22);                    /*ȱ��������*/
                    }
                }
                testdo(fsys, facbegsys, 23);        /*���Ӻ��зǷ�����*/
            }
        }
    }
    return 0;
}

/*
*��������
*/
int condition(bool *fsys, int *ptx, int lev) {
    enum symbol relop;
    bool nxtlev[symnum];
    if (sym == oddsym)                                /*׼������ odd ���㴦��*/
    {
        getsymdo;
        expressiondo(fsys, ptx, lev);
        gendo(opr, 0, 6);                             /*���� odd ָ��*/
    }
    else {
        /*�߼����ʽ����*/
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
* ���ͳ���
*/
void interpret() {
    int p, b, t;               /*ָ��ָ�룬ָ���ַ��ջ��ָ��*/
    struct instruction i;   /*��ŵ�ǰָ��*/
    int s[stacksize];       /*ջ*/

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
                        str_p = strtok(temp, ".");      // ��С����ָ��ַ���
                        before_dot = atoi(str_p);       // ���������ַ���תΪint��
                        str_p = strtok(NULL, " ");
                        if (str_p != NULL) {            // С������Ϊ����ȡ0���ǿ���ת��Ӧ�ַ���Ϊint��
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
