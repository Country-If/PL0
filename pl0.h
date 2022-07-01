// #pragma once

// #include <stdio.h>


typedef enum {
    false,
    true
} bool;


#define norw 20         // sym中的保留字数量(*sym/*SYM)
#define txmax 100
#define nmax 14
#define al 10
#define amax 2047
#define levmax 3
#define cxmax 200
#define step 2      // 定义FOR语句循环变量的步长


enum symbol {
    nul, ident, number, plus, minus,                // 5
    times, slash, oddsym, eql, neq,                 // 10
    lss, leq, gtr, geq, lparen,                     // 15
    rparen, comma, semicolon, period, becomes,      // 20
    beginsym, endsym, ifsym, thensym, whilesym,     // 25
    writesym, readsym, dosym, callsym, constsym,    // 30
    varsym, procsym, FORSYM, TOSYM, DOWNTOSYM,      // 35
    PLUSEQ, MINUSEQ, INC, DEC, RETURNSYM,           // 40
    ELSESYM, intsym, charsym, colon,                // 44
};
#define symnum 44       // sym数量

/*名字表中的类型*/

enum object {
    constant,
    variable,
    procedur,
    int_type,
    char_type,
    array_type,
};

/*虚拟机代码*/

enum fct {
    lit, opr, lod,
    sto, cal, inte,
    jmp, jpc, sta,  // sta: store array
    lda, ack,       // lda: load arr; ack: array check
};
#define fctnum 11

/*虚拟机代码结构*/

struct instruction {
    enum fct f;                /*虚拟机代码指令*/
    int l;                    /*引用层与生命层的层次差*/
    int a;                    /*根据f的不同而不同*/
};


FILE *fas;              /* 输出名字表 */
FILE *fa;               /* 输出虚拟机代码 */
FILE *fa1;              /* 输出源文件及其各行对应的首地址 */
FILE *fa2;              /* 输出结果 */
bool listswitch;        /* 显示虚拟机代码与否 */
bool tableswitch;        /* 显示名字表与否 */
char ch;                /* 获取字符的缓冲区，getch使用 */
enum symbol sym;        /* 当前的符号 */
char id[al + 1];          /* 当前ident，多出的一个字节用于存放0 */
int num;                /* 当前number */
int cc, ll;             /* getch使用的计数器，cc表示当前字符（ch）的位置 */
int cx;                 /* 虚拟机代码指针，取值范围[0,cxmax-1] */
char line[81];          /* 读取行缓冲区 */
char a[al + 1];           /* 临时符号，多出的一个字节用于存放0 */
struct instruction code[cxmax]; /* 存放虚拟机代码的数组*/
char word[norw][al];    /* 保留字 */
enum symbol wsym[norw]; /* 保留字对应的符号值 */
enum symbol ssym[256];  /* 单字符的符号值 */
char mnemonic[fctnum][5]; /* 虚拟机代码指令名称 */
bool declbegsys[symnum];/* 表示声明开始的符号集合 */
bool statbegsys[symnum];/* 表示语句开始的符号集合 */
bool facbegsys[symnum]; /* 表示因子开始的符号集合 */
/* 名字表结构 */
struct tablestruct {
    char name[al];      /* 名字 */
    enum object kind;   /* 类型：const，var，array or procedure */
    int val;            /* 数值，仅const使用 */
    int level;          /* 所处层，仅const不使用*/
    int adr;            /* 地址，仅const不使用 */
    int size;           /* 需要分配的数据区空间，仅procedure使用 */
    int base_id;        // 数组基址
};
struct tablestruct table[txmax]; /* 名字表 */
FILE *fin;
FILE *fout;
char fname[al];
int err;                /* 错误计数器 */


/* 当函数中会发生fatal error 时，返回-1告知调用它的函数，最终退出程序 */
#define getsymdo                   if(-1==getsym()) return -1
#define getchdo                    if(-1==getch()) return -1
#define testdo(a, b, c)              if(-1==test(a,b,c)) return -1
#define gendo(a, b, c)               if(-1==gen(a,b,c)) return -1
#define expressiondo(a, b, c)        if(-1==expression(a,b,c)) return -1
#define factordo(a, b, c)            if(-1==factor(a,b,c)) return -1
#define termdo(a, b, c)              if(-1==term(a,b,c)) return -1
#define conditiondo(a, b, c)         if(-1==condition(a,b,c)) return -1
#define statementdo(a, b, c)         if(-1==statement(a,b,c)) return -1
#define constdeclarationdo(a, b, c)  if(-1==constdeclaration(a,b,c)) return -1
#define vardeclarationdo(a, b, c)    if(-1==vardeclaration(a,b,c)) return -1
#define intdeclarationdo(a, b, c)    if(-1==intdeclaration(a,b,c)) return -1
#define chardeclarationdo(a, b, c)    if(-1==chardeclaration(a,b,c)) return -1

void error(int n);

int getsym();

int getch();

void init();

int gen(enum fct x, int y, int z);

int test(bool *s1, bool *s2, int n);

int inset(int e, bool *s);

int addset(bool *sr, bool *s1, bool *s2, int n);

int subset(bool *sr, bool *s1, bool *s2, int n);

int mulset(bool *sr, bool *s1, bool *s2, int n);

int block(int lev, int tx, bool *fsys);

void interpret();

int factor(bool *fsys, int *ptx, int lev);

int term(bool *fsys, int *ptx, int lev);

int condition(bool *fsys, int *ptx, int lev);

int expression(bool *fsys, int *ptx, int lev);

int statement(bool *fsys, int *ptx, int lev);

void listcode(int cx0);

int vardeclaration(int *ptx, int lev, int *pdx);

int intdeclaration(int *ptx, int lev, int *pdx);

int chardeclaration(int *ptx, int lev, int *pdx);

int constdeclaration(int *ptx, int lev, int *pdx);

int position(char *idt, int tx);

void enter(enum object k, int *ptx, int lev, int *pdx);

int base(int l, int *s, int b);

void print_time();