
#define DEFAULT_SIZE 100
#include<iostream>
using namespace std;
template<typename T>class MinHeap {
private:
	T* heap;        
	//存放堆元素的数组
	int current_size;
	//当前的规模
	void SiftDown(int start, int end);    
	//向下调整函数
	void SiftUp(int start);               
	//向上调整函数
public:
	void Show() {
		for (int i = 0; i < current_size; i++)
			cout << heap[i] << '\t';
	}
	//作为测试函数，检测最小堆类是否正确
	MinHeap(int size = DEFAULT_SIZE);
	//构造函数
	~MinHeap() { delete[]heap; }
	T RemoveMin();
	//移除堆顶的最小元素
	bool Insert(const T& x);
	//插入x
};
template<typename T>MinHeap<T>::MinHeap(int size)
{
	heap = new T[size];
	if (heap == NULL) {
		cerr << "堆存储分配失败" << endl;
		exit(1);
	}
	current_size = 0;
}
template<typename T> void MinHeap<T>::SiftDown(int start, int end)
{
	//从start开始到end为止，如果子女节点值比父节点小，则子女节点较小的上浮（可能不止一次
	//继续向下层比较，调整局部最小堆
	//j=2*i+1,j为左子女位置
	int i = start;
	int j = 2 * i + 1;
	T temp = heap[i];       //占存，交换值用
	while (j <= end)
	{
		if (j < end) {
			j = heap[j] > heap[j + 1] ? j+1 : j ;
		}
		if (temp <= heap[j])
			break;                      //堆顶元素已经是最小不用调整
		else
		{
			heap[i] = heap[j];
			i = j;
			j = 2 * i + 1;         //下沉比较
		}
	}
		heap[i] = temp;
}
template<typename T>void MinHeap<T>::SiftUp(int start)
{
	int j = start;
	int i = (j - 1) / 2;
	T temp = heap[j];
	while (j > 0)//i>=0也可，但是j=0多比较一次
	{
		if (heap[i] <= temp)
			break;
		else
		{
			heap[j] = heap[i];
			j = i;
			i = (i - 1) / 2;         //向上继续比较
		}
	}
		heap[j] = temp;
}
template<typename T>bool MinHeap<T>::Insert(const T& x)
{
	heap[current_size] = x;
	SiftUp(current_size);
	current_size++;
	return true;
}
template<typename T>T MinHeap<T>::RemoveMin()
{
	T x;
	if (current_size <= 0) {
		cerr << "empty" << endl;
		return false;
	}
	x = heap[0];
	heap[0] = heap[current_size - 1];         //最后一个顶上去
	current_size--;
	SiftDown(0, current_size - 1);
	return x;
}

int main()
{
	int n;
	int temp;
	int ans=0;
	cout<<"请输入需要将木头砍为几块"<<endl;
	cin>>n;
	while(n<=0)
	{
		cout<<"块数不符合要求,请重新输入："<<endl;
		cin>>n;
	}
	MinHeap<int> H(n);
	cout<<"请依次输入权值"<<endl;
	for(int i=0;i<n;i++)
	{
		cin>>temp;
		while(temp<=0)
		{
			cout<<"所输入的权值不符合要求，请重新输入"<<endl;
			cin>>temp;
		}
		H.Insert(temp);
	}
	n--;
	while(n--)
	//砍n-1次
	{
		temp=H.RemoveMin()+H.RemoveMin();
		ans+=temp;
		H.Insert(temp);
	}
	cout<<ans;
	system("pause");
}