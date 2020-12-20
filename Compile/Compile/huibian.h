#pragma once
// 实现一个汇编器
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h> 
#define INS_COUNT (sizeof(g_ins_name) / sizeof(g_ins_name[0])) 
#define INS_SYM {"HLT", "JMP", "CJMP", "OJMP", "CALL", "RET","PUSH", "POP", "LOADB", "LOADW", "STOREB", "STOREW","LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB","SUBI", "MUL", "DIV", "AND", "OR", "NOR", "NOTB","SAL", "SAR", "EQU", "LT", "LTE", "NOTC"} 
const char *g_ins_name[] = INS_SYM; /*定义存放指令助记符的数组*/
/*8个数字代表八种存放指令的格式*/
const char ins_format[33] = "12222133444451667575777778778881";
typedef struct _BIAOHAO_INFO { /*用于存储标号信息的链表*/
	char name[30];
	int line_num;
	struct _BIAOHAO_INFO *next;
}biaohao;
typedef struct _VAR { /*用于存储变量信息的链表*/
	char name[30];
	int size; /*存储变量的个数*/
	int pianyi; /*偏移量*/
	short value[256];
	int zijie; /*用于判断变量的类型*/
	struct _VAR *next;
}var;
int getinscode(const char *op_sym); /*由助记符得指令代码*/
unsigned long transtocode(char *instr_line, int instr_num, biaohao *hd1, var *hd2);/*指令的译码*/
int getregnum(char *instr_line, char *reg_name);/*由寄存器名对应到编码*/
int save_biaohao(char *biaohao_name, int line_num, biaohao *hd);/*保存标号的信息在链表里*/
int save_var(char *a_line, int pianyi, int size, var *hd, int zijie);/*保存标号的信息在链表里*/
long write_var(FILE *pfout, var *hd);/*最后输出变量的值在目标文件里*/


/*存放标号信息的链表*/
int save_biaohao(char *biaohao_name, int line_num, biaohao *hd)
{
	biaohao *p;
	for (p = hd; p->next != NULL; p = p->next)
	{
		;
	}
	biaohao *pnew = (biaohao *)malloc(sizeof(biaohao));
	p->next = pnew;
	pnew->line_num = line_num;
	strcpy_s(pnew->name, biaohao_name);
	pnew->next = NULL;
	return 1;
}
/*存放变量信息的链表*/
int save_var(char *a_line, int pianyi, int size, var *hd, int zijie)
{
	var *p1;
	short value1;
	char *p = NULL;
	char value_str[20];
	int i, k, fuzhi = 0;
	char varname_temp[20]; /*存放临时的变量名称*/
	char string[200]; /*存放定义变量name和size的字符串*/
	sscanf_s(a_line, "%*s %s", string);
	sscanf_s(string, "%[^[]", varname_temp); /*存变量名*/

	for (p1 = hd; p1->next != NULL; p1 = p1->next)
	{
		;
	}
	var *pnew = (var *)malloc(sizeof(var));
	p1->next = pnew;
	pnew->pianyi = pianyi; /*偏移量*/
	pnew->size = size; /*变量存值多少个*/
	pnew->zijie = zijie; /*字节为1或2分别为byte型及word型数据*/
	strcpy_s(pnew->name, varname_temp); /*变量名称*/
	pnew->next = NULL;
	/*以下对变量的赋值进行处理*/
	for (i = 0; i < size; i++)
	{
		pnew->value[i] = 0;
	}
	if (p = strchr(a_line, '='))
	{
		if (size == 1) /*只有一个值*/
		{
			sscanf_s(a_line, "%*[^=]=%d", &value1);
			pnew->value[0] = value1;
		}
		else
		{
			for (; p != NULL; p++)
			{
				if (*p == '{')
				{
					++p;
					fuzhi = 1; /*表示将会被赋值*/
					k = 0;
					while (*p != '}')
					{
						for (i = 0; *p != ',' && *p != '}'; p++)
						{
							value_str[i++] = *p;
						}
						value_str[i] = '\0';
						pnew->value[k++] = atoi(value_str);
						if (*p == ',')
						{
							++p;
						}
					}
				}
				else if (*p == '"')
				{
					fuzhi = 1; /*表示将会被赋值*/
					++p;
					k = 0;
					for (i = 0; *p != '"'; p++)
					{
						pnew->value[k++] = *p;
					}
					pnew->value[k] = '\0';
				}
				if (fuzhi) { break; }
			}
		}

	}
	return 0;
}

/*由指令助记符得到指令操作码*/
int getinscode(const char *op_sym) {
	int i;
	for (i = 0; i < 32; i++)
	{
		if (strcmp(g_ins_name[i], op_sym) == 0)
		{
			break;
		}
	}
	return i;
}
/*由寄存器名得到寄存器编号*/
int getregnum(char *ins_line, char *reg_name) {
	int reg_num;
	if (tolower(*reg_name) == 'z')
	{
		reg_num = 0;
	}
	else if ((tolower(*reg_name) >= 'a') && (tolower(*reg_name) <= 'g'))
	{
		reg_num = tolower(*reg_name) - 'a' + 1;
	}
	else
	{
		printf("ERROR:bad register name in %s!\n", ins_line);
		exit(-1);
	}
	return reg_num;
}
/*将指令翻译成目标代码， ins_num为指令操作码*/

unsigned long transtocode(char *ins_line, int ins_num, biaohao *hd1, var *hd2)
{
	unsigned long op_code; /*op_code为指令操作码*/
	unsigned long arg1, arg2, arg3; /*arg为寄存器对应的编号*/
	unsigned long ins_code = 0ul;
	char op_sym[8], reg0[8], reg1[8], reg2[8];/*op_sym为指令助记符reg为寄存器名*/
	unsigned long addr;
	int immed, port, n;
	char string[20];
	biaohao *p1;
	var *p2;
	switch (ins_format[ins_num])
	{
	case '1': /*类型 op(5b) + padding(27)*/
	{
		op_code = ins_num;
		ins_code = op_code << 27;
		break;
	}
	case '2': /*类型op(5b) + padding(3b) + address(24b)*/
	{
		n = sscanf_s(ins_line, "%s %s", op_sym, sizeof(op_sym), string, sizeof(string));
		if (n < 2)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		for (p1 = hd1->next; p1 != NULL; p1 = p1->next)
		{
			if (strcmp(p1->name, string) == 0)
			{
				break;
			}
		}
		if (p1 == NULL)
		{
			printf("ERROR:%s", ins_line);
			exit(-1);
		}
		addr = (unsigned long)((p1->line_num) * 4);
		op_code = getinscode(op_sym);/*由助记符得到操作码*/
		ins_code = (op_code << 27) | (addr & 0x00ffffff);
		break;
	}
	case '3': /*类型op(5b) + reg0(3b) + padding(24b)*/
	{
		n = sscanf_s(ins_line, "%s %s", op_sym, sizeof(op_sym), reg0, sizeof(reg0));
		if (n < 2)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getinscode(op_sym);
		/*由寄存器名得到寄存器编号*/
		arg1 = getregnum(ins_line, reg0);
		ins_code = (op_code << 27) | (arg1 << 24);
		break;
	}
	case '4': /*类型op(5b) + reg0(3b) + address(24b)*/
	{
		n = sscanf_s(ins_line, "%s %s %s", op_sym, sizeof(op_sym), reg0, sizeof(reg0), string);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		for (p2 = hd2->next; p2 != NULL; p2 = p2->next)
		{
			if (strcmp(p2->name, string) == 0)
			{
				break;
			}
		}
		if (p2 == NULL)
		{
			printf("ERROR:%s", ins_line);
			exit(-1);
		}
		addr = (unsigned long)(p2->pianyi);
		op_code = getinscode(op_sym);
		/*由寄存器名得到寄存器编号*/
		arg1 = getregnum(ins_line, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (addr & 0x00ffffff);
		break;
	}
	case '5': /*类型 op(5b)+reg0(3b)+padding(8b)+immediate(16b)*/
	{
		n = sscanf_s(ins_line, "%s %s %i", op_sym, sizeof(op_sym), reg0, sizeof(reg0), &immed);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getinscode(op_sym);

		arg1 = getregnum(ins_line, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (immed & 0x0000ffff);
		break;
	}
	case '6': /*类型op(5b) + reg0(3b) + padding(16b) + port(8b)*/
	{
		n = sscanf_s(ins_line, "%s %s %i", op_sym, sizeof(op_sym), reg0, sizeof(reg0), &port);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getinscode(op_sym);
		arg1 = getregnum(ins_line, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (port & 0x0000ffff);
		break;
	}
	case '7':/*类型op(5b) + reg0(3b) + reg1(4b) + reg2(4b) + padding(16b)*/
	{
		n = sscanf_s(ins_line, "%s%s%s%s", op_sym, sizeof(op_sym), reg0, sizeof(reg0), reg1, sizeof(reg1), reg2, sizeof(reg2));
		if (n < 4)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getinscode(op_sym);
		arg1 = getregnum(ins_line, reg0);
		arg2 = getregnum(ins_line, reg1);
		arg3 = getregnum(ins_line, reg2);
		ins_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20) | (arg3 << 16);
		break;

	}
	case '8':/*类型op(5b)+reg0(3b)+reg1(4b)+reg2(4b)+padding(16b)*/
	{
		n = sscanf_s(ins_line, "%s%s%s", op_sym, sizeof(op_sym), reg0, sizeof(reg0), reg1, sizeof(reg1));
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getinscode(op_sym);
		arg1 = getregnum(ins_line, reg0);
		arg2 = getregnum(ins_line, reg1);
		ins_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20);
		break;
	}
	}
	return ins_code; /*返回目标代码*/
}
/*最后输出变量的值在目标文件里*/
long write_var(FILE *pfout, var *hd) {
	var *p;
	int i;
	long k = 0;
	unsigned long addr = 0ul;
	for (p = hd->next; p != NULL; p = p->next)
	{
		for (i = 0; i < p->size; i++)
		{
			if ((k % 4) == 0)
			{
				if ((p->zijie) == 1)
				{
					addr += p->value[i];
					k += 1;
				}
				else if ((p->zijie) == 2)
				{
					addr += (p->value[i]) << 8;
					k += 2;
				}
			}
			else if ((k % 4) == 1)
			{
				if ((p->zijie) == 1)
				{
					addr += (p->value[i]) << 8;
					k += 1;
				}
				else if ((p->zijie) == 2)
				{
					addr += (p->value[i]) << 16;
					k += 2;
				}
			}
			else if ((k % 4) == 2)
			{
				if ((p->zijie) == 1)
				{
					addr += (p->value[i]) << 16;
					k += 1;
				}
				else if ((p->zijie) == 2)
				{
					addr += (p->value[i]) << 24;
					k += 2;
					printf("0x%08lx\n", addr);
					fprintf(pfout, "0x%08lx\n", addr);
					addr = 0ul;
				}
			}
			else if ((k % 4) == 3)
			{
				if ((p->zijie) == 1)
				{
					addr += (p->value[i]) << 24;
					k += 1;
					printf("0x%08lx\n", addr);
					fprintf(pfout, "0x%08lx\n", addr);
					addr = 0ul;
				}
				else if ((p->zijie) == 2)
				{
					addr += ((p->value[i]) & 0x00ff) << 24;
					k += 2;
					printf("0x%08lx\n", addr);
					fprintf(pfout, "0x%08lx\n", addr);
					addr = (p->value[i]) >> 8;
				}
			}
		}
	}
	if ((k % 4) != 0)
	{
		printf("0x%08lx\n", addr);
		fprintf(pfout, "0x%08lx\n", addr);
	}
	printf("0x%08lx\n", addr);
	fprintf(pfout, "0x%08lx\n", k);
	return k;
}

void compile() {
	char a_line[200];
	char op_sym[8];
	int op_num = 0, i;
	char *pcpos;
	int zijie;
	FILE *pfin, *pfout;
	//输入文件位置
	const char *filename_in = "C:\\Users\\wonder\\Desktop\\生产实习\\sum.txt";
	////输出文件位置
	//const char *filename_out = "C:\\Users\\wonder\\Desktop\\生产实习\\Compile\\sumout.dat";
	/*建立标号链表和变量的链表头指针,但头指针没放数据*/
	biaohao *hd_biaohao = (biaohao *)malloc(sizeof(biaohao));
	hd_biaohao->next = NULL;
	var *hd_var = (var *)malloc(sizeof(var));
	hd_var->next = NULL;
	char biaohao_temp[20]; /*存放临时的标号名称*/
	char string[200]; /*存放定义变量name和size的字符串*/
	char var_size_str[20]; /*存放变量的大小(字符串格式)*/
	int var_size; /*存放变量的大小(int型格式)*/
	int pianyi_num = 0;
	int n;
	int line_num = 0; /*存代码当前的行数*/
	//输入汇编源程序文件名，打开文件
	fopen_s(&pfin, filename_in, "r");
	if (pfin == NULL) {
		printf("ERROR: cannot open file\n");
		return;
	}
	while (!feof(pfin))
	{
		fgets(a_line, 200, pfin); /*从源文件取一行命令*/
		if ((pcpos = strchr(a_line, '#')) != NULL)
		{
			*pcpos = '\0'; /*去掉注释*/
		}
		n = sscanf_s(a_line, "%s", op_sym, sizeof(op_sym)); /*取指令助记符*/

		if (n < 1) /*空行和注释行的处理*/
		{
			continue;
		}
		if ((pcpos = strchr(a_line, ':')) != NULL)
		{
			sscanf_s(a_line, "%[^:]", biaohao_temp, sizeof(biaohao_temp));
			save_biaohao(biaohao_temp, line_num, hd_biaohao);
		}
		else if (strstr(a_line, "WORD") != NULL)
		{
			sscanf_s(a_line, "%*s %s", string);
			zijie = 2;
			if ((pcpos = strchr(string, '[')) != NULL) /*若变量为数组格式*/
			{
				sscanf_s(string, "%*[^[][%[^]]", var_size_str);/*存变量大小*/
				var_size = atoi(var_size_str);
			}
			else
			{
				var_size = 1;
			}
			save_var(a_line, pianyi_num, var_size, hd_var, zijie);
			pianyi_num += 2 * var_size; /*一个word型值占俩字节*/
			line_num--;
		}
		else if (strstr(a_line, "BYTE") != NULL)
		{
			sscanf_s(a_line, "%*s %s", string);
			zijie = 1;
			if ((pcpos = strchr(string, '[')) != NULL) /*若变量为数组格式*/
			{
				sscanf_s(string, "%*[^[][%[^]]", var_size_str);/*存变量大小*/
				var_size = atoi(var_size_str);
			}
			else
			{
				var_size = 1;
			}
			save_var(a_line, pianyi_num, var_size, hd_var, zijie);
			pianyi_num += var_size; /*一个byte型值占一个字节*/
			line_num--;
		}
		line_num++;
	}

	fclose(pfin);
	//第二次读源文件，处理32个指令助记符 
	fopen_s(&pfin, filename_in, "r");
	if (pfin == NULL) {
		printf("ERROR: cannot open file\n");
		return;
	}
	/*fopen_s(&pfout, filename_out, "w");
	if (pfout == NULL) {
		printf("ERROR: cannot open file\n");
		return ;
	}*/
	while (!feof(pfin))
	{
		fgets(a_line, 200, pfin);
		if ((pcpos = strchr(a_line, '#')) != NULL)
		{
			*pcpos = '\0';
		}
		if ((pcpos = strstr(a_line, "WORD")) != NULL)
		{
			continue;
		}
		else if ((pcpos = strstr(a_line, "BYTE")) != NULL)
		{
			continue;
		}
		else if ((pcpos = strchr(a_line, ':')) != NULL) /*去掉标号*/
		{
			for (i = 0; a_line[i] != ':'; i++)
			{
				a_line[i] = ' ';
			}
			a_line[i] = ' ';
		}
		n = sscanf_s(a_line, "%s", op_sym, sizeof(op_sym));
		if (n < 1) {
			continue;
		}

		op_num = getinscode(op_sym); /*由助记符得到指令的操作码*/
		//非法指令处理 
		if (op_num > 31)
		{
			printf("ERROR: %s is a invalid instruction! \n", a_line);
			exit(-1);
		}
		printf("0x%08lx\n", transtocode(a_line, op_num, hd_biaohao, hd_var));
		/*fprintf(pfout, "0x%08lx\n", transtocode(a_line, op_num, hd_biaohao, hd_var));*/

	}
	//write_var(pfout, hd_var); /*将变量的值以十六进制输出进pfout文件中*/
	fclose(pfin);
	/*fclose(pfout);*/
	printf("success!!\n");
	return;
}