#include<iostream>
#include<string>
#include<stdio.h>
#include<math.h>    
using namespace std;
//定义运算符的优先次序
//把单目的+,-运算符，看为与0做运算的双目运算符，但是我们扫描出来都是一样的，该怎么让计算机理解我们呢

//在运算数栈加个0为表头即可
#define N 100
//表达式的最大长度
#define N_ORTP 11
//加，减，乘，除，取余，乘方，左括号，右括号，等于号
//横为当前运算符
//竖为栈顶运算符
//小于则意味着栈顶运算符优先级更低，计算延迟，当前运算符入栈
//typedef enum{ADD,SUB,MUL,DIV,REM,POW,L_P,R_P,EQL}Operator;
//map<char, int> Oper={ {'+',0},{'-',1},{'*',2},{'/',3},{'%',4},{'^',5},{'@',6},{'#',7}, {'(',8},{')',9},{'=',10} };

char Prior[N_ORTP][N_ORTP] = { '>','>','<','<','<','<','<','<','<','>','>',
							   '>','>','<','<','<','<','<','<','<','>','>',
							   '>','>','>','>','>','<','<','<','<','>','>',
							   '>','>','>','>','>','<','<','<','<','>','>',
							   '>','>','>','>','>','<','<','<','<','>','>',
							   '>','>','>','>','>','>','<','<','<','>','>',
	                           '>','>','>','>','>','>',' ',' ','<','>','>',
							   '>','>','>','>','>','>',' ',' ','<','>','>',
							   '<','<','<','<','<','<','<','<','<','=',' ',
							   ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							   '<','<','<','<','<','<','<','<','<',' ','='
};

#define DEFAULT_SIZE 1024


template<typename T>
class stack
{
    public:
    stack()
    {
        stack_pointer=0;
        this->stack_mfq=new T[DEFAULT_SIZE];
    }
    //默认构造函数
    bool empty();
    //检验是否为空
    T push(T new_element);
    //压栈
    T top();
    //返回栈顶元素
    T pop();
    //退栈
    int size();
    //返回栈的大小
    void print();
    //测试函数，输出栈内容
    void make_empty();
    //置空
    private:
    int stack_pointer;
    //指向栈顶的下一位置
    T* stack_mfq;
    //存储顺序栈
};

template<typename T>
T stack<T>::pop()
{
    T temp;
    if (stack_pointer == 0)
    {
        cout << "空栈，表达式输入错误" << endl;
		exit(1);
    }
    else
    {
        temp = stack_mfq[--stack_pointer];
    }
    return temp;
}

template<typename T>
T stack<T>::push(T new_element)
{
    if (stack_pointer >= DEFAULT_SIZE)
    {
        cout << "栈满" << endl;
    }
    else
    {
        stack_mfq[stack_pointer++] = new_element;
    }
    return new_element;
}

template<typename T>
T stack<T>::top()
{
    T temp;
    if (stack_pointer == 0)
    {
        cout << "空栈,表达式输入错误" << endl;
		exit(1);
    }
    else
    {
        temp = stack_mfq[stack_pointer - 1];
    }
    return temp;
}
template<typename T>
bool stack<T>::empty()
{
    return this->stack_pointer==0;
}
template<typename T>
int stack<T>::size()
{
    return stack_pointer;
}
template<typename T>
void stack<T>::print()
{
    for (int i = 0; i < stack_pointer; i++)
    {
        cout << stack_mfq[i] << " " ;
    }
    cout << endl;
}
template<typename T>
void stack<T>::make_empty()
{
    stack_pointer = 0;
}

void trim(string &s)
//去除我们表达式中的空格。
{
     int index = 0;
     if( !s.empty())
     {
         while( (index = s.find(' ',index)) != string::npos)
         {
             s.erase(index,1);
         }
     }
}

int Convert_to_subs(char s)
{
	switch(s)
	{
		case '+':
		return 0;
		case '-':
		return 1;
		case '*':
		return 2;
		case '/':
		return 3;
		case '%':
		return 4;
		case '^':
		return 5;
		case '@':
		return 6;
		case '#':
		return 7;
		case '(':
		return 8;
		case ')':
		return 9;
		case '=':
		return 10;
	}
	return -1;
}
char OrderBetween(char s1, char s2)    //正确
{
	return Prior[Convert_to_subs(s1)][Convert_to_subs(s2)];
}
int Cal(int opnd1, int opnd2, char op)        //正确
//opnd1为当前栈顶元素，opnd2为pos位置的元素
{
	switch (op) {
	case '+':
		return opnd2 + opnd1;
		break;
	case '-':
		return opnd2 - opnd1;
		break;
	case '*':
		return opnd2 * opnd1;
		break;
	case '/':
		if (opnd1 == 0)
		{
			cerr << "error" << endl;
			return 0;
		}
		else
		return opnd2 / opnd1;
		break;
	case '%':
		if (opnd1 == 0)
		{
			cerr << "error" << endl;
			return 0;
		}
		else
		return opnd2 % opnd1;
		break;
	case '^':
		return int(pow(double(opnd2), opnd1));
		break;
	default:
		cerr << "error" << endl;
		return 0;
		break;
	}
}
int Cal(int opnd1, char op)    //正确
{
	switch (op) {
	case '@':
		return opnd1;
		break;
	case '#':
		return 0 - opnd1;
		break;
	default:
		cerr << "error" << endl;
		return 0;
		break;
	}
}
bool Isdigit(char s)
{
	return s>='0'&&s<='9';
}
int Solution(string s)
{
    int pos = 0;
	int Popnd1,Popnd2;
	char op;
	stack<int>opnd;    //操作数
	stack<char>optr;    //操作符
	optr.push('=');
	while (!optr.empty())
	{
		if (Isdigit(s[pos])) {
			//opnd.push(int(s[pos]-'0'));
			//pos++;
			int temp=int(s[pos]-'0');
			pos++;
			while(Isdigit(s[pos]))
			{
				temp=temp*10+int(s[pos]-'0');
				pos++;
			}
			opnd.push(temp);
		}
		else
		{
			switch (OrderBetween(optr.top(),s[pos])) {
			case '<':
				//结合下面的单双目的+,-判断，我们能不能再压栈时就给他一个标志
				//或者说，我们读取到+,-运算符时，如果是单目，我们压一个不一样的运算符进去
				//但是注意到我们读取的还是正常的+,-运算符
				if (s[pos] == '+' || s[pos] == '-')
				{
					if (pos == 0)
					{
						//单目
						if (s[pos] == '+')
							optr.push('@');
						else
							optr.push('#');
						pos++;
					}
					else
					{
						if (s[pos - 1] == ')' || Isdigit(s[pos - 1]))
						{
							optr.push(s[pos]);
							pos++;
							//双目
						}
						else
						{
							if (s[pos] == '+')
								optr.push('@');
							else
								optr.push('#');
							pos++;
							//单目
						}
					}
				}
				else {
					optr.push(s[pos]);
					pos++;
				}
				break;
			case '=':
				optr.pop();
				pos++;
				break;
			case '>':                 //栈顶运算符参与运算
				op = optr.top();
				optr.pop();
				if (op == '@' || op == '#')
				{
					Popnd1 = opnd.top();
					opnd.pop();
					opnd.push(Cal(Popnd1, op));
				}
				else
				{
					Popnd1 = opnd.top();
					opnd.pop();
					Popnd2 = opnd.top();
					opnd.pop();
					opnd.push(Cal(Popnd1, Popnd2, op));
				}
				break;
			default:
				cerr << "错误表达式" << endl;
				exit(1);
			}
		}
	}

	return opnd.top();
}

int main()
{
	string s;
	cout<<"请输入表达式,以等号结束，等号之后输入的不起作用"<<endl;
	getline(cin,s);
	//注意这里不能用cin，因为输入空格就认为输入结束了。
	trim(s);
	cout<<Solution(s);
	//cout<<s;
	system("pause");
}

	