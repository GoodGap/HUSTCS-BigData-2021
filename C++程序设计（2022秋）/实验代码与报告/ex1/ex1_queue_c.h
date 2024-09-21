#define _CRT_SECURE_NO_WARNINGS

struct Queue {
	int* const  elems;	   	//elems申请内存用于存放队列的元素
	const  int  max;	  	//elems申请的最大元素个数max
	int   head, tail;	 	//队列头head和尾tail，队空head=tail;初始head=tail=0
};