#include<iostream>
#include<string>
#include<iomanip>
using namespace std;
//采用左长子右兄弟,多个第一祖先，可能是森林，练成树，所以可以只设一个根节点
/*本项目的实质是完成兑家谱成员信息的建立，查找，插入，修改，删除等功能，
可以首先定义家族成员数据结构，然后将每个功能作为一个成员函数来完成对数
据的操作，最后完成主函数以验证各个函数功能并得到运行结果。*/

template<typename T> struct TreeNode {
	T data;
	//数据
	TreeNode<T>* firstChild, * nextSibling;
	//分别是长子以及兄弟
	TreeNode() :firstChild(NULL), nextSibling(NULL) {}
	TreeNode(T value) :data(value), firstChild(NULL), nextSibling(NULL) {}
	TreeNode(T value, TreeNode<T>* left, TreeNode<T>* next) :data(value), firstChild(left), nextSibling(next) {}
	//三种构造函数
};
template<typename T>void visit(TreeNode<T>* p)
{
	cout << p->data<<' ';
}
template<typename T>class Tree {
private:
	TreeNode<T>* root, * current;
public:
	//构造函数
	Tree() { root = current = NULL; }
	Tree(T value) { root = new TreeNode<T>(value); current = root; }
	Tree(Tree<T>& OtherTree);
	//只读接口
	void Root() { current = root; }        
	//让current重新变为root
	bool Empty() { return root == NULL; }
	//检查是否为空树
	bool Parent(T value, TreeNode<T>* t);   
	//在Parent子树中找value的爸爸，并将其存到current中
	bool Parent();            
	//在整颗树中找当前结点的爸爸，并使之成为当前节点
	bool Parent(TreeNode<T>* t, TreeNode<T>* p); 
	//在跟为*t的子树中找*p的父节点
	bool Search(TreeNode<T>* t, T value);              
	//子树t里找value，并使之成为当前节点
	bool Search(T value);                                                      
	//在全树中查找，并使之成为当前结点
	bool Search(TreeNode<T>* t, TreeNode<T>* p); 
    //在t子树里面找p
	bool Search(TreeNode<T>* p) { return Search(current, p); }   
	//从当前节点的子树里找p节点
	bool FirstChild(TreeNode<T>* subTree);
	//找subtree节点的长子，并存到当前节点里
	bool NextSibling(TreeNode<T>* subtree);
	//找subtree节点的兄弟，并存到当前节点里
	void ShowCurrent();
	//输出当前节点的值
	void ShowCurrentChild();
	//输出当前节点的第一代儿女
	bool VisitFamily(TreeNode<T>* t, void(*visit)(TreeNode<T>*));
	//遍历t子树
	bool VisitFamily(void(*visit)(TreeNode<T>*));       
	//遍历当前节点对应的子树                       
	//可写接口
	void Start();                               
	//大菜单
	void Change();
	//修改函数
	bool Insert(T value, TreeNode<T>* Parent);
	//插到parent底下
	bool Insert(T value);
	//默认插到当前结点的孩子
	void Delete();
	//删除此人的家庭                        
	void BuildFamily();
	//建立家庭
};
template<typename T>
void Tree<T>::ShowCurrentChild()
{
	TreeNode<T>* q = this->current->firstChild;
	if(q==NULL)
	{
		cout<<"没有孩子"<<endl;
		return;
	}
	while(q!=NULL)
	{
		cout<<q->data<<" ";
		q=q->nextSibling;
	}
}
template<typename T>void Tree<T>::Start()
{
	
	cout << "首先建立一个家谱" << endl;
	cout << "请输入祖先的名字" << endl;
	T s;
	char s1;
	cin >> s;
	root = new TreeNode<T>(s);
	current = root;
	cout << "祖先的名字是" << ' ' << s << endl;
	cout << "请输入您需要的操作" << endl;
	cin >> s1;
	int loop=1;
	while (1)
	{
		switch (s1) {
		case 'A':
			BuildFamily();
			break;
		case 'B':
			BuildFamily();
			break;
		case 'C':
		    this->Delete();
			break;
		case 'D':
		    this->Change();
			break;
		case 'E':
		    this->VisitFamily(root,visit);
			cout<<endl;
			break;
		case 'F':
		    this->VisitFamily(visit);
			cout<<endl;
			break;
		case 'G':
		    cout<<"欢迎下次使用"<<endl;
			return;
			break;
		default:
			cout << "无此操作" << endl;
			break;
		}
		cout<< "请输入您要进行的操作" << endl;
		cin >> s1;
	}
}
template<typename T>void Tree<T>::Change()
{
	T s;
    while(1)
		{
			cout << "请输入您要改变的人的名字" << endl;
			cin >> s;
			if (!this->Search(s))
			{
				cerr << "查无此人，请重新输入" << endl;
			}
			else
			{
				cout<<"请输入您要将其改为何名"<<endl;
				cin>>s;
				current->data = s;
				cout<<"已经更名为"<<s<<endl;
				break;
			}
		}
}

template<typename T>void Tree<T>::ShowCurrent()
{
	cout << current->data;
}
template<typename T>bool Tree<T>::Parent(TreeNode<T>* t, TreeNode<T>* p)
{
	bool result;
	TreeNode<T>* q = t->firstChild;
	while (q != NULL && q != p)
	{
		if ((result = Parent(q, p)) == true)
			return result;
		else
			q = q->nextSibling;
	}
	if (q != NULL && q == p) {
		current = t;
		return true;
	}
	else {
		current = NULL;
		return false;
	}
}

template<typename T>bool Tree<T>::Parent()
{
	TreeNode<T>* p = current;
	if (p == NULL && p == root) {
		current = NULL;
		return false;
	}
	else
		return Parent(root, p);
}
template<typename T>bool Tree<T>::Parent(T value, TreeNode<T>* t)
{
	bool result = false;
	TreeNode<T>* p = t->firstChild;
	while (p != NULL && p->data != value)
	{
		if ((result = Parent(value, p)) == true)
			return result;
		else
			p = p->nextSibling;
	}
	if (p != NULL && p->data == value) {
		current = t;
		return true;
	}
	else {
		current = NULL;
		return false;
	}
}

template<typename T>bool Tree<T>::Search(TreeNode<T>* t, T value)
{
	if (t == NULL) {
		cout << "空子树" << endl;
		return false;
	}
	bool result = false;
	TreeNode<T>* p = t;
	if (p->data == value) {
		current = t;
		return true;
	}
	else
	{
		p = p->firstChild;
		while (p != NULL && p->data != value)
		{
			if((result = Search(p, value)) == true){
			current = p;
			return true;
}
			else
				p = p->nextSibling;
		}
		if (p != NULL && p->data == value) {
			current = p;
			return true;
		}
		else {
			current = NULL;
			return false;
		}
	}
}
template<typename T>bool Tree<T>::Search(TreeNode<T>* t, TreeNode<T>* q)
{
	bool result = false;
	if (t == NULL || q == NULL) {
		cout << "子树或者要查找结点为空" << endl;
		return false;
	}
	else
	{
		TreeNode<T>* p = t;
		if (p==q) {
			current = t;
			return true;
		}
		else
		{
			p = p->firstChild;
			while (p != NULL && p!= q)
			{
				if ((result = Search(p, q)) == true) {
					current = p;
					return true;
				}
				else
					p = p->nextSibling;
			}
			if (p != NULL && p==q) {
				current = p;
				return true;
			}
			else {
				current = NULL;
				return false;
			}
		}
	}
}
template<typename T>bool Tree<T>::Search(T value)
{
	if (root == NULL) {
		current = NULL;
		return false;
	}
	else
		return Search(root, value);
}

template<typename T>bool Tree<T>::FirstChild(TreeNode<T>* subTree)
{
	if (subTree == NULL)
		return false;
	else
	{
		current = subTree->firstChild;
		return true;
	}
}

template<typename T>bool Tree<T>::NextSibling(TreeNode<T>* subTree)
{
	if (subTree == NULL)
		return false;
	else
	{
		current = subTree->nextSibling;
		return true;
	}
}

template<typename T>bool Tree<T>::Insert(T value, TreeNode<T>* Parent)
{
	if (Parent == NULL)
		return false;
	else {
		TreeNode<T>* p = Parent->firstChild;
		TreeNode<T>*NewChild = new TreeNode<T>(value);
		Parent->firstChild = NewChild;
		NewChild->nextSibling = p;
		return true;
	}
}

template<typename T>bool Tree<T>::Insert(T value)
{
	return Insert(value, current);
}

template<typename T>void Tree<T>::Delete()
{
	T name;
	cout<<"请输入您要解散家庭的人的名字"<<endl;
	cin>>name;
	while(!this->Search(name))
	{
		cout<<"查无此人，请重新输入"<<endl;
		cin>>name;
	}
	if (current->firstChild == NULL)
	{
		cerr << "该人无家庭" << endl;
		return ;
	}
	else
	{
		TreeNode<T>* p = current->firstChild;
		cout<<name<<":的第一代子孙为："<<endl;
		while (p != NULL)
		{	
			cout<<p->data<<" ";
			TreeNode<T>* q = new TreeNode<T>;
			q = p;
			p = p->nextSibling;
			delete q;
		}
		cout<<endl;
		current->firstChild = NULL;
		return;
	}
}
template<typename T>void Tree<T>::BuildFamily()
{
	T value;
	cout << "请输入您要建立家庭的人的姓名" << endl;
	cin >> value;
	while(!Search(value))
	{
		cerr << "家谱中没有这号人" << endl;
		cout<<"请重新输入"<<endl;
		cin>>value;
	}
		Search(value);                                       //找到了current，current->data=value;
		cout << "请输入您要加入的孩子" << endl;
		T data;
		cin >> data;
		Insert(data);          //需要注意不加这两句，会导致第一个数进不去，解决方案是先打个空格
		while (cin.get() != '\n')
		{
			cin >> data;
			Insert(data);
		}
		cout<<value<<"的第一代儿女是："<<endl;
	    this->ShowCurrentChild();
		cout<<endl;
}

template<typename T>bool Tree<T>::VisitFamily(TreeNode<T>* t, void(*visit)(TreeNode<T>*))
{
	if (t == NULL) {
		cout << "NULL";
		return false;
	}
	else
	{
		TreeNode<T>* p = t->firstChild;
		visit(t);
		while (p != NULL)
		{
			VisitFamily(p, visit);
			p = p->nextSibling;
		}
		return true;
	}
}
template<typename T>bool Tree<T>::VisitFamily(void(*visit)(TreeNode<T>*))
{
	return VisitFamily(current, visit);
}

int main()
{
	 cout << "**" << setw(14) << ' ' << "家谱管理系统" << setw(16) << ' ' << "**" << endl;
	 cout << "==============================================" << endl;
	 cout << "**" << setw(13) << ' ' << "请选择要执行的操作" << setw(11) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "A --- 完善家谱" << setw(15) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "B --- 添加家庭成员" << setw(11) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "C --- 解散局部家庭" << setw(11) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "D --- 更改家庭成员姓名" << setw(7) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "E --- 展示全部家谱" << setw(11) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "F --- 展示当前家谱" << setw(11) << ' ' << "**" << endl;
	 cout << "**" << setw(13) << ' ' << "G --- 退出程序" << setw(15) << ' ' << "**" << endl;
	 
	 cout << "==============================================" << endl;
	 Tree<char> T;
     T.Start();
	 system("pause");
}
