#include<iostream>
#include<time.h>
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define N 7     
using namespace std;
int Map[N][N];
int Mark[N][N];
/*迷宫问题的求解过程可以采用回溯法即在一定的约束条件下试探地搜索前进，
若前进中受阻，则及时回头纠正错误另择通路继续搜索的方法。从入口出发，
按某一方向向前探索，若能走通，即某处可达，则到达新点，否则探索下一个
方向；若所有的方向均没有通路，则沿原路返回前一点，换下一个方向再继续
试探，直到所有可能的道路都探索到，或找到一条通路，或无路可走又返回入口点。
在求解过程中，为了保证在达到某一个点后不能向前继续行走时，能正确返回前一个
以便从下一个方向向前试探，则需要在试探过程中保存所能够达到的每个点的下标
以及该点前进的方向，当找到出口时试探过程就结束了。*/
#define DEFAULT_SIZE 1024
template<typename T>
class stack
{
private:
    T *elements;      
    //存放栈中元素
    int top;          
    //栈顶指针
public:
    stack(int size=DEFAULT_SIZE);      
    //建立空栈
    ~stack(){delete []elements;}
    //析构函数，删除当前栈
    bool empty();
    //判断当前栈是否为空
    void pop();
    //退栈
    void push(const T &x);
    //压栈
    T gettop();
    //得到栈顶元素
};


template<typename T>
stack<T>::stack(int size)
{
    this->elements=new T[size];
    if(elements==NULL)
    {
        cerr<<"栈的创建失败"<<endl;
        exit(1);
    }
    top=-1;
}

template<typename T>
bool stack<T>::empty()
{
    return top==-1;
}

template<typename T>
void stack<T>::push(const T& x)
{
    this->elements[++top]=x;
}

template<typename T>
void stack<T>::pop()
{
    if(this->empty())
    {
        cerr<<"已经是空栈了"<<endl;
        exit(1);
    }
    top--;
}

template<typename T>
T stack<T>::gettop()
{
    if(this->empty())
    {
        cerr<<"已经是空栈了"<<endl;
        exit(1);
    }
    return this->elements[top];
}


struct items {
	int x,y;
	items(int x1,int y1):x(x1),y(y1){}
	items():x(0),y(0){}
};
//1代表围墙，0,代表路
void Maze(int size=N)
{
    srand((unsigned)time(NULL));        
	for(int i=0;i<N;i++)
		for (int j = 0; j < N; j++)
		{
				Map[i][j] = rand()%3==1? 1:0;
		}
    //如果总是产生无解迷宫，一个简单的解决方案是可以将3调大，使得产生的围墙稀疏一些
    //随机产生迷宫
	for (int i = 0; i <= size-2 + 1; i++)
		Map[0][i] = Map[size-2 + 1][i] = 1;
        //设置围墙
	for (int j = 0; j <= size-2 + 1; j++)
		Map[j][0] = Map[j][size-2 + 1] = 1;     
        //设置围墙
	Map[1][1] = Map[size-2][size-2] = 0;
        //打通出口以及入口
}
void Solve(int m, int n)     //m,n为迷宫的规模
{	
    //m，n为用户给出的迷宫规模
    if (m <= 0 || n <= 0)
		cerr << "迷宫错误" << endl;
	if (m > N || n > N)
		cerr << "迷宫过大" << endl;
    //一格简单的输入检测
	int i, j, g, h, d;
    //i,j分别储存当前位置的横纵坐标
    //g,h分别储存将要到达位置的横纵坐标
    //d储存将前行的方向
	items Move[5];
    //1，2，3，4分别对应上下左右，我们给出对应方向移动一格的坐标变化
	Move[1].x = -1; Move[1].y = 0;
	Move[2].x = 1;  Move[2].y = 0;
	Move[3].x = 0; Move[3].y = -1;
	Move[4].x = 0;  Move[4].y = 1;
	stack<items>S;
	stack<items>T;
	items tmp;
	tmp.x = 1;
	tmp.y = 1;
	S.push(tmp);
	while (!S.empty())
	{
		tmp = S.gettop();
		i = tmp.x;
		j = tmp.y;
		d = 1;
		while (d <= 4) {
			g = i + Move[d].x;
			h = j + Move[d].y;
			if (g == m && h == n )
            //已经到达终点
        {
				while (!S.empty())
			    {
					T.push(S.gettop());
					S.pop();
				}
				while(!T.empty())
				{
					cout << T.gettop().x << "," << T.gettop().y << " --> ";
					T.pop();
				}
				cout << m << "," << n ;
                //cout<<"：路径"<<endl;
				return;
		}
			if (Map[g][h] == 0 && Mark[g][h] == 0)
            //这说明目标位置是可以到达得
            {
				Mark[g][h] = 1;
				tmp.x = g;
				tmp.y = h;
				S.push(tmp);
				i = g;
				j = h;
				d = 1;
			}
			else
            //目标位置无法到达，换一个方向继续尝试
				d++;
		}
        //每一个方向都走不了，说明走到了死胡同，需要回溯到上一位置
		S.pop();
	}
    //栈已经空了，却还未到终点，说明此迷宫无解
	cout << "无解" << endl;
}


int main()
{
    Maze();
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << Map[i][j] << ' ';
		cout << endl;
	}
	Solve(5, 5);
	system("pause");
}

