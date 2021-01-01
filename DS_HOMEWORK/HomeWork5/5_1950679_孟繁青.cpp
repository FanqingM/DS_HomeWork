#include<iostream>
using namespace std;
/*设某银行有A，B两个业务窗口，且处理业务的速度不一样，其中A窗口处理速度是B窗口
的2倍----即当A窗口每处理完2个顾客是，B窗口处理完1个顾客。给定到达银行的顾客序列，
请按照业务完成的顺序输出顾客序列。假定不考虑顾客信后到达的时间间隔，并且当不同窗
口同时处理完2个顾客时，A窗口的顾客优先输出。*/

template <typename T>
struct Node {
	Node(Node<T>* ptr = NULL) { next = ptr; }
	Node(const T& d, Node<T>* ptr = NULL) { data = d; next = ptr; }
	T data;
	Node<T>* next;
};

template <typename T>
class Queue
{
public:
	Queue() {
		head = new Node<T>();
		tail = head;
	}
    //构造函数，空队列
	void EnQueue(T d);
    //将数据插入队列
    int Size();
    //返回队列的大小
	T DeQueue();
    //出列，并且返回其值
    T Front();
    //返回队首的值
	bool isEmpty() { return head == tail; }
    //检测队列是否为空
	void print();
    //输出当前队列
private:
	Node<T>* head;
    //队头指针
	Node<T>* tail;
    //队尾指针
};

template<typename T>
int Queue<T>::Size()
{
    int length=0;
    Node<T>* p=head;
    while(p!=this->tail)
    {
        length++;
        p=p->next;
    }
    return length;
}

template<typename T>
T Queue<T>::Front()
{
    if(this->isEmpty())
    {
        cerr<<"队列目前是空的"<<endl;
        exit(1);
    }
    return head->next->data;
}

template<typename T>
void Queue<T>::EnQueue(T d)
{
	Node<T>* item = new Node<T>(d);
	tail->next = item;
	item->next = NULL;
	tail = item;
}

template<typename T>
T Queue<T>::DeQueue()
{
	if (head == tail) {
		cerr<<"队列目前是空的"<<endl;;
		exit(1);
	}
	Node<T>* item = head->next;
	//队列中只有一个元素
	if (head->next == tail) {
		head->next = NULL;
		tail = head;
	}
	else {
		head->next = item->next;
	}
	return item->data;
}


template<typename T>
void Queue<T>::print()
{
	Node<T>* item = head->next;
	while (item->next != NULL) {
		cout << item->data << " ";
		item = item->next;
	}
	// avoid the last blank
	cout << item->data;
}


void Divide(Queue<int>& Q, Queue<int>& Q1, Queue<int>& Q2)
{
	Q.DeQueue();            //去掉人数那一项
	while (!Q.isEmpty())
	{
		if (Q.Front() % 2 == 0)
			Q2.EnQueue(Q.Front());
		else
			Q1.EnQueue(Q.Front());
		Q.DeQueue();
	}
}
/*void Show(Queue<int>& Q)
{
	while (!Q.empty())
	{
		cout << Q.front()<<' ';
		Q.pop();
	}
}*/

void Solution(Queue<int>& Q1, Queue<int>& Q2)
{
	//Q1在A窗口处理，Q2在B窗口处理，该函数直接产生最终的输出结果
	//我们在主函数中将Q1与Q2分开
	Queue<int>Q;
	while (Q1.Size() >= 2 && Q2.Size() > 0)
	{
		Q.EnQueue(Q1.Front());
		Q1.DeQueue();
		Q.EnQueue(Q1.Front());
		Q1.DeQueue();
		Q.EnQueue(Q2.Front());
		Q2.DeQueue();
	}
	//Q1Q2都空就不用做操作了
	if (Q2.isEmpty() && !Q1.isEmpty())
	{
		while (!Q1.isEmpty())
		{
			Q.EnQueue(Q1.Front());
			Q1.DeQueue();
		}
	}
	if (!Q2.isEmpty() && Q1.isEmpty())
	{
		while (!Q2.isEmpty())
		{
			Q.EnQueue(Q2.Front());
			Q2.DeQueue();
		}
	}
	if (!Q2.isEmpty() && Q1.Size() == 1)
	{
		Q.EnQueue(Q1.Front());
		while (!Q2.isEmpty())
		{
			Q.EnQueue(Q2.Front());
			Q2.DeQueue();
		}
	}
	Q.print();
}
//Show()正确
int main()
{
	Queue<int>Q,Q1,Q2;
	int data;
	cout << "请输入队列，第一个数输入总人数" << endl;
	cin >> data;
	Q.EnQueue(data);  //需要注意不加这两句，会导致第一个数进不去列表，解决方案是先打个空格
	while (cin.get() != '\n')
	{
		cin >> data;
		Q.EnQueue(data);
	}
	Divide(Q, Q1, Q2);
	Solution(Q1, Q2);
	system("pause");
}