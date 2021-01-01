#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<time.h>
#include <random>
#include <algorithm>
#define N 100000
#define MAX_SIZE 100000
using namespace std;

template<typename T>
class maxheap{
    //由于我们这里排序的都是数，就不写堆的节点struct了
    public:
    maxheap()
    {
        element=new T[N];
        if(element==NULL)
        {
            cout<<"创建堆失败"<<endl;
            exit(1);
        }
        currentsize=0;
    }
    maxheap(T* a,int num)
    {
        element=new T[N];
        if(element==NULL)
        {
            cout<<"创建堆失败"<<endl;
            exit(1);
        }
        for(int i=0;i<num;i++)
        {
            this->element[i]=a[i];
        }
        this->currentsize=num;
    }
    T *element;
    int currentsize;
    void siftdown(int start,int end,long long& swap_times,long long& compare_times);
    void siftup(int start);
    void show()
    {
        for(int i=0;i<currentsize;i++)
        {
            cout<<element[i]<<" ";
        }
    }
};

template<typename T>
void maxheap<T>::siftdown(int start,int end,long long& swap_times,long long& compare_times)
{
    int i=start;
    int j=2*i+1;
    T temp=this->element[i];
    while(j<=end)
    {
        compare_times++;
        if(j<end&&this->element[j]<element[j+1])
        {
            j++;
        }
        if(temp>=this->element[j])
        {
            break;
        }
        else
        {
            this->element[i]=this->element[j];
            i=j;
            j=2*j+1;
            swap_times+=1;
            //i下降到子女位置
        }
    }
    this->element[i]=temp;
}
//排为升序
template<typename T>
void show(T* a,int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
        if(i%30==0)
        {
            cout<<endl;
        }
    }
}

template<typename T>
void Bubble_Sort(T* a,int n,long long& swap_times,long long& compare_times)
{
    bool exchange;
    int i,j;
    swap_times=0;
    compare_times=0;
    //exchange检测是否发生交换
    for(int i=0;i<n-1;i++)
    {
        exchange=false;
        for(int j=0;j<n-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                swap(a[j],a[j+1]);
                swap_times+=1;
                exchange=true;
            }
            compare_times++;
        }
        if(exchange==false)
        {
            return;
        }
    }
}

template<typename T>
void Select_Sort(T* a,int n,long long& swap_times,long long& compare_times)
{
    for(int i=0;i<n;i++)
    {
        int k=i;
        //从k开始找最小的
        for(int j=i+1;j<n;j++)
        {
            if(a[j]<a[k])
            {
                k=j;
                swap_times++;
            }
            compare_times++;
        }
        if(k!=i)
        {
            swap(a[i],a[k]);
            swap_times++;
        }
    }
}
template<typename T>
void Insert_Sort(T* a,int n,long long& swap_times,long long& compare_times)
{
    int i ;
    for(i = 1;i < n;i++)
    {
        compare_times++;
        if(a[i] < a[i - 1])//else则不做操作
        {
            int j = i - 1;
            T temp = a[i];
            for(;j >= 0;j--)
            {
                compare_times++;
                if(temp < a[j])
                {
                    a[j + 1] = a[j];
                    swap_times++;
                    continue;
                }
                break;
            }
            a[j + 1] = temp;
        }
    }
}

template<typename T>
void Shell_Sort(T* a,int n,long long& swap_times,long long& compare_times)
{
    int i,j;
    int gap=(int)(n/2);
    T temp;
    while(gap)
    {
        for(int i=0+gap;i<n;i++)
        {
            for(int j=i;j>=gap;j=j-gap)
            {
                compare_times++;
                if(a[j]>a[j-gap])
                {
                    break;
                }
                else
                {
                    swap(a[j],a[j-gap]);
                    swap_times++;
                }
                
            }

        }
        gap = gap == 2 ? 1 : (int)(gap / 2);
    }
}

template<typename T>
void Quick_Sort(T* a,int left,int right,long long& swap_times,long long& compare_times)
{
    //传0与num-1
    int l=left;
    int r=right;
    int temp;
    if(l <= r)
    //要<=，<时l=r没定义temp
    {   
        temp = a[l];  //待排序的第一个元素作为基准元素
        while(l != r){   //从左右两边交替扫描，直到left = right
            //这是一个交叉走的过程
            compare_times+=2;
                while(r > l && a[r] >= temp)  
                     r --;        //从右往左扫描，找到第一个比基准元素小的元素
                  a[l] = a[r];  //将该元素与left所指交换
                  swap_times++;

                while(l < r && a[l] <= temp)
                     l ++;         //从左往右扫描，找到第一个比基准元素大的元素
                  a[r] = a[l];  //将该元素与right所指交换
                  swap_times++;
            }
            a[r] = temp;    //基准元素归位
            //前面属于分的策略
            Quick_Sort(a,left,l-1,swap_times,compare_times);  //对基准元素左边的元素进行递归排序
            Quick_Sort(a, r+1,right,swap_times,compare_times);  //对基准元素右边的进行递归排序
    }        
    
}
template<typename T>
void Heap_Sort(T *a,int num,long long& swap_times,long long& compare_times)
{
    maxheap<int> H(a,num);
    for(int i=(H.currentsize-2)/2;i>=0;i--)
    {
        H.siftdown(i,H.currentsize-1,swap_times,compare_times);
    }
    //初始化最大堆
    for(int i=H.currentsize-1;i>=0;i--)
    {
        //交换，重建最大堆
        swap(H.element[0],H.element[i]);
        swap_times+=1;
        H.siftdown(0,i-1,swap_times,compare_times);
    }
    for(int i=0;i<num;i++)
    {
        a[i]=H.element[i];
    }
}

template<typename T>
void merge(T *L1,T* L2,const int left,const int mid,const int right,long long& swap_times,long long& compare_times)
{
    //L1[left:mid]，与L1[mid+1,right]合并
    //合并成L1[left:right]
    for(int k=left;k<=right;k++)
    {
        L2[k]=L1[k];
    }
    //我们需要一个等长的额外的空间
    int s1=left;
    int s2=mid+1;
    int t=left;
    while(s1<=mid&&s2<=right)
    {
        compare_times++;
        if(L2[s1]<=L2[s2])
        {
            L1[t++]=L2[s1++];
            swap_times++;
        }
        else
        {
            L1[t++]=L2[s2++];
            swap_times++;
        } 
    }
    while(s1<=mid)
    {
        L1[t++]=L2[s1++];
        swap_times++;
    }
    while(s2<=right)
    {
        L1[t++]=L2[s2++];
        swap_times++;
    }

}

template<typename T>
void Merge_Sort(T *a,T* L2,int left,int right,long long& swap_times,long long& compare_times)
{
    //输入left 0
    //输入right num-1
    if(right<=left)
    {
        return;
    }
    int mid=(left+right)/2;
    Merge_Sort(a,L2,left,mid,swap_times,compare_times);
    Merge_Sort(a,L2,mid+1,right,swap_times,compare_times);
    merge(a,L2,left,mid,right,swap_times,compare_times);
}
template<typename T>
void Radix_Sort(T* a,int n,long long& swap_times,long long& compare_times)
{
    int max=a[0];
    for(int i=1;i<n;i++)
    {
        if(a[i]>max)
        {
            max=a[i];
        }
    }
    //我们找出最大数，因为我们需要他的位数
    int d=1;//位数
    int temp[N];
    while(max/d>0)
    {
        int bucket[10]={0};
        //因为只有十个数，0，1，。。。9
        //每一轮根据基数排序都要重置基数桶
        for(int i=0;i<n;i++)
        {
            bucket[(a[i]/d)%10]++;
        }

        //求出基数桶的边界索引
        for(int i=1;i<10;i++)
        {
            bucket[i]+=bucket[i-1];
        }
        //从右向左扫描，保证排序稳定性

        for(int i=n-1;i>=0;i--)
        {
            temp[--bucket[(a[i]/d)%10]]=a[i];
        }

        for(int i=0;i<n;i++)
        {
            a[i]=temp[i];
        }
        d*=10;
    }
}


int main()
{
    clock_t start,finish;
    int num;
    int choose;
    double totaltime;
    long long swap_times;
    long long compare_times;
    int a[N];
    int b[N];
    cout << "**" << setw(10) << ' ' << "排序算法比较" << setw(10) << ' ' << "**" << endl;
	cout << "====================================" << endl;
	cout << "**" << setw(9) << ' ' << "1 --- 冒泡排序" << setw(9) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "2 --- 选择排序" << setw(9) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "3 --- 直接插入排序" << setw(5) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "4 --- 希尔排序" << setw(9) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "5 --- 快速排序" << setw(9) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "6 --- 堆排序" << setw(11) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "7 --- 归并排序" << setw(9) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "8 --- 基数排序" << setw(9) << ' ' << "**" << endl;
	cout << "**" << setw(9) << ' ' << "9 --- 退出程序" << setw(9) << ' ' << "**" << endl;
	cout << "====================================" << endl;
    cout<<endl<<"请输入您想要产生1000还是10000还是100000个随机数,输入请勿超过100000"<<endl;
    cin>>num;
    while(num>N)
    {
        cout<<"所输入数字过大，请重新输入"<<endl;
        cin>>num;
    }
    while(num<=0)
    {
        cout<<"所输入数字过小，请重新输入"<<endl;
        cin>>num;
    }
    srand((unsigned)time(NULL));
    for(int i=0;i<num;i++)
    {
        int x;
        x=rand();
        //cin>>x;
        a[i]=x;
    }
    for(int i=0;i<num;i++)
    {
        b[i]=a[i];
    }
    //show(a,num);
    cout<<endl<<"请输入您希望进行哪一种排序"<<endl;
    cin>>choose;
    while(choose!=9)
    {
        switch(choose)
        {
            case 1:
            swap_times=0;
            compare_times=0;
            start=clock();
            Bubble_Sort(b,num,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for bubble sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for bubble sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for bubble sort"<<endl;
            break;
            case 2:
            swap_times=0;
            compare_times=0;
            start=clock();
            Select_Sort(b,num,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for select sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for select sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for select sort"<<endl;
            break;
            case 3:
            swap_times=0;
            compare_times=0;
            start=clock();
            Insert_Sort(b,num,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for insert sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for insert sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for insert sort"<<endl;
            break;
            case 4:
            swap_times=0;
            compare_times=0;
            start=clock();
            Shell_Sort(b,num,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for shell sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for shell sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for shell sort"<<endl;
            break;
            case 5:
            swap_times=0;
            compare_times=0;
            start=clock();
            Quick_Sort(b,0,num-1,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for quick sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for quick sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for quick sort"<<endl;
            break;
            case 6:
            swap_times=0;
            compare_times=0;
            start=clock();
            Heap_Sort(b,num,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for heap sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for heap sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for heap sort"<<endl;
            break;
            case 7:
            swap_times=0;
            compare_times=0;
            start=clock();
            int* L2;
            L2=new int[N];
            Merge_Sort(b,L2,0,num-1,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for merge sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for merge sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for merge sort"<<endl;
            break;
            case 8:
            swap_times=0;
            compare_times=0;
            start=clock();
            Radix_Sort(b,num,swap_times,compare_times);
            finish=clock();
            totaltime=double(finish-start)/CLOCKS_PER_SEC;
            cout<<endl<<1000*totaltime<<"ms"<<" for radix sort"<<endl;
            cout<<endl<<swap_times<<"swap times"<<" for radix sort"<<endl;
            cout<<endl<<compare_times<<"compare times"<<" for radix sort"<<endl;
            break;
        }
        for(int i=0;i<num;i++)
        {
            b[i]=a[i];
        }
        cout<<endl<<"请输入您希望进行哪一种排序"<<endl;
        cin>>choose;
    }
}
