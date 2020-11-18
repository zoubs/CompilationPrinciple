#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
#include<string>

using namespace std;

#define TABLENUM 200 
#define CODENUM 300
int errors = 0;     //错误总数
int whilenum = 0, donum = 0, thennum = 0, ifnum = 0;
//**************************************************************************
enum symbol //字符类型
{							 
	ident, programS, varS, procedureS, beginS, endS, ifS, thenS, elseS,
	whileS, doS, callS, readS, writeS, oddS, constS, number, equalS, plusS, minusS,
	multiplyS, involution, divide, more, notless, lessS, notmore, notequal,
	assignment, comma, semicolon, leftbrackets, rightbrackets, undefine
};
//*************************************************
enum OpCode    //操作码枚举类型
{
	LIT, OPR, LOD, STO, CAL, RET, INT, JMP, JPC, READ, WRITE
};
struct Wordtable            //单词表
{							 
	enum symbol SYM;   //单词类型 
	char ID[15];	   //标识符内容 
	int NUM;		   //常数内容 
	int lineN;		   //行号 
}tab;
//*************************************************
typedef struct tablelink 
{				    //单词链表结构体 
	enum symbol SYM;//单词类型
	char ID[15];	//标识符内容
	int NUM;		//常数内容
	int lineN;		//行号
	struct tablelink* next;//下一单元地址 
}tabnode, * linklist;
//*************************************************
struct  table          //符号表
{							
	enum symbol type;  //符号类型 
	char name[15];	   //符号名称 
	int num;		   //常数值或空间
	int level;         //所在层次
	char levelname[15];  //层名
	int adr;           //相对偏移量
}ta[TABLENUM];
//*************************************************
struct Code
{
	enum OpCode Op;     //操作码类型
	int lev;            //层差
	int adr;            //相对位移量
}code[CODENUM];
//*************************************************
void freelink(linklist& tb)
{						//销毁链表 
	linklist q;
	while (tb != NULL)
	{
		q = tb;
		tb = tb->next;
		free(q);
	}
}
//**************************************************************************

#define Stack_Init_Size 1000	//默认最大长度为100
#define STACKINCREMENT 100	//空间增量为100
typedef int SElemType;
typedef struct SqStack								//定义栈结构
{
	SElemType* base;//栈底指针
	SElemType* top;	//栈顶指针
	int Stacksize;	//当前分配的存储容量
}Stack;
Stack S;
//*************************************************
void InitStack(SqStack& S)					//构造空的栈
{
	S.base = (SElemType*)malloc(Stack_Init_Size * sizeof(SElemType));//申请空间
	S.top = S.base;
	S.Stacksize = Stack_Init_Size;
}
//*************************************************
void DestroyStack(SqStack& S)				//销毁栈
{
	S.top = S.base;
	S.Stacksize = 0;
	free(S.base);
	S.base = NULL;
	S.top = NULL;
}
//*************************************************
void StackPush(SqStack& S, SElemType e)		//插入新的栈顶元素
{
	SElemType* newbase;
	if ((S.top - S.base) == Stack_Init_Size)	//若空间已满则重新申请空间
	{
		newbase = (SElemType*)realloc(S.base, (S.Stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!newbase)
			exit(0);
		S.base = newbase;
		S.top = S.base + Stack_Init_Size;
		S.Stacksize += STACKINCREMENT;
	}
	*(S.top) = e;
	S.top++;
}
//*************************************************
void GetESem(SqStack S, SElemType& e)		//返回栈顶元素
{
	if (S.top != S.base)
		e = *(S.top - 1);
	else
	{
		printf("栈为空！\n");
		exit(0);
	}
}

#pragma once