#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H
#include "RdtReceiver.h"
class StopWaitRdtReceiver :public RdtReceiver
{
private:
	int base;
	int NextSeqNum;	// �ڴ��յ�����һ���������
	Packet lastAckPkt;				//�ϴη��͵�ȷ�ϱ���
	Packet ReceivedPacket[Seqlen];		//�ѷ��Ͳ��ȴ�Ack�����ݰ�
	bool packetWaitingFlags[Seqlen];     //��true��false��Ǵ�����ÿһ�����ݰ��Ƿ��ڵȴ�״̬������ǵȴ�����˵�����ڻ���������δ������
public:
	StopWaitRdtReceiver();
	virtual ~StopWaitRdtReceiver();

public:
	
	void receive(const Packet &packet);	//���ձ��ģ�����NetworkService����
};

#endif

