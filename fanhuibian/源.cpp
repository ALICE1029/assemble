// 实现一个反汇编器
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include<fstream>
using namespace std;
int my_count = 0;
enum BinIns;
int biaohaoshu = 0;
// 二进制指令结构体
// 指令码+操作数
struct Instruction
{
	BinIns op;  // 指令码只占一个字节
	int    arg; // 操作数，占四个字节
};
const char instructFormat[33] = "12222133444451667575777778778881";
int biaohao[100];

// 枚举类型的二进制指令集
enum BinIns
{
	binHalt, binJump, 
	binCjmp,binOjmp,binCall,
	binRet, binPush,binPop,
	binLoadb,binLoadw,binStoreb,binStorew,
	binLoadi, binNop, binIn, binOut, binAdd, binAddi, binSub, binSubi,
	binMul, binDiv,binAnd,binOr,biNor, binNotb,binSal,binSar,
	binEqu,binLt, binLte, binNotc
};
// 枚举类型说明：
// enum后面定义的是枚举类型名
// 花括号内部是该枚举类型可以取的值

// 初始化汇编指令集
void InitAssembleInstructions(vector<string>& assIns)
{
	assIns.clear();

	assIns.push_back("HALT");
	assIns.push_back("JUMP");
	assIns.push_back("CJMP");
	assIns.push_back("OJMP");
	assIns.push_back("CALL");
	assIns.push_back("RET");
	assIns.push_back("PUSH");
	assIns.push_back("POP");
	assIns.push_back("LOADB");
	assIns.push_back("LOADW");
	assIns.push_back("STOREB");
	assIns.push_back("STOREW");
	assIns.push_back("LOADI");
	assIns.push_back("NOP");
	assIns.push_back("IN");
	assIns.push_back("OUT");
	assIns.push_back("ADD");
	assIns.push_back("ADDI");
	assIns.push_back("SUB");
	assIns.push_back("SUBI");

	assIns.push_back("MUL");
	assIns.push_back("DIV");
	assIns.push_back("AND");

	assIns.push_back("OR");
	assIns.push_back("NOR");
	assIns.push_back("NOTB");

	assIns.push_back("SAL");
	assIns.push_back("SAR");
	assIns.push_back("EQU");
	assIns.push_back("LT");
	assIns.push_back("LTE");
	assIns.push_back("NOTC");
}


// 建立二进制指令到汇编指令的映射
// 初始化
void InitBinaryToAssemble(const vector<string>& assIns, map<BinIns, string>& binToIns)
{
	binToIns.clear();
	for (auto i = 0; i != assIns.size(); ++i)
	{
		// assIns和BinIns的指令次序一致
		binToIns[static_cast<BinIns>(i)] = assIns[i];
	}
}
void read_file(vector<string>&bin) {
	ifstream in;
	in.open("C:\\Users\\DELL\\Desktop\\input.txt", ios::in);
	if (!in.is_open()) {
		cout << "文件打开失败" << endl;
		return;
	}
	string buf;
	while (getline(in, buf)) {
		my_count++;
		bin.push_back(buf);
	}
	in.close();
}
// 读入二进制指令
void ReadBinary(vector<string>& bin)
{
	bin.clear();
	string line;
	while (getline(cin, line))
	{
		bin.push_back(line);
	}
}

// 显示二进制和汇编指令
void Display(const vector<string>& bar)
{
	for (auto i = 0; i != bar.size(); ++i)
	{
		cout << bar[i] << endl;
	}
}
void write_file(const vector<string>& bar) {
	ofstream out;
	out.open("C:\\Users\\DELL\\Desktop\\output.txt", ios::out);
	for (auto i = 0; i != bar.size(); ++i)
	{
		out << bar[i] << endl;
	}
	out.close();
}
int todec(string a) {
	int count = 0;
	for (int i = 0; i < a.size(); i++) {
		count += int(a[i]-'0')*pow(2, (a.size()-i-1));
	}
	return count;
}
void MyBinaryToAssemble(const vector<string>& bin, Instruction *&ins,vector<string>& ass, const map<BinIns, string>& binToIns)
{
	ass.clear();
	string binLine;
	for (auto i = 0; i != bin.size(); ++i)
	{
		binLine += bin[i] + '\t';
	}

	//cout << binLine << endl;

	istringstream sin(binLine);//从binline中读取字符
	string strOp, strArg;
	string op;
	string arg;
	string assIns;
	BinIns opBin;
	string a, b;
	int i = 0;

	while (sin >> strOp)//strop接受读入的字符
	{
		assIns.clear();
		ins[i].op = static_cast<BinIns>(todec(strOp.substr(0, 5)));		
		//opBin = static_cast<BinIns>(atoi(strOp.c_str()));//string转char*转int
		auto cit = binToIns.find(ins[i].op);
		if (cit == binToIns.end())
		{
			// 非法二进制指令
			// 忽略处理
			;
			break;
		}
		op = cit->second;
	//	int t = instructFormat[ins[i].op];
		if (instructFormat[ins[i].op]-'0' ==1) {
			//break;
			assIns = op ;
		}

		if (instructFormat[ins[i].op] - '0' ==2) {
			
			ins[i].arg = todec(strOp.substr(8, 24));
			int argNum = ins[i].arg;
			arg = to_string(ins[i].arg);
			if (op == "CJMP") {
				
				biaohao[biaohaoshu] = argNum;
				if (argNum == 36)
				{
					
					assIns = op + '\t' + "loop1";
					
				}
				if (argNum == 20)
				{
					
					assIns = op + '\t' + "again1";
					
				}
				if (argNum == 68)
				{
				
					assIns = op + '\t' + "loop2";
				
				}
				ass.push_back(assIns);
				i++;
				biaohaoshu++;
				continue;
			}
			assIns = op + '\t' + arg;
		}
		if (instructFormat[ins[i].op ] - '0' ==3) {
			int reg= todec(strOp.substr(5,3));
			string sreg;
			if (reg == 0)sreg = "Z";
			if (reg == 1)sreg = "A";
			if (reg == 2)sreg = "B";
			if (reg == 3)sreg = "C";
			if (reg == 4)sreg = "D";
			if (reg == 5)sreg = "E";
			if (reg == 6)sreg = "F";
			if (reg == 7)sreg = "G";
			assIns = op + '\t' + sreg;
		}
		if (instructFormat[ins[i].op] - '0' ==4) {
			int reg = todec(strOp.substr(5, 3));
			string sreg;
			if (reg == 0)sreg = "Z";
			if (reg == 1)sreg = "A";
			if (reg == 2)sreg = "B";
			if (reg == 3)sreg = "C";
			if (reg == 4)sreg = "D";
			if (reg == 5)sreg = "E";
			if (reg == 6)sreg = "F";
			if (reg == 7)sreg = "G";
			int add= todec(strOp.substr(8, 24));
			string sadd = to_string(add);
			assIns = op + '\t' + sreg+ '\t' + sadd;
		}
		if (instructFormat[ins[i].op] - '0' == 5) {
			int reg = todec(strOp.substr(5, 3));
			string sreg;
			if (reg == 0)sreg = "Z";
			if (reg == 1)sreg = "A";
			if (reg == 2)sreg = "B";
			if (reg == 3)sreg = "C";
			if (reg == 4)sreg = "D";
			if (reg == 5)sreg = "E";
			if (reg == 6)sreg = "F";
			if (reg == 7)sreg = "G";
			int imm= todec(strOp.substr(16, 16));
			string simm = to_string(imm);
			assIns = op + '\t' + sreg+ '\t' + simm;

		}
		if (instructFormat[ins[i].op] - '0' == 6) {
			int reg = todec(strOp.substr(5, 3));
			string sreg;
			if (reg == 0)sreg = "Z";
			if (reg == 1)sreg = "A";
			if (reg == 2)sreg = "B";
			if (reg == 3)sreg = "C";
			if (reg == 4)sreg = "D";
			if (reg == 5)sreg = "E";
			if (reg == 6)sreg = "F";
			if (reg == 7)sreg = "G";
			int port = todec(strOp.substr(24, 8));
			string sport = to_string(port);
			assIns = op + '\t' + sreg + '\t' + sport;
		}
		if (instructFormat[ins[i].op] - '0' == 7) {
			int reg = todec(strOp.substr(5, 3));
			string sreg;
			if (reg == 0)sreg = "Z";
			if (reg == 1)sreg = "A";
			if (reg == 2)sreg = "B";
			if (reg == 3)sreg = "C";
			if (reg == 4)sreg = "D";
			if (reg == 5)sreg = "E";
			if (reg == 6)sreg = "F";
			if (reg == 7)sreg = "G";
			int reg2 = todec(strOp.substr(8, 4));
			string sreg2;
			if (reg2 == 0)sreg2 = "Z";
			if (reg2 == 1)sreg2 = "A";
			if (reg2 == 2)sreg2 = "B";
			if (reg2 == 3)sreg2 = "C";
			if (reg2 == 4)sreg2 = "D";
			if (reg2 == 5)sreg2 = "E";
			if (reg2 == 6)sreg2 = "F";
			if (reg2 == 7)sreg2 = "G";
			int reg3 = todec(strOp.substr(12, 4));
			string sreg3;
			if (reg3 == 0)sreg3 = "Z";
			if (reg3 == 1)sreg3 = "A";
			if (reg3 == 2)sreg3 = "B";
			if (reg3 == 3)sreg3 = "C";
			if (reg3 == 4)sreg3 = "D";
			if (reg3 == 5)sreg3 = "E";
			if (reg3 == 6)sreg3 = "F";
			if (reg3 == 7)sreg3 = "G";
			assIns = op + '\t' + sreg + '\t'+sreg2 + '\t'+sreg3;
		}
		if (instructFormat[ins[i].op] - '0' == 8) {
			int reg = todec(strOp.substr(5, 3));
			string sreg;
			if (reg == 0)sreg = "Z";
			if (reg == 1)sreg = "A";
			if (reg == 2)sreg = "B";
			if (reg == 3)sreg = "C";
			if (reg == 4)sreg = "D";
			if (reg == 5)sreg = "E";
			if (reg == 6)sreg = "F";
			if (reg == 7)sreg = "G";
			int reg2 = todec(strOp.substr(8, 4));
			string sreg2;
			if (reg2 == 0)sreg2 = "Z";
			if (reg2 == 1)sreg2 = "A";
			if (reg2 == 2)sreg2 = "B";
			if (reg2 == 3)sreg2 = "C";
			if (reg2 == 4)sreg2 = "D";
			if (reg2 == 5)sreg2 = "E";
			if (reg2 == 6)sreg2 = "F";
			if (reg2 == 7)sreg2 = "G";
			assIns = op + '\t' + sreg + '\t' + sreg2 ;
		}
		ass.push_back(assIns);
		i++;
	}
}
// 将读入的二进制指令转换为汇编指令
void BinaryToAssemble(const vector<string>& bin,vector<string>& ass,const map<BinIns, string>& binToIns,map<BinIns, int>& insArgNum)
{
	ass.clear();
	string binLine;
	for (auto i = 0; i != bin.size(); ++i)
	{
		binLine += bin[i] + '\t';
	}

	//cout << binLine << endl;

	istringstream sin(binLine);//从binline中读取字符
	string strOp, strArg;
	string op;
	string arg;
	string assIns;
	BinIns opBin;
	while (sin >> strOp)//strop接受读入的字符
	{
		opBin = static_cast<BinIns>(atoi(strOp.c_str()));//string转char*转int
		auto cit = binToIns.find(opBin);
		if (cit == binToIns.end())
		{
			// 非法二进制指令
			// 忽略处理
			;
			break;
		}
		op = cit->second;
		int argNum = insArgNum[cit->first];
		if (argNum > 0)
		{
			sin >> strArg;
			arg = strArg;
		}
		else
		{
			arg = "";
		}
		assIns = op + '\t' + arg;
		ass.push_back(assIns);
	}
}

// 十六进制字符串为十进制字符串
string StringToNum(const string& str)
{
	string ret;
	int num = 0;
	for (auto i = 0; i != str.size(); ++i)
	{
		num = num * 2 + str[i] - '0';
	}
	char tmp[101];
	_itoa_s(num, tmp, 10);
	ret = tmp;
	return ret;
}

// 二进制指令转换为十进制指令
// 针对输入的二进制指令为二进制编码形式的情况
void BinaryToDec(vector<string>& bin)
{
	for (auto i = 0; i != bin.size(); ++i)
	{
		istringstream sin(bin[i]);
		string tmp, ins;
		while (sin >> tmp)
		{
			ins += StringToNum(tmp) + '\t';
		}
		bin[i] = ins;
	}
}

string ox[16] = { "0000","0001","0010","0011" ,
			"0100" ,"0101" ,"0110" ,"0111" ,
			"1000" ,"1001","1010","1011",
			"1100","1101","1110","1111" };
string xtoBinary_single(string &bin) {
	string res="";
	//res.resize(bin.size() * 4);
	for (int i = 0; i < bin.size(); i++) {
		if (bin[i] >= 'a'&&bin[i] <= 'f') {
			res += ox[bin[i] - 87];
		}
		else {
			res += ox[bin[i] - '0'];
		}
	}
	return res;
}
vector<string> xtoBinary(vector<string>& bin) {
	vector<string>res;
	res.resize(bin.size());
	for (int i = 0; i < bin.size(); i++) {
		bin[i] = bin[i].substr(2, 8);
		res[i] = xtoBinary_single(bin[i]);

	}
	return res;
}
void addbiaoshifu(vector<string>&ass) {
	for (int i = 0; i < ass.size(); i++) {
		for (int j = 0; j < biaohaoshu; j++) {
			if (biaohao[j] / 4 == i) {
				if (biaohao[j] == 36)
				{
					ass[i].insert(0, "loop1:  ");
					break;
				}
				if (biaohao[j] == 68)

					ass[i].insert(0, "loop2:  ");
				if (biaohao[j] == 20)

					ass[i].insert(0, "again1:  ");

			}
		}

	}
}
vector<string> getResultOfDeCompile( vector<string> bin) {
	// 汇编指令集
	vector<string> assIns;
	InitAssembleInstructions(assIns);


	// 汇编指令到二进制的映射
	map<BinIns, string> binToAss;
	InitBinaryToAssemble(assIns, binToAss);

	//vector<string> bin; // 保存读入的二进制指令
	//read_file(bin);
	cout << endl;
	Display(bin);

	cout << endl;
	bin = xtoBinary(bin);


	vector<string> ass; // 保存转换后的汇编指令
	Instruction* ins = (Instruction*)malloc(sizeof(Instruction)*my_count);
	MyBinaryToAssemble(bin, ins, ass, binToAss);
	addbiaoshifu(ass);
	Display(ass);
	write_file(ass);
	return ass;

}
//int main()
//{
//	// 汇编指令集
//	vector<string> assIns;
//	InitAssembleInstructions(assIns);
//
//
//	// 汇编指令到二进制的映射
//	map<BinIns, string> binToAss;
//	InitBinaryToAssemble(assIns, binToAss);
//
//	vector<string> bin; // 保存读入的二进制指令
//	read_file(bin);
//	cout << endl;
//	Display(bin);
//
//	cout << endl;
//	bin = xtoBinary(bin);
//
//
//	vector<string> ass; // 保存转换后的汇编指令
//	Instruction* ins = (Instruction*)malloc(sizeof(Instruction)*my_count);
//	MyBinaryToAssemble(bin, ins, ass, binToAss);
//	addbiaoshifu(ass);
//	Display(ass);
//	write_file(ass);
//
//	cout << endl;
//
//	system("pause");
//	return 0;
//}