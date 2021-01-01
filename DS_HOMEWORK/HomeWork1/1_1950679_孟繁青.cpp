#include<iostream>
#include <string>
#include <sstream>
#include<vector>
//#include"List.h"
using namespace std;
/*本项目的实质是完成对考生信息的建立，查找，插入，修改，删除等功能。
其中考生信息包括准考证号，姓名，性别，年龄和报考类别等信息。项目在
设计时应首先确定系统的数据结构，定义类的成员变量和成员函数；然后实
现各成员函数以完成对数据操作的相应功能；最后完成主函数以验证各个成
员函数的功能并得到运行结果。（建议采用链表实现）*/

struct Item {
	//由于不知道学号等到底多少位，所以用string来存储
	string age;      //学生年龄
	string num;      //学生考号
	string name;     //学生姓名
	string sex;      //学生性别
	string aim;      //学生报考类别
	void Copy(Item& D) { age = D.age, num = D.num, name = D.name, sex = D.sex, aim = D.aim; }
	void Show() { cout << num << ' ' << name << ' ' << sex << ' ' << age << ' ' << aim; }
	Item():age("0"),num("0"),name("0"),sex("0"),aim("0"){}
	Item(string _age, string _num, string _name, string _sex, string _aim) :age(_age), num(_num), name(_name), sex(_sex), aim(_aim) {}
	Item(Item& D):age(D.age),num(D.num),name(D.name),sex(D.sex),aim(D.aim){}
	Item(string str){
		vector<string> res;
		string result;
		stringstream input(str);
		while(input>>result)
		{
			res.push_back(result);
		}
		this->age=res[0];
		this->num=res[1];
		this->name=res[2];
		this->sex=res[3];
		this->aim=res[4];
	}
	//为了实现一开始列表的建立，按行读取，所以需要建立通过string来初始化的操作
};
struct ListNode {
	Item data;
	ListNode* link;
	ListNode():data("0","0","0","0","0"),link(NULL){}
	ListNode(Item& D):data(D),link(NULL){}
};
//为了插入与删除的方便，我们的列表是带表头的
class List {
private:
	ListNode* head;             //指向头结点
	int length;                 //表长
public:
	//构造函数
	List() { head = new ListNode; length = 0; }         //head指向头结点
	List(const List& L);
	//析构函数
	~List() { Destory(); }
	//只读访问接口
	bool Empty() const;                    //检测是否为空表
	bool Search(string num,Item& X) const;         //按考号查找,若存在将考生信息存于X内
	bool CheckInput(Item& L)const;      //检测是否与之前出现相同的学号
	int Length() const;                   //统计人数
	void Show()const;                       //按一定格式输出
	//可写访问接口
	void Choose();                   //选择用什么操作，相当于一个大菜单
	void Build();                        //建立链表，提示输入信息，尾插法,并输入节点数目
	void MakeEmpty();            //置空
	void Destory();                   //销毁该表
	void GetData(int i, Item& X);                          //取第i个Item，并存在X里面
	void SetData(int i, Item& X);                          //修改第i个节点的值
	bool Remove(int i, Item& X);                         //删除第i个节点，并且把信息存于Item X中
	bool Insert(int i, Item& X);                             //插入数据与第i个节点后面，需要注意我们认为表头节点是第0个
	bool Push(Item& X);                                       //默认插入末尾
	bool Pop(Item X);                                         //默认移除末尾
};

void List::Choose()                                   //大菜单函数
{
	int i;
	int pos;	
	Item X;			
	string age,  num, name, sex, aim;		
	while (1)
	{
		cout << "请输入您要进行的操作(1为插入，2为删除，3为查找，4为修改，5为统计，6为展示,7为建立，0为退出操作" << endl;
		cin >> i;
		if (i == 0)
		{
			cout<<"欢迎再次使用考试管理系统～"<<endl;
			break;
		}
		switch (i) {
		case 1:
			cout << "请输入您要插入的位置"<<endl;
			cin >> pos;
			while(pos<0||pos>length)
			{
				cout<<"插入位置错误，请重新输入"<<endl;
				cin>>pos;
			}
			if (pos >= 0 && pos <= length)
			{
				cout << "请输入您要插入的数据"<<endl;
                cout<<"请分别输入年龄，考号，姓名，性别，考取目标"<<endl;
				cin >> age >> num >> name >> sex >> aim;
				Item L(age, num, name, sex, aim);
				Insert(pos, L);
			}
			else
				cerr << "无效下标"<<endl;
			break;
		case 2:
			cout << "请输入您要删除的位置"<<endl;
			cin >> pos;
			while(pos<1||pos>length)
			{
				cout<<"删除位置错误，请重新输入"<<endl;
				cin>>pos;
			}
			Remove(pos, X);
			break;
		case 3:

			cout << "请输入您要查找的学号" << endl;
			cin >> num;
			if (Search(num, X))
			{
				cout << "存在该考生" << endl;
				X.Show();
			}
			else
				cout << "没有该考生" << endl;
			break;
		case 4:
			cout << "请输入您想修改的位置";
			cin >> pos;
			while(pos<1||pos>length)
			{
				cout<<"该位置不合理,请重新输入"<<endl;
				cin>>pos;
			}
			if (pos >= 1 && pos <= length)
			{
				cout << "请输入您要插入的数据" << endl;
				string  age,num, name, sex, aim;
				cin >> age >> num >> name >> sex >> aim;
				Item L(age, num, name, sex, aim);
				SetData(pos, L);
			}
			else
				cerr << "下标错误" << endl;
			break;
		case 5:
			cout << "总人数为" << Length() << endl;
			break;
		case 6:
			Show();
			break;
		case 7:
			this->Build();
			break;
		default:
			cerr<<"抱歉，不存在该操作"<<endl;
			break;
		}
		
	}
}

bool List::CheckInput(Item& L)const
{
	if(Search(L.num,L))
	{
		cerr << "已经存在该考号" << endl;
		return false;
	}
	return true;
}

void List::Show()const
{
	if (head->link == NULL)
		cerr << "NULL";
	cout<<"该链表有"<<this->Length()<<"个数据"<<endl;
	ListNode* p = head;
	while (p->link != NULL)
	{
		p->link->data.Show();
		cout << endl;
		p = p->link;
	}
}

bool List::Remove(int i, Item& X)
{
	//还要检查i的有效性
	if (i >= 1 && i <= length)
	{
		ListNode* p = head;
		int num = 1;
		while (num < i)
		{
			p = p->link;
			num++;
		}
		X.Copy(p->link->data);
		length--;
		ListNode* q = p->link;
		p->link = q->link;
		delete q;
		return true;
	}
	cerr << "下标错误";
	return false;
}

void List::SetData(int i, Item& X)        //修改第i个节点的值，i从1开始
{
	if (i >= 1 && i <= length)
	{
		ListNode* p = head;
		int num = 0;
		while (num < i)
		{
			p = p->link;
			num++;
		}
		p->data.Copy(X);
	}
	else
       	cerr << "下标错误"<<endl;
}

void List::MakeEmpty()
{
	if (Empty())
		return;
	ListNode* p = head->link;
	while (p != NULL)
	{
		ListNode* q = p;
		p = p->link;
		delete q;
	}
	length = 0;
}

void List::Destory()
{
	MakeEmpty();
	delete head;
}

bool List::Search(string num,Item& X)const
{
	if (Empty())
		return false;
	ListNode* p = head->link;
	while (p != NULL)
	{
		if (p->data.num == num)           //string库重载了==
			break;
		p = p->link;
	}
	if (p != NULL)
	{
		X.Copy(p->data);
		return true;
	}
	else
		return false;
}
	
void List::Build()
{
	int loop = 1;
	int n;
	cout << "首先请建立考生信息系统！" << endl;
	while (loop) {
		cout << "请输入考生人数：";
		cin >> n;
		if (n>0) {
			loop = 0;
		}
		else
			cout << "人数只能是正整数" << endl << endl;
	}
	cout << "请依次输入考生的年龄，考号， 姓名， 性别，及报考类别！" << endl;
	string str;
	getchar();             //吸收回车
	for(int i = 0; i < n; i++)
	{
		getline(cin, str);
		Item candi(str);
		this->Push(candi);
	}
	
}
bool List::Empty()const
{
	return head->link == 0;
}

int List::Length()const
{
	return length;
}


bool List::Insert(int i, Item& X)
{
	if (i >= 0 && i <= length)
	{
		if (CheckInput(X)) {
			int num = 0;
			ListNode* newnode = new ListNode(X);
			ListNode* p = head;
			while (num < i)
			{
				p = p->link;
				num++;
			}
			newnode->link = p->link;
			p->link = newnode;
			length++;
			return true;
		}
		else
			cerr << "该考号已存在，请重新输入" << endl;
	}
	cerr << "错误下标";
	return false;
}

bool List::Push(Item& X)
{
	ListNode* p = new ListNode(X);
	ListNode* q = head;
	while (q->link != NULL)
		q = q->link;
	p->link = q->link;
	q->link = p;
	length++;
	return true;
}

int main()
{
	cout<<"欢迎使用考试管理系统～"<<endl;
	List S;
	S.Choose();
}