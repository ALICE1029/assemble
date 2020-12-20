#pragma once
// ʵ��һ�������
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h> 
#define INS_COUNT (sizeof(g_ins_name) / sizeof(g_ins_name[0])) 
#define INS_SYM {"HLT", "JMP", "CJMP", "OJMP", "CALL", "RET","PUSH", "POP", "LOADB", "LOADW", "STOREB", "STOREW","LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB","SUBI", "MUL", "DIV", "AND", "OR", "NOR", "NOTB","SAL", "SAR", "EQU", "LT", "LTE", "NOTC"} 
const char *g_ins_name[] = INS_SYM; /*������ָ�����Ƿ�������*/
/*8�����ִ�����ִ��ָ��ĸ�ʽ*/
const char ins_format[33] = "12222133444451667575777778778881";
typedef struct _BIAOHAO_INFO { /*���ڴ洢�����Ϣ������*/
	char name[30];
	int line_num;
	struct _BIAOHAO_INFO *next;
}biaohao;
typedef struct _VAR { /*���ڴ洢������Ϣ������*/
	char name[30];
	int size; /*�洢�����ĸ���*/
	int pianyi; /*ƫ����*/
	short value[256];
	int zijie; /*�����жϱ���������*/
	struct _VAR *next;
}var;
int getinscode(const char *op_sym); /*�����Ƿ���ָ�����*/
unsigned long transtocode(char *instr_line, int instr_num, biaohao *hd1, var *hd2);/*ָ�������*/
int getregnum(char *instr_line, char *reg_name);/*�ɼĴ�������Ӧ������*/
int save_biaohao(char *biaohao_name, int line_num, biaohao *hd);/*�����ŵ���Ϣ��������*/
int save_var(char *a_line, int pianyi, int size, var *hd, int zijie);/*�����ŵ���Ϣ��������*/
long write_var(FILE *pfout, var *hd);/*������������ֵ��Ŀ���ļ���*/


/*��ű����Ϣ������*/
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
/*��ű�����Ϣ������*/
int save_var(char *a_line, int pianyi, int size, var *hd, int zijie)
{
	var *p1;
	short value1;
	char *p = NULL;
	char value_str[20];
	int i, k, fuzhi = 0;
	char varname_temp[20]; /*�����ʱ�ı�������*/
	char string[200]; /*��Ŷ������name��size���ַ���*/
	sscanf_s(a_line, "%*s %s", string);
	sscanf_s(string, "%[^[]", varname_temp); /*�������*/

	for (p1 = hd; p1->next != NULL; p1 = p1->next)
	{
		;
	}
	var *pnew = (var *)malloc(sizeof(var));
	p1->next = pnew;
	pnew->pianyi = pianyi; /*ƫ����*/
	pnew->size = size; /*������ֵ���ٸ�*/
	pnew->zijie = zijie; /*�ֽ�Ϊ1��2�ֱ�Ϊbyte�ͼ�word������*/
	strcpy_s(pnew->name, varname_temp); /*��������*/
	pnew->next = NULL;
	/*���¶Ա����ĸ�ֵ���д���*/
	for (i = 0; i < size; i++)
	{
		pnew->value[i] = 0;
	}
	if (p = strchr(a_line, '='))
	{
		if (size == 1) /*ֻ��һ��ֵ*/
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
					fuzhi = 1; /*��ʾ���ᱻ��ֵ*/
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
					fuzhi = 1; /*��ʾ���ᱻ��ֵ*/
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

/*��ָ�����Ƿ��õ�ָ�������*/
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
/*�ɼĴ������õ��Ĵ������*/
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
/*��ָ����Ŀ����룬 ins_numΪָ�������*/

unsigned long transtocode(char *ins_line, int ins_num, biaohao *hd1, var *hd2)
{
	unsigned long op_code; /*op_codeΪָ�������*/
	unsigned long arg1, arg2, arg3; /*argΪ�Ĵ�����Ӧ�ı��*/
	unsigned long ins_code = 0ul;
	char op_sym[8], reg0[8], reg1[8], reg2[8];/*op_symΪָ�����Ƿ�regΪ�Ĵ�����*/
	unsigned long addr;
	int immed, port, n;
	char string[20];
	biaohao *p1;
	var *p2;
	switch (ins_format[ins_num])
	{
	case '1': /*���� op(5b) + padding(27)*/
	{
		op_code = ins_num;
		ins_code = op_code << 27;
		break;
	}
	case '2': /*����op(5b) + padding(3b) + address(24b)*/
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
		op_code = getinscode(op_sym);/*�����Ƿ��õ�������*/
		ins_code = (op_code << 27) | (addr & 0x00ffffff);
		break;
	}
	case '3': /*����op(5b) + reg0(3b) + padding(24b)*/
	{
		n = sscanf_s(ins_line, "%s %s", op_sym, sizeof(op_sym), reg0, sizeof(reg0));
		if (n < 2)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getinscode(op_sym);
		/*�ɼĴ������õ��Ĵ������*/
		arg1 = getregnum(ins_line, reg0);
		ins_code = (op_code << 27) | (arg1 << 24);
		break;
	}
	case '4': /*����op(5b) + reg0(3b) + address(24b)*/
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
		/*�ɼĴ������õ��Ĵ������*/
		arg1 = getregnum(ins_line, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (addr & 0x00ffffff);
		break;
	}
	case '5': /*���� op(5b)+reg0(3b)+padding(8b)+immediate(16b)*/
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
	case '6': /*����op(5b) + reg0(3b) + padding(16b) + port(8b)*/
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
	case '7':/*����op(5b) + reg0(3b) + reg1(4b) + reg2(4b) + padding(16b)*/
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
	case '8':/*����op(5b)+reg0(3b)+reg1(4b)+reg2(4b)+padding(16b)*/
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
	return ins_code; /*����Ŀ�����*/
}
/*������������ֵ��Ŀ���ļ���*/
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
	//�����ļ�λ��
	const char *filename_in = "C:\\Users\\wonder\\Desktop\\����ʵϰ\\sum.txt";
	////����ļ�λ��
	//const char *filename_out = "C:\\Users\\wonder\\Desktop\\����ʵϰ\\Compile\\sumout.dat";
	/*�����������ͱ���������ͷָ��,��ͷָ��û������*/
	biaohao *hd_biaohao = (biaohao *)malloc(sizeof(biaohao));
	hd_biaohao->next = NULL;
	var *hd_var = (var *)malloc(sizeof(var));
	hd_var->next = NULL;
	char biaohao_temp[20]; /*�����ʱ�ı������*/
	char string[200]; /*��Ŷ������name��size���ַ���*/
	char var_size_str[20]; /*��ű����Ĵ�С(�ַ�����ʽ)*/
	int var_size; /*��ű����Ĵ�С(int�͸�ʽ)*/
	int pianyi_num = 0;
	int n;
	int line_num = 0; /*����뵱ǰ������*/
	//������Դ�����ļ��������ļ�
	fopen_s(&pfin, filename_in, "r");
	if (pfin == NULL) {
		printf("ERROR: cannot open file\n");
		return;
	}
	while (!feof(pfin))
	{
		fgets(a_line, 200, pfin); /*��Դ�ļ�ȡһ������*/
		if ((pcpos = strchr(a_line, '#')) != NULL)
		{
			*pcpos = '\0'; /*ȥ��ע��*/
		}
		n = sscanf_s(a_line, "%s", op_sym, sizeof(op_sym)); /*ȡָ�����Ƿ�*/

		if (n < 1) /*���к�ע���еĴ���*/
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
			if ((pcpos = strchr(string, '[')) != NULL) /*������Ϊ�����ʽ*/
			{
				sscanf_s(string, "%*[^[][%[^]]", var_size_str);/*�������С*/
				var_size = atoi(var_size_str);
			}
			else
			{
				var_size = 1;
			}
			save_var(a_line, pianyi_num, var_size, hd_var, zijie);
			pianyi_num += 2 * var_size; /*һ��word��ֵռ���ֽ�*/
			line_num--;
		}
		else if (strstr(a_line, "BYTE") != NULL)
		{
			sscanf_s(a_line, "%*s %s", string);
			zijie = 1;
			if ((pcpos = strchr(string, '[')) != NULL) /*������Ϊ�����ʽ*/
			{
				sscanf_s(string, "%*[^[][%[^]]", var_size_str);/*�������С*/
				var_size = atoi(var_size_str);
			}
			else
			{
				var_size = 1;
			}
			save_var(a_line, pianyi_num, var_size, hd_var, zijie);
			pianyi_num += var_size; /*һ��byte��ֵռһ���ֽ�*/
			line_num--;
		}
		line_num++;
	}

	fclose(pfin);
	//�ڶ��ζ�Դ�ļ�������32��ָ�����Ƿ� 
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
		else if ((pcpos = strchr(a_line, ':')) != NULL) /*ȥ�����*/
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

		op_num = getinscode(op_sym); /*�����Ƿ��õ�ָ��Ĳ�����*/
		//�Ƿ�ָ��� 
		if (op_num > 31)
		{
			printf("ERROR: %s is a invalid instruction! \n", a_line);
			exit(-1);
		}
		printf("0x%08lx\n", transtocode(a_line, op_num, hd_biaohao, hd_var));
		/*fprintf(pfout, "0x%08lx\n", transtocode(a_line, op_num, hd_biaohao, hd_var));*/

	}
	//write_var(pfout, hd_var); /*��������ֵ��ʮ�����������pfout�ļ���*/
	fclose(pfin);
	/*fclose(pfout);*/
	printf("success!!\n");
	return;
}