//两个有序且非降序链表的交集
//如果是无序的显然复杂度为O（mn）
//但是注意到有序这个条件，我们考虑在第二个链表中查找第一个链表中的数，但是需要注意
//当我们找到第一个数时，记下其在第二个链表中的位置，再从此开始查找第二个数
#include<iostream>
using namespace std;
template<typename T>struct ListNode {
	T data;
	ListNode* link;
	ListNode() :data(-1), link(NULL){}               //默认构造尾节点
	ListNode(T D):data(D),link(NULL){}
};
template<typename T>class List {
private:
	ListNode<T>* head;         //head指向表头节点，认为是第0个
    int length;
public:
	//构造函数
	List() { head = new ListNode<T>; length=0;}
	List(const List<T>& L);
	//析构函数
	//~List();
	//只读
	void Show()const;
	bool Empty()const;
	int Length()const
    {return this->length;}
	//bool CheckInput(T data);    //Set中不能出现相同的数
    bool IsOrdered()const;           //检验集合是否已经非降序排列
	//可写
	void make_empty();
	void Build();               //根据自己输入建立新链表
	bool Push(T data);          //插入尾部
	void InterSection(const List<T>& L1, const List<T>& L2, List<T>& Res);//L1，L2交集存到Res中
};

template<typename T>void List<T>::make_empty()
{
	this->head->link=NULL;
	length=0;
}

template<typename T>bool List<T>:: IsOrdered()const
{
    ListNode<T>* p=this->head->link;         //让p指向实际上的第一个节点
    while(p->link!=NULL)
    {
        if(p->data>p->link->data&&p->link->data!=-1)
        {
            return false;
        }
        p=p->link;
    }
    return true;
}

template<typename T>void List<T>::Show()const
{
	if (Empty())
		cout << "NULL";
	else {
		ListNode<T>* p = head;
		while (p->link != NULL)
		{
			cout << p->link->data << ' ';
			p = p->link;
		}
	}
}

template<typename T>bool List<T>::Empty()const
{
	return head->link == NULL;
}

template<typename T>void List<T>::Build()
{
	T data;
	cin >> data;
	Push(data);          //需要注意不加这两句，会导致第一个数进不去列表，解决方案是先打个空格
    if(data==-1)
    return;
	while (cin.get() != '\n')
	{
		cin >> data;
		Push(data);
        if(data==-1)
        return;
	}
}
template<typename T>bool List<T>::Push(T data)
{
	ListNode<T>* p = new ListNode<T>(data);
	ListNode<T>* q = head;
	while (q->link != NULL)
		q = q->link;
	p->link = q->link;
	q->link = p;
    this->length++;
	return true;
}
template<typename T>void List<T>::InterSection(const List<T>& L1, const List<T>& L2, List<T>& Res)
{
	ListNode<T>* p, * q, * start;
	p = L1.head;
	q = L2.head;
	start = L2.head;                     //用来标记我们每次从表二中查找的起始位置
	while (p->link != NULL)
	{
		for (q = start; q->link != NULL; q = q->link)
		{
			if (q->link->data == p->link->data&&q->link->data!=-1&&p->link->data!=-1)
			{
				Res.Push(p->link->data);
				start = q->link;
			}
		}
		p = p->link;
	}
}


int main()
{
	List<int> L;
	List<int> S;
	List<int> R;
    cout<<"请输入第一个连表,通过-1或者回车结束"<<endl;
	L.Build();        
    if(!L.IsOrdered())
    {
        cerr<<"请重新输入一个非降序列表"<<endl;
		L.make_empty();
        L.Build();
    }
    cout<<endl;
	cout << endl << "请初始化第二个，通过-1或者回车结束" << endl;
	S.Build();
    if(!S.IsOrdered())
    {
        cerr<<"请重新输入一个非降序列表"<<endl;
        S.make_empty();
		S.Build();
    }
	R.InterSection(L, S, R);
	cout << endl;
	R.Show();
	system("pause");
}