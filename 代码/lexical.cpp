#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

/*类别：

	1	procedure,

	2	begin,

	3	const,

	4	var,

	5	end,

	6	odd,	

	7	if,

	8	else,

	9	then,

	10	do,

	11	while,

	12	read,

	13	write,

	14  program,

	15  call，


	16	NUM,	//常量

	17	ID,	//标识符



	//运算符

	18	ADD,	//'+'

	19	SUB,	//'-'

	20	MUL,	//'*'

	21	DIV,	//'/'

	22	EQUAL,	//'='，

	23	LESS,	//'<'

	24	GREAT,	//'>'

	25	LESSEQUAL,	//'<='

	26  GREATEQUAL,//'>='
	
	27  NOTEQUAL,	//'<>不等于

	
	28  ASSIGN,		//':='   赋值

	



	//界符

	29  SEMICOLON //';'

	30  COMMA	//','

	31 	LPAR,	//'('

	32  RPAR,	//')'


	33	.		//结束符

	
*/

fstream datafile;
string line;   //每一行字符串
int len;  //当前字符串长度
int row;    //排
int index;  //索引，某一行的字符序号
char ch;    //当前字符
vector<string>ID;   //标识符表
vector<int>NUM;     //常数表
string word;        //存放单词
int indexID = 0;
int indexNUM = 0;
string keyword[15] = { "procedure","begin","const","var","end","odd","if","else","then","do","while","read","write","program","call" };

int IsKeyWord(string word)
{//判断是否为关键字，是则返回位置，不是则返回0
	int i;
	for (i = 0; i < 15; i++)
	{
		if (word.compare(keyword[i]) == 0)
		{
			return (i+1);
		}
	}
	return 0;
}
void GetChar()
{//读入下一字符，并将搜索指示器前移
	ch = tolower(line[index]); //获得小写字母
	index++;    //指示器前移
}
void GetBC()
{//检查是否为空格或换行
	while ((ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') && (index < len))
	{//防止空白行读取越界
		GetChar();
	}
}
void Retract()
{  //搜索指示器回调一个字符，并将ch置为空字符
	index--;
	ch = ' ';
}
/*bool IsLetter(char ch)
{
	if (ch > 'a' && ch < 'z')
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool IsDigit(char ch)
{
	if (ch > '0' && ch < '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}*/
void GetSYM()
{//词法分析
	
	while (index <= len - 1)
	{
		GetChar();
		GetBC();   //得到非空白或换行字符
		if (index > len)
		{//说明已读完本行，跳出函数
			return;
		}
		if(isalpha(ch))
		{//字母开头，为标识符或关键字
			word = "";
			while (isalpha(ch) || isdigit(ch))
			{//后续若为字母或数字，则一直读入，直至结束
				word = word + ch;   //将ch加到word尾部
				GetChar();
			}
			Retract();   //搜索指示器，回调一个字符
			int n = IsKeyWord(word);    //获得其返回值，位置或0

			if (n > 0)  //说明该单词是关键字
			{
				datafile << word << "\t" << "关键字" << "\t" << n << endl;
			}
			else    //说明该单词为标识符
			{
				vector <string>::iterator iter_str;
				iter_str = find(ID.begin(), ID.end(), word);
				if (iter_str != ID.end())
				{//说明找到了该元素
					datafile << word << "\t" << "标识符" << "\t" << distance(ID.begin(), iter_str) << endl;
				}
				else
				{
					datafile << word << "\t" << "标识符" << "\t" << indexID<< endl;
					indexID++;
					ID.push_back(word);
					//将一个新的元素加到vector的最后面，位置为当前元素的下一个元素
				}
			}
		}
		else if (isdigit(ch))  //开头为数字
		{
			word = "";
			while (isdigit(ch))
			{
				word = word + ch;
				GetChar();
			}
			if (isalpha(ch))
			{//数字后跟字母
				datafile << "error:标识符不规范" << "    第" << row << "行第" << index << "列" << endl;
				while (isdigit(ch) || isalpha(ch))
				{
					index++;
					GetChar();
				}
			}
			else
			{
				Retract();  //回调一个字符
				int num = atoi(word.c_str());
				//c_str()函数返回一个指向正规C字符串的指针常量 
				//为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
				vector <int>::iterator iter_digit;
				iter_digit = std::find(NUM.begin(), NUM.end(), num);//查常数表
				if (iter_digit == NUM.end())  //表中未找到此常量
				{
					datafile << word << "\t" << "常量" << "\t" << indexNUM << endl;
					indexNUM++;
					NUM.push_back(num);
				}
				else
				{
					datafile << word << "\t" << "常量" << "\t" << distance(NUM.begin(), iter_digit) << endl;
				}
			}
		}
		else if(ch == '+')
		{
			
			datafile << "+" << "\t" << "18" << "\t"<< "~" << endl;
		}
		else if (ch == '-')
		{
			datafile << "-" << "\t" << "19" << "\t" << "~" << endl;
		}
		else if (ch == '*')
		{
			datafile << "*" << "\t" << "20" << "\t" << "~" << endl;
		}
		else if (ch == '/')
		{
			datafile << "/" << "\t" << "21" << "\t" << "~" << endl;
		}
		else if (ch == '=')
		{
			datafile << "=" << "\t" << "22" << "\t" << "~" << endl;
		}
		else if (ch == '<')
		{
			if (line[index] == '=')  //idex始终指向下一个待读字符
			{
				datafile << "<=" << "\t" << "25" << "\t" << "~" << endl;
				index++;
			}
			else if (line[index] == '>')
			{
				datafile << "<>" << "\t" << "27" << "\t" << "~" << endl;
				index++;
			}
			else
			{
				datafile << "<" << "\t" << "23" << "\t" << "~" << endl;
			}
		}
		else if (ch == '>')
		{
			if (line[index] == '=')
			{
				datafile << ">=" << "\t" << "26" << "\t" << "~" << endl;
				index++;
			}
			else
			{
				datafile << ">" << "\t" << "24" << "\t" << "~" << endl;
			}
		}
		else if (ch == ':')
		{
			if (line[index] == '=')
			{
				datafile << ":=" << "\t" << "27" << "\t" << "~" << endl;
				index++;
			}
			else
			{
				datafile << "error:赋值号错误，应为 :=" << "    第" << row << "行第" << index << "列" << endl;
			}
		}
		else if (ch == ';')
		{
			datafile << ";" << "\t" << "29" << "\t" << "~" << endl;
		}
		else if (ch == ',')
		{
			datafile << "," << "\t" << "30" << "\t" << "~" << endl;
		}
		else if (ch == '(')
		{
			datafile << "(" << "\t" << "31" << "\t" << "~" << endl;
		}
		else if (ch == ')')
		{
			datafile << ")" << "\t" << "32" << "\t" << "~" << endl;
		}
		else if (ch == '.')
		{
			datafile << "." << "\t" << "33" << "\t" << "~" << endl;
			datafile << "END" << endl;
			return ;
		}
		else
		{
			datafile << "error:非法字符！" << "    第" << row << "行第" << index << "列" << endl;
		}
	}
}
int main()
{
	fstream read_file;
	char filename[100];   //文件名
	cout << "请输入要处理的文件名：";
	cin >> filename;
	read_file.open(filename, ios::in);
	if (read_file.fail())
	{
		cout << "打开输入文件失败\n";
		return 0;
	}
	datafile.open("C:\\Users\\a\\Desktop\\lex_out.txt", ios::out);
	if (datafile.fail())
	{
		cout << "输出文件打开失败\n";
		return 0;
	}
	row = 1;//从第一行开始
	while (getline(read_file, line))
	{
		len = line.length();  //当前行字符串长度
		index = 0;
		GetSYM();
		row++;
	}
	cout << "文件处理完毕" << endl;
	read_file.close();
	datafile.close();
	return 0;
}