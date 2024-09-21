#define _CRT_SECURE_NO_WARNINGS 
class QUEUE {
	int* const  elems;	//elems�����ڴ����ڴ�Ŷ��е�Ԫ��
	const  int  max;	//elems��������Ԫ�ظ���Ϊmax
	int   head, tail;	 	//����ͷhead��βtail���ӿ�head=tail;��ʼhead=tail=0
public:
	QUEUE(int m);		//��ʼ�����У��������m��Ԫ��
	QUEUE(const QUEUE& q); 			//��q�����ʼ������
	QUEUE(QUEUE&& q)noexcept;		//��q�ƶ���ʼ������
	virtual operator int() const noexcept;	//���ض��е�ʵ��Ԫ�ظ���
	virtual int size() const noexcept;		//���ض�����������Ԫ�ظ���max
	virtual QUEUE& operator<<(int e);  	//��e�����β���������ص�ǰ����
	virtual QUEUE& operator>>(int& e); 	//�Ӷ��׳�Ԫ�ص�e�������ص�ǰ����
	virtual QUEUE& operator=(const QUEUE& q);//�����ֵ�����ر���ֵ����
	virtual QUEUE& operator=(QUEUE&& q)noexcept;//�ƶ���ֵ�����ر���ֵ����
	virtual char* print(char* s) const noexcept;//��ӡ������s������s
	virtual ~QUEUE();	 					//���ٵ�ǰ����
};

class STACK : public QUEUE {
	QUEUE q;
public:
	STACK(int m);                    		//��ʼ��ջ�������2m-2��Ԫ��
	STACK(const STACK& s);         		//��ջs�����ʼ��ջ
	STACK(STACK&& s)noexcept;     		//��ջs�ƶ�������ʼ��ջ
	int  size()const noexcept;		  		//����ջ��������2m
	operator int() const noexcept;	   		//����ջ��ʵ��Ԫ�ظ���
	STACK& operator<<(int e); 	     		//��e��ջ�������ص�ǰջ
	STACK& operator>>(int& e);     		//��ջ��e�������ص�ǰջ
	STACK& operator=(const STACK& s);	//�����ֵ�����ر���ֵջ
	STACK& operator=(STACK&& s)noexcept;//�ƶ���ֵ�����ر���ֵջ
	char* print(char* b)const noexcept;	//��ջ�׵�ջ����ӡջԪ�� 
	~STACK()noexcept;	              	//����ջ
};