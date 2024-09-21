#define _CRT_SECURE_NO_WARNINGS 
class QUEUE {
	int* const  elems;	//elems申请内存用于存放队列的元素
	const  int  max;	//elems申请的最大元素个数为max
	int   head, tail;	 	//队列头head和尾tail，队空head=tail;初始head=tail=0
public:
	QUEUE(int m);		//初始化队列：最多申请m个元素
	QUEUE(const QUEUE& q); 			//用q深拷贝初始化队列
	QUEUE(QUEUE&& q)noexcept;		//用q移动初始化队列
	virtual operator int() const noexcept;	//返回队列的实际元素个数
	virtual int size() const noexcept;		//返回队列申请的最大元素个数max
	virtual QUEUE& operator<<(int e);  	//将e入队列尾部，并返回当前队列
	virtual QUEUE& operator>>(int& e); 	//从队首出元素到e，并返回当前队列
	virtual QUEUE& operator=(const QUEUE& q);//深拷贝赋值并返回被赋值队列
	virtual QUEUE& operator=(QUEUE&& q)noexcept;//移动赋值并返回被赋值队列
	virtual char* print(char* s) const noexcept;//打印队列至s并返回s
	virtual ~QUEUE();	 					//销毁当前队列
};

class STACK : public QUEUE {
	QUEUE q;
public:
	STACK(int m);                    		//初始化栈：最多存放2m-2个元素
	STACK(const STACK& s);         		//用栈s深拷贝初始化栈
	STACK(STACK&& s)noexcept;     		//用栈s移动拷贝初始化栈
	int  size()const noexcept;		  		//返回栈的容量即2m
	operator int() const noexcept;	   		//返回栈的实际元素个数
	STACK& operator<<(int e); 	     		//将e入栈，并返回当前栈
	STACK& operator>>(int& e);     		//出栈到e，并返回当前栈
	STACK& operator=(const STACK& s);	//深拷贝赋值并返回被赋值栈
	STACK& operator=(STACK&& s)noexcept;//移动赋值并返回被赋值栈
	char* print(char* b)const noexcept;	//从栈底到栈顶打印栈元素 
	~STACK()noexcept;	              	//销毁栈
};