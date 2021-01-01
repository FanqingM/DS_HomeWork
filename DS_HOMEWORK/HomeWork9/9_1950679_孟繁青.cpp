
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
using namespace std;
#define COURSE_NAME_SIZE 40
#define COURSE_PREPARATIONS_MAX 38
#define COURSE_NUM_MAX 40
#define COURSE_NUM 38
#define N 100

template<typename T>
class queue
{
    public:
    queue();

    bool push(const T x);
    bool pop();
    void show();
    bool empty()
    {return this->queue_rear==0;}
    bool search(const T x);
    T front();
    int size();
    int queue_rear;
    int queue_front;
    int *q;
};

template<typename T>
void queue<T>::show()
{
    for(int i=queue_front;i<queue_rear;i++)
    {
        cout<<q[i]+1<<" ";
    }
    cout<<endl;
}

template<typename T>
bool queue<T>::search(const T x)
{
    int i;
    for(i=this->queue_front;i<this->queue_rear;i++)
    {
        if(this->q[i]==x)
        break;
    }
    if(i==this->queue_rear)
    {
        return false;
    }
    else
    {
        return true;
    }
    
}

template<typename T>
queue<T>::queue()
{
    this->q=new T[N];
    this->queue_front=0;
    this->queue_rear=0;
}

template<typename T>
int queue<T>::size()
{
    return this->queue_rear - this->queue_front;
}

template<typename T>
bool queue<T>::push(const T x)
{
    if(this->size()>N-1)
    {
        cerr<<"队列已满"<<endl;
        return false;
    }
    else
    {
        this->q[this->queue_rear++]=x;
    }
    return true;
}

template<typename T>
T queue<T>::front()
{
    if(this->empty())
    {
        cerr<<"空队列"<<endl;
        exit(1);
    }
    else
    {
        return this->q[this->queue_front];
    }
}

template<typename T>
bool queue<T>::pop()
{
    if(this->empty())
    {
        cerr<<"空队列"<<endl;
        return false;
    }
    else
    {
        this->queue_front++;
        return true;
    }
}
vector<string> split(const string& sentence){
	istringstream iss(sentence);
    vector<string> vec;
	do{
        string sub;
        iss >> sub;

        vec.push_back(sub);
        //cout << sub << endl;
    } while (iss);
    return vec;
}

using namespace std;
struct Course
{
    int course_sub;
    //课程下标，需要注意标号1课程对应下标是0
    string course_name;
    //课程名字
    int course_hours;
    //课程学时
    int course_semester;
    //课程指定学期
    int course_preparations[COURSE_PREPARATIONS_MAX];
    //课程先修课数组
    int course_preparations_num;
    //课程先修课程数目
};

class Course_Arrangement
{
    public:
    void construct_schedule();
    Course_Arrangement();
    bool initial();
    //根据读取的文件初始化
    void write_schedule_to_file();
    //将排好的课程表写入用户制定文件
    void insert_course(int course_subs,int semester);
    //插入指定学期
    void convert_to_output();
    //转化为输出的课程表
    //private:
    int course_num;
    //拓扑排序中要用
    Course course_list[COURSE_NUM];
    //这个将course做了一个标号
    int class_schedule[8][5][4];
    //8学期，5天，4节课；
    //对应位置有课即为该课的索引，否则即为-1
    int output_class_schedule[8][5][10];
    //将class_schedule转化为可视化的课程表
    //如果我们不采用这种两种类型课程表进行过度，那么在插入过程中会重复涉及到大量的switch操作
    string filename;
    ifstream read_course_file;
    //读文件
    ofstream write_course_table;
    //写文件
    queue<int> prioity_queue;
    //为优先级队列，按出度最大，则其在队首
    int out[COURSE_NUM];
    //储存出度数组
    void sort(queue<int>& queue)
    //优先级队列的调整函数
    {
        for(int i=queue.queue_front;i<queue.queue_rear-1;i++)
        {
            for(int j=i;j<queue.queue_rear-1;j++)
            {
                if(this->out[queue.q[j]]<this->out[queue.q[j+1]])
                {
                    swap(queue.q[j],queue.q[j+1]);
                }
            }
        }
    }
};

void Course_Arrangement::construct_schedule()
{
    int num;
    int sum=0;
    int inserted[29]={0};
    for(int semester=0;semester<8;semester++)
    {
        this->insert_course(semester+29,semester);
    }
    this->insert_course(37,0);
    //由于这里对制定学期的课都没有先修课程，所以我们先排他们，这实际上是一个漏洞。
    for(int i=0;i<29;i++)
    {
        if(this->course_list[i].course_preparations_num==0)
        {
            this->prioity_queue.push(i);
        }
    }
    this->sort(prioity_queue);
    //初始化入度为0的优先级队列
    for(int semester=0;semester<8;semester++)
    {
        cout<<"请输入您想在 "<<semester+1<<" 学期上多少节课: ";
        cin>>num;
        while(num<0)
        {
            cout<<"课程数不能是负数"<<endl;
            cout<<"请重新输入"<<endl;
            cin>>num;
        }
        sum+=num;
        if(semester==0)
        {
            if(num<2)
            {
                cout<<"第一学期课程过少"<<endl;
            }
            if(num-2>prioity_queue.size())
            {
                cout<<"第一学期课程过多"<<endl;
            }
            while(num<2||num-1>prioity_queue.size())
            {
                cout<<"第"<<semester+1<<" 学期课程数不符合要求，请重新输入"<<endl;
                cin>>num;
            }
            for(int i=0;i<num-2;i++)
            {
                int course_id=this->prioity_queue.front();
                inserted[course_id]=1;
                this->insert_course(course_id,semester);
                this->prioity_queue.pop();
            }
            //根据用户要求，从优先级队列中拿出指定数目的课程
            for(int i=0;i<29;i++)
            {
                if(inserted[i]==1)
                {
                    for(int j=0;j<29;j++)
                    {
                        if(this->course_list[j].course_preparations[i]==1)
                        {
                            this->course_list[j].course_preparations[i]=-1;
                            this->course_list[j].course_preparations_num-=1;
                        }
                    }
                }
            }
            //根据拿出的课程更新边的关系以及入度数组
            for(int i=0;i<29;i++)
            {
                if(inserted[i]==0&&!prioity_queue.search(i)&&this->course_list[i].course_preparations_num==0)
                {
                    prioity_queue.push(i);
                }
            }
            this->sort(prioity_queue);
            //更新新的没有先修课程的队列
        }
        else
        //由于第一学期要先插入2个指定学期的课，后面都是一个，所以我们有两个分之
        //操作与之前相同
        {
            if(num<1)
            {
                cout<<"第"<<semester+1<<" 学期课程过少"<<endl;
            }
            if (num-1>prioity_queue.size())
            {
                cout<<"第"<<semester+1<<" 学期课程过多"<<endl;
            }
            while(num<1||num-1>prioity_queue.size())
            {
                cout<<"第"<<semester+1<<" 学期课程数不符合要求，请重新输入"<<endl;
                cin>>num;
            }
            
            for(int i=0;i<num-1;i++)
            {
                int course_id=this->prioity_queue.front();
                inserted[course_id]=1;
                this->insert_course(course_id,semester);
                this->prioity_queue.pop();
            }
            for(int i=0;i<29;i++)
            {
                if(inserted[i]==1)
                {
                    for(int j=0;j<29;j++)
                    {
                        if(this->course_list[j].course_preparations[i]==1)
                        {
                            this->course_list[j].course_preparations[i]=-1;
                            this->course_list[j].course_preparations_num-=1;
                        }
                    }
                }
            }
            for(int i=0;i<29;i++)
            {
                if(inserted[i]==0&&!prioity_queue.search(i)&&this->course_list[i].course_preparations_num==0)
                {
                    prioity_queue.push(i);
                }
            }
            this->sort(prioity_queue);
        }
    }
    cout<<endl;
    for(int i=0;i<29;i++)
    {
        cout<<inserted[i]<<" ";
    }
    cout<<endl;
    //检测是否所有课程都插入课程表中了；
    if(sum!=COURSE_NUM)
    {
        cout<<"您所输入的课程总数不满足要求"<<endl;
        return;
    }
    
}


Course_Arrangement::Course_Arrangement()
{

    this->course_num=0;
    for(int i=0;i<COURSE_NUM;i++)
    {
        this->course_list[i].course_preparations_num=0;
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<5;j++)
        {
            for(int k=0;k<4;k++)
            {
                this->class_schedule[i][j][k]=-1;
            }
        }
    }
}
bool Course_Arrangement::initial()
{
    for(int i=0;i<COURSE_NUM;i++)
    {
        for(int j=0;j<COURSE_PREPARATIONS_MAX;j++)
        {
            this->course_list[i].course_preparations[j]=-1;
        }
    }
    for(int i=0;i<COURSE_NUM;i++)
    {
        out[i]=0;
    }
    //不知道为什么在构造函数里面初始化就出错
    vector<vector<string> > vec;
    int i=0;
    cout<<"请输入课程清单所在文件的相对路径"<<endl;
    cin>>filename;
    this->read_course_file.open(filename,ios::in);
    while(!this->read_course_file)
    {
        cout<<"打开文件失败，请重新输入"<<endl;
        cin>>filename;
        this->read_course_file.open(filename,ios::in);
    }
    string temp_line;
    while(getline((this->read_course_file),temp_line))
    {
        vector<string> temp;
        temp=split(temp_line);
        vec.push_back(temp);
        i++;
    }
    this->course_num=i;
    cout<<"共"<<i<<"个课程"<<endl;
    //下面根据这个二维向量来初始化我们的course_list
    for(int j=0;j<38;j++)
    {
        //cout<<vec[j].size()<<endl;
        int sub=stoi(vec[j][0]);
        int hour=stoi(vec[j][2]);
        int semester=stoi(vec[j][3]);
        //将字符串转化为int型
        //cout<<j<<endl;
        this->course_list[j].course_sub=sub-1;
        this->course_list[j].course_name=vec[j][1];
        this->course_list[j].course_hours=hour;
        this->course_list[j].course_semester=semester;
        if(vec[j].size()>5)
        {
            int k=0;
            //说明这时候有先修课程
            for(int m=4;m<vec[j].size()-1;m++)
            {
                this->course_list[j].course_preparations[stoi(vec[j][m])-1]=1;
                //我们记这个位置为1，代表course_list[j]课程需要下表为stoi(vec[j][m])的课程上过才能上
                k++;
            }
            this->course_list[j].course_preparations_num=k;
            //cout<<" "<<k<<endl;
        }
        else
        {
            this->course_list[j].course_preparations_num=0;
            //我们认为0代表没有先修课
        }
    }
    //注意第k个课程，对应course_list中的下表是k-1;
    for(int i=0;i<COURSE_NUM;i++)
    {
        for(int j=0;j<COURSE_NUM;j++)
        {
            if(this->course_list[j].course_preparations[i]==1)
            {
                this->out[i]++;
            }
        }
    }
    //我们初始化出度数组，这需要我们在优先级队列里面进行使用
    for(int i=0;i<COURSE_NUM;i++)
    cout<<out[i]<<" ";
    cout<<endl;
    return true;
}
//符合要求

void Course_Arrangement::insert_course(int course_subs,int semester)
{
    int i;
    int j;
    int k;
    int r;
    bool flag=false;
    switch(this->course_list[course_subs].course_hours)
    {
        //我们根据课时来分类
        //在排课时，如一门课程有3节课，则优先安排3节课连续上；
        //如3节课连续无法按排，再优先安排两节课连续上，
        //最后再安排单节课上的情况；
        //并且对于一星期上两次的课，根据jump找到其下一次位置
        case 3:
        for(i=0;i<5;i++)
        {
            if(this->class_schedule[semester][i][1]!=-1&&this->class_schedule[semester][i][3]!=-1)
            //这一天3节大课的位置没有了
            {
                continue;
            }
            else
            {
                if(this->class_schedule[semester][i][1]==-1)
                {
                    this->class_schedule[semester][i][1]=course_subs;
                }
                else
                {
                    this->class_schedule[semester][i][3]=course_subs;
                }
                break;
            }
        }
        if (i==5)
        {
            cout<<"三学时课排课出错"<<endl;
        }
        break;
        case 4:
        for(i=0;i<5;i++)
        {
            if(this->class_schedule[semester][i][0]!=-1&&this->class_schedule[semester][i][2]!=-1)
            //这一天2节大课的位置没有了
            {
                continue;
            }
            else
            {
                if(this->class_schedule[semester][i][0]==-1)
                {
                    this->class_schedule[semester][i][0]=course_subs;
                }
                else
                {
                    this->class_schedule[semester][i][2]=course_subs;
                }
                break;
            }
        }
        if(i==5)
        {
            cout<<"四学时课排课错误"<<endl;
            return;
        }
        //否则插入了星期i+1
        //下面根据jump数组排第二次课
        //如果满足jump最好，不然我们插空
        j=(i+2-5>=0)?(i+2-5):(i+2);
        if(this->class_schedule[semester][j][0]!=-1&&this->class_schedule[semester][j][2]!=-1)
        {
            //差不到jump的那里
            for(k=0;k<5;k++)
            {
                if(this->class_schedule[semester][k][0]==-1)
                {
                    this->class_schedule[semester][k][0]=course_subs;
                    break;
                }
                if(this->class_schedule[semester][k][2]==-1)
                {
                    this->class_schedule[semester][k][2]=course_subs;
                    break;
                }
            }
            if(k==5)
            {
                cout<<"四学时的课第二节课排课错误"<<endl;
                return;
            }
        }
        else if(this->class_schedule[semester][j][0]==-1)
        {
            this->class_schedule[semester][j][0]=course_subs;
        }
        else
        {
            this->class_schedule[semester][j][2]=course_subs;
        }
        break;
        case 5:
        //先三后二，或者先二后三
        //我们这里可能得换一种想法了
        //我们从头遍历先出三就先三后二，否则先二后三，或者排不出来
        //但是如果我们先三的时候没有空位，就可以断言排不进去了
        for(i=0;i<5;i++)
        {
            if(flag)
            {
                break;
            }
            for(r=0;r<4;r++)
            {
                if(flag)
                {
                    break;
                }
                if(this->class_schedule[semester][i][r]==-1)
                {
                    //代表这地方没有课
                    switch(r)
                    {
                        case 0:
                        case 2:
                        //先二后三
                        this->class_schedule[semester][i][r]=course_subs;
                        j=(i+2-5>=0)?(i+2-5):(i+2);
                        if(this->class_schedule[semester][j][1]!=-1&&this->class_schedule[semester][j][3]!=-1)
                        {
                            //插不到jump的那里
                            for(k=0;k<5;k++)
                            {
                                if(this->class_schedule[semester][k][1]==-1)
                                {
                                    this->class_schedule[semester][k][1]=course_subs;
                                    flag=true;
                                    //goto re;
                                    break;
                                }
                                if(this->class_schedule[semester][k][3]==-1)
                                {
                                    this->class_schedule[semester][k][3]=course_subs;
                                    flag=true;
                                    //goto re;
                                    break;
                                }
                            }
                            if(k==5)
                            {
                                cout<<"六学时的课第二节课排课错误"<<endl;
                                return;
                            }
                        }
                        //插的到jump那里
                        else if(this->class_schedule[semester][j][1]==-1)
                        {
                            this->class_schedule[semester][j][1]=course_subs;
                            flag=true;
                            //goto re;
                            
                        }
                        else
                        {
                            this->class_schedule[semester][j][3]=course_subs;
                            flag=true;
                            //goto re;
                            
                        }
                        break;
                        case 1:
                        case 3:
                        //先三后二
                        this->class_schedule[semester][i][r]=course_subs;
                        j=(i+2-5>=0)?(i+2-5):(i+2);
                        if(this->class_schedule[semester][j][0]!=-1&&this->class_schedule[semester][j][2]!=-1)
                        {
                            //插不到jump的那里
                            for(k=0;k<5;k++)
                            {
                                if(this->class_schedule[semester][k][0]==-1)
                                {
                                    this->class_schedule[semester][k][0]=course_subs;
                                    flag=true;
                                    //goto re;
                                    break;
                                }
                                if(this->class_schedule[semester][k][2]==-1)
                                {
                                    this->class_schedule[semester][k][2]=course_subs;
                                    flag=true;
                                    //goto re;
                                    break;
                                }
                            }
                            if(k==5)
                            {
                                cout<<"六学时的课第二节课排课错误"<<endl;
                                return;
                            }
                        }
                        //插的到jump那里
                        else if(this->class_schedule[semester][j][0]==-1)
                        {
                            this->class_schedule[semester][j][0]=course_subs;
                            flag=true;
                            //goto re;
                            
                        }
                        else
                        {
                            this->class_schedule[semester][j][2]=course_subs;
                            flag=true;
                            //goto re;
                            
                        }
                        break;

                    }
                }

            }
        }
        if(i==5)
        {
            cout<<"课表以满"<<endl;
            return;
        }
        //re:
        break;
        case 6:
        for(i=0;i<5;i++)
        {
            if(this->class_schedule[semester][i][1]!=-1&&this->class_schedule[semester][i][3]!=-1)
            //这一天2节大课的位置没有了
            {
                continue;
            }
            else
            {
                if(this->class_schedule[semester][i][1]==-1)
                {
                    this->class_schedule[semester][i][1]=course_subs;
                }
                else
                {
                    this->class_schedule[semester][i][3]=course_subs;
                }
                break;
            }
        }
        if(i==5)
        {
            cout<<"六学时课排课错误"<<endl;
            return;
        }
        //否则插入了星期i+1
        //下面根据jump数组排第二次课
        //如果满足jump最好，不然我们插空
        j=(i+2-5>=0)?(i+2-5):(i+2);
        if(this->class_schedule[semester][j][1]!=-1&&this->class_schedule[semester][j][3]!=-1)
        {
            //差不到jump的那里
            for(k=0;k<5;k++)
            {
                if(this->class_schedule[semester][k][1]==-1)
                {
                    this->class_schedule[semester][k][1]=course_subs;
                    break;
                }
                if(this->class_schedule[semester][k][3]==-1)
                {
                    this->class_schedule[semester][k][3]=course_subs;
                    break;
                }
            }
            if(k==5)
            {
                cout<<"四学时的课第二节课排课错误"<<endl;
                return;
            }
        }
        else if(this->class_schedule[semester][j][1]==-1)
        {
            this->class_schedule[semester][j][1]=course_subs;
        }
        else
        {
            this->class_schedule[semester][j][3]=course_subs;
        }
        break;
    }
}

void Course_Arrangement::convert_to_output()
{
    for(int semester=0;semester<8;semester++)
    {
        for(int weekday=0;weekday<5;weekday++)
        {
            for(int index=0;index<4;index++)
            {
                int temp_mark=this->class_schedule[semester][weekday][index];
                switch(index)
                {
                    //一天被分为4大部分，每一个部分的课有的连上2节，有的三节，所以我们
                    //需要根据课程的index来分情况填充output_class_schedule
                    case 0:
                    this->output_class_schedule[semester][weekday][0]=temp_mark;
                    this->output_class_schedule[semester][weekday][1]=temp_mark;
                    break;
                    case 1:
                    this->output_class_schedule[semester][weekday][2]=temp_mark;
                    this->output_class_schedule[semester][weekday][3]=temp_mark;
                    this->output_class_schedule[semester][weekday][4]=temp_mark;
                    break;
                    case 2:
                    this->output_class_schedule[semester][weekday][5]=temp_mark;
                    this->output_class_schedule[semester][weekday][6]=temp_mark;
                    break;
                    case 3:
                    this->output_class_schedule[semester][weekday][7]=temp_mark;
                    this->output_class_schedule[semester][weekday][8]=temp_mark;
                    this->output_class_schedule[semester][weekday][9]=temp_mark;
                    break;
                }
            }
        }
    }
}

void Course_Arrangement::write_schedule_to_file()
{
    cout<<"请输入你要写入文件的相对路径"<<endl;
    cin>>this->filename;
    this->write_course_table.open(filename,ios::out);
    for(int semester=0;semester<8;semester++)
    {
        this->write_course_table<<endl;
        this->write_course_table<<"Semester"<<semester+1<<":"<<endl;
        this->write_course_table<<"*";
        for(int i=0;i<6;i++)
        {
            for(int j=0;j<COURSE_NAME_SIZE;j++)
            {
                this->write_course_table<<"=";
            }
            this->write_course_table<<"*";
        }
        this->write_course_table<<endl;
        for(int index=0;index<10;index++)
        {
            string temp="| Class ";
            
            this->write_course_table<<setiosflags(ios::left)
            <<setw(COURSE_NAME_SIZE)
            <<temp;
            for(int weekday=0;weekday<5;weekday++)
            {
                int temp_course_mark=this->output_class_schedule[semester][weekday][index];
                if(temp_course_mark!=-1)
                {
                    //代表有课
                    this->write_course_table<<"|";
                    int course_name_size=this->course_list[temp_course_mark].course_name.length();
                    int space_size=(COURSE_NAME_SIZE-course_name_size)/2;
                    int remain_space_size=COURSE_NAME_SIZE-course_name_size-space_size;
                    //做的打印课程居中处理
                    for(int i=0;i<space_size;i++)
                    {
                        this->write_course_table<<" ";
                    }
                    this->write_course_table<<this->course_list[temp_course_mark].course_name;
                    for(int i=0;i<remain_space_size;i++)
                    {
                        this->write_course_table<<" ";
                    }
                }
                else
                {
                    //代表没有课，我们在这里填入空格
                    this->write_course_table<<"|";
                    for(int i=0;i<COURSE_NAME_SIZE;i++)
                    {
                        this->write_course_table<<" ";
                    }
                }
            }
            this->write_course_table<<"|"<<endl;
            if(index<9)
            {
                this->write_course_table<<"*";
                for(int i=0;i<6;i++)
                {
                    for(int j=0;j<COURSE_NAME_SIZE;j++)
                    {
                        this->write_course_table<<"-";
                    }
                    if(i<5)
                    {
                        this->write_course_table<<"+";
                    }
                    else
                    {
                        this->write_course_table<<"*";
                    }
                    
                }
                this->write_course_table<<endl;
            }
        }
        this->write_course_table<<"*";
        for(int i=0;i<6;i++)
        {
            for(int j=0;j<COURSE_NAME_SIZE;j++)
            {
                this->write_course_table<<"=";
            }
            this->write_course_table<<"*";
        }
        this->write_course_table<<endl;
    }
}

int main()
{
    Course_Arrangement C;
    C.initial();
    C.construct_schedule();
    C.convert_to_output();
    C.write_schedule_to_file();  
}
