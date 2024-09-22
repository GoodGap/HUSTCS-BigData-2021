#ifndef RDT_RECEIVER_H
#define RDT_RECEIVER_H


#include "DataStructure.h"
//����RdtReceiver�����࣬�涨�˱���ʵ�ֵ�һ���ӿڷ���
//������������StopWaitRdtReceiver��GBNRdtReceiver���������һ�������ľ���ʵ��
//ֻ���ǵ����䣬�����շ�ֻ��������
struct  RdtReceiver
{
	virtual void receive(const Packet &packet) = 0;		//���ձ��ģ�����NetworkService����	
	virtual ~RdtReceiver() = 0;
};

#endif