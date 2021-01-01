
#include <iomanip>
#include <string>
#include <cstdio>
#include <iostream>
#define DEFAULT_SIZE 30
using namespace std;

template<typename T, typename E>
struct MinSpanTreeNode {
	
	//这里面存的是边，所以和struct edge结构一样
	int tail, head;
	E weight;
	MinSpanTreeNode(): tail(-1), head(-1), weight(0) {}
	//与边相同的默认构造函数
	bool operator<= (MinSpanTreeNode<T, E>& R) { return weight <= R.weight; }
	bool operator> (MinSpanTreeNode<T, E>& R) { return weight > R.weight; }
	//由于之后要用最小堆，所以要定义顺序
};
template<typename T, typename E>
class MinSpanTree
{
public:
	MinSpanTreeNode<T, E>* edgevalue;
	MinSpanTree(int sz = DEFAULT_SIZE - 1) : maxSize(sz), n(0) {
		edgevalue = new MinSpanTreeNode<T, E>[sz];
	}
	bool InsertNode(MinSpanTreeNode<T, E>& node) {
		if (n == maxSize) {
			cerr << "最小生成树已满" << endl;
			return false;
		}
		else {
			edgevalue[n] = node;
			n++;
			return true;
		}
	}
private:
	int maxSize, n;
};
template<typename T, typename E>
//我们的最小堆里面存的是一个三元组（边）
//本质上就是最小生成树的树节点
class MinHeap
{
public:
	MinHeap(int size = DEFAULT_SIZE);
	~MinHeap() { delete[]heap; }
	bool insert(const MinSpanTreeNode<T, E>& x);
	bool remove(MinSpanTreeNode<T, E>& item);
	void siftDown(int start, int end);
	void siftUp(int start);
	int length() { return currentSize; }
	bool empty() { return currentSize == 0; }
	//void print();
private:
	MinSpanTreeNode<T, E> *heap;
	int currentSize;
	int maxSize;
};

template<typename T, typename E>
MinHeap<T, E>::MinHeap(int size) {
	if (DEFAULT_SIZE < size) {
		maxSize = size;
	}
	else {
		maxSize = DEFAULT_SIZE;
	}
	heap = new MinSpanTreeNode<T, E>[maxSize];
	if (heap == NULL) {
		cerr << "创建最小堆失败" << endl;
		exit(1);
	}
	currentSize = 0;
}


template<typename T, typename E>
void MinHeap<T, E>::siftDown(int start, int end) {
	MinSpanTreeNode<T, E> temp = heap[start];
	int i = start;
	int j = 2 * i + 1;
	while (j <= end) {
		if (heap[j] > heap[j + 1] && j < end)
			j++;
		if (temp <= heap[j])
			break;
		else {
			heap[i] = heap[j];
			i = j;
			j = 2 * j + 1;
		}
	}
	heap[i] = temp;
}

template<typename T, typename E>
void MinHeap<T, E>::siftUp(int start) {
	MinSpanTreeNode<T, E> temp = heap[start];
	int j = start;
	int i = (j - 1) / 2;
	while (j > 0) {
		if (heap[i] <= temp)
			break;
		else {
			heap[j] = heap[i];
			j = i;
			i = (i - 1) / 2;
		}
	}
	heap[j] = temp;
}

template<typename T, typename E>
bool MinHeap<T, E>::insert(const MinSpanTreeNode<T, E> &item) {
	if (currentSize == maxSize) {
		cerr << "当前最小堆是满的" << endl;
		return false;
	}
	heap[currentSize] = item;
	siftUp(currentSize);
	currentSize++;
	return true;
}

template<typename T, typename E>
bool MinHeap<T, E>::remove(MinSpanTreeNode<T, E>& item) {
	if (currentSize == 0) {
		cout << "当前最小堆是空的" << endl;
		return false;
	}
	item = heap[0];
	heap[0] = heap[currentSize - 1];
	currentSize--;
	siftDown(0, currentSize - 1);
	return true;
}
template<typename T,typename E>
class Graph;
template<typename E>
//我们采用领结表来实现图这一数据结构
struct Edge {
	//friend class Graph<T,E>;
	int dest;
	// dest是相应的节点在table中的索引
	E weight;
	// 边的权值
	Edge* link;
	Edge() {
		dest = -1;
		weight = 0;
		link = NULL;
	}
	Edge(int num, E weight) {
		dest = num;
		weight = weight;
		link = NULL;
	}
	bool operator!= (Edge<E>& R) const {
		return (dest != R.dest) ? true : false;
	}
};

template<typename T, typename E>
struct Vertex {
	//friend class Graph<T,E>;
	T data;
	Edge<E>* next;
};

template<typename T, typename E>
class Graph
{
public:
	Graph();
	//我们用系统自带的析构函数
	int num_of_vertices() { return vertices; }
	int num_of_edges() { return edges; }
	T get_value(int i) {
		return (i >= 0 && i < vertices) ? table[i].data : '0';
	}
	//由顶点的dest到顶点的data
	E get_weight(int v1, int v2);
	//得到这条边的权值
	bool insert_vertex(const T& vertex);
	//插入顶点
	bool insert_edge(int v1, int v2, E cost);
	//插入边
	int get_first_neighbor(int v);
	//得到第一个相邻顶点
	int get_next_nerghbor(int v, int w);
	//v的相邻顶点中在w顶点后面紧跟着的相邻顶点
	int get_vertex_pos(const T vertex) {return this->get_vertex_dest(vertex);}
	//由顶点的data到dest的map
private:
	Vertex<T, E> *table;
	int vertices;
	int maxVertices;
	int edges;
	int get_vertex_dest(const T &name_of_vertex)
	//做一个简单的封装
	{
		for(int i=0;i<this->vertices;i++)
		{
			if(this->table[i].data==name_of_vertex)
			{
				return i;
			}
		}
		return -1;
	}
};


template<typename T, typename E>
Graph<T, E>::Graph() {
	maxVertices = DEFAULT_SIZE;
	edges = 0;
	vertices = 0;
	table = new Vertex<T, E>[maxVertices];
	if (table == NULL) {
		cerr << "顶点数组创建失败" << endl;
		exit(1);
	}
	for (int i = 0; i < maxVertices; i++) {
		table[i].next = NULL;
	}
}

template<typename T, typename E>
int Graph<T, E>::get_first_neighbor(int v) {
	if (v != -1) 
	//根据我们的构造函数，-1代表没有
	{
		Edge<E>* p = table[v].next;
		if (p != NULL) 
			return p->dest;
	}
	return -1;
}

template<typename T, typename E>
int Graph<T, E>::get_next_nerghbor(int v, int w) {
	if (v != -1) {
		Edge<E>* p = table[v].next;
		while (p != NULL && p->dest != w) {
			p = p->link;
		}
		if (p != NULL && p->link != NULL) {
			return p->link->dest;
		}
	}
	return -1;
}

template<typename T, typename E>
E Graph<T, E>::get_weight(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		Edge<E>* p = table[v1].next;
		while (p != NULL && p->dest != v2) {
			p = p->link;
		}
		if (p != NULL) {
			return p->weight;
		}
	}
	return 0;
	//根据我们的构造函数，0代表没有边
}



template<typename T, typename E>
bool Graph<T, E>::insert_vertex(const T& vertex) 
{
	//这里仅仅是插入一个孤立的顶点。
	//在本提中我们就是先插顶点在插边的
	if (vertices == maxVertices) {
		return false;
	}
	else {
		table[vertices].data = vertex;
		vertices++;
		return true;
	}
}



template<typename T, typename E>
bool Graph<T, E>::insert_edge(int v1, int v2, E weight) 
{
	if (v1 >= 0 && v1 < vertices&&v2 >= 0 && v2 < vertices) {
		Edge<E>* q;
		Edge<E>* p = table[v1].next;
		while (p != NULL && p->dest != v2) {
			p = p->link;
		}
		//我们遍历以v1为头的边
		if (p != NULL) {
			cerr<<"这条边已经存在了"<<endl;
			return false;
		}
		//这代表这条边已经存在了
		p = new Edge<E>;
		q = new Edge<E>;
		//否则，可以插入这条边
		//我们默认都插到表头节点之后第一个，即头插法
		p->dest = v2;
		p->weight = weight;
		p->link = table[v1].next;
		table[v1].next = p;
		// 第一步是在v1引出的链上插入v2
		q->dest = v1;
		q->weight = weight;
		q->link = table[v2].next;
		table[v2].next = q;
		// 第二步是在v2引出的链上插入v1
		edges++;
		return true;
	}
	else {
		return false;
	}
}
template<typename T, typename E>
bool Prim(const T u0, Graph<T, E>& G, MinSpanTree<T, E>& MST) {
	// u0 是第一个顶点
	MinSpanTreeNode<T, E> ed;
	int i, u, v, count;
	//此为边节点辅助单元
	int n = G.num_of_vertices();
	int m = G.num_of_edges();
	if(m<n-1)
	{
		cout<<"边数过少，不存在最小生成树"<<endl;
		return false;
	}
	u = G.get_vertex_pos(u0);
	MinHeap<T, E> H(m);
	bool *Vmst = new bool[n];
	//Vmst数组记录我们已经访问过的那些顶点，即已经到最小堆里的
	for (int i = 0; i < n; i++) {
		Vmst[i] = false;
	}
	Vmst[u] = true;
	count = 1;
	//n个节点，n-1条边就结束了
	do {
		v = G.get_first_neighbor(u);
		while (v != -1) {
			if (Vmst[v] == false) {
				ed.head = v;
				ed.tail = u;
				ed.weight = G.get_weight(u, v);
				H.insert(ed);
			}
			//如果它的第一个相邻顶点没有被访问，则将其填到最小堆
			v = G.get_next_nerghbor(u, v);
			//找下一个相邻顶点
		}
		//上面本质上就是初始化最小堆，因为我们要从堆中找权值最小的
		while (!(H.empty()) && count < n) {
			// 移除权值最小的边
			H.remove(ed);
			if (Vmst[ed.head] == false) {
				MST.InsertNode(ed);
				//加入最小生成树
				u = ed.head;
				Vmst[u] = true;
				//务必要标记访问过了
				count++;
				break;
			}
		}
	}while (count < n);
	cout << "生成Prim最小生成树!" << endl;
	return true;

}

void addVertexes(Graph<char, int>& G, int n) {

	cout << "请输入各个顶点的名称：";
	while (n > 0) {
		char name;
		cin >> name;
		G.insert_vertex(name);
		n--;
	}
}

void addEdges(Graph<char, int>& G) {
	int n=G.num_of_vertices();
	while (1) 
	{
		if(G.num_of_edges()==n*(n-1)/2)
		{
			cout<<"边数已经达到最大值"<<endl;
			break;
		}
		cout << "请输入两个顶点及边：若要停止输入请在任意一顶点输入	#"<<endl;
		char v1,v2;
		int weight;
		cin >> v1 >> v2 >> weight;
		if(weight<=0)
		{
			cout<<"边的权值不符合要求，请重新输入边的权值"<<endl;
			while(1)
			{
				cin>>weight;
				if(weight>0)
				{
					break;
				}
				else
				{
					cout<<"边的权值不符合要求，请重新输入边的权值"<<endl;
				}
			}
		}
		if (v1 == '#' || v2 == '#') 
		{
			break;
		}
		int v1_pos = G.get_vertex_pos(v1);
		int v2_pos = G.get_vertex_pos(v2);
		if (!G.insert_edge(v1_pos, v2_pos, weight)) {
			cerr << "顶点创建失败，请检查后重新输入" << endl;
		}
	}
}


int main() 
{

    cout << "*===========================================================*" << endl;
    cout << "|                       电网造价模拟系统                       |" << endl;
    cout << "*===========================================================*" << endl;
    cout << "|                  选择：                                    |" << endl;
    cout << "|              A --- 创建电网顶点                             |" << endl;
    cout << "|              B --- 添加电网的边                             |" << endl;
    cout << "|              C --- 构造最小生成树                           |" << endl;
    cout << "|              D --- 显示最小生成树                           |" << endl;
    cout << "|              E --- 退出    程序                            |" << endl;
    cout << "*===========================================================*" << endl;
	
	Graph<char, int> G;
	MinSpanTree<char, int> MST;
	int num_vertex = 0;
	char option;
	int loop = 1;
	int Init = 0;
	bool flag=false;
	//防止我们还没有创建就直接显示最小生成树
	while (loop) {
		cout << endl << "请选择操作：";
		cin >> option;
		switch (option) {
		case 'A':
			if (Init == 1) {
				cerr << "已有电网存在，不要重复创建" << endl;
			}
			else {
				int n;
				cout<<"请输入要创建的节点数"<<endl;
				cin>>n;
				while(n<=0)
				{
					cout<<"创建的节点数不符合要求，请重新输入"<<endl;
					cin>>n;
				}
				num_vertex=n;
				addVertexes(G, num_vertex);
				Init = 1;
			}
			break;
		case 'B':
			if (Init != 1) {
				cout << "请先创建电网" << endl;
			}
			else {
				if(G.num_of_vertices()<=1)
				{
					cerr<<"顶点数小于等于1，无法创建边,请重新创建电网"<<endl;
					Init=0;
				}
				else
				{
					addEdges(G);
				}
			}
			break;
		case 'C':
			if (Init != 1) {
				cout << "请先创建电网" << endl;
				break;
			}
			cout << "请输入起始顶点：";
			char begin;
			cin >> begin;
			flag=Prim(begin, G, MST);
			break;
		case 'D': {
			if (Init != 1) {
				cout << "请先创建电网" << endl;
				break;
			}
			if(!flag)
			//代表我们还没构建最小生成树或者不存在最小生成树
			{
				cout<<"请先构造最小生成树或者最小生成树不存在"<<endl;
				break;
			}
			cout << "最小生成树的顶点及边分别为：" << endl;
			for (int i = 0; i < num_vertex - 1; i++) {
				char head = G.get_value(MST.edgevalue[i].tail);
				char tail = G.get_value(MST.edgevalue[i].head);
				cout << head << "-<" << MST.edgevalue[i].weight << ">->" << tail;
				if (i != num_vertex - 2) {
					cout << "\t";
				}
			}
			cout << endl;
			break;
		}
		case 'E':
			loop = 0;
			break;
		default:
			cout << "选项不存在，请重新输入" << endl;
			break;
		}
	}
}