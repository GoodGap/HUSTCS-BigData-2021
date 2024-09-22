#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H
#include "RdtReceiver.h"
class StopWaitRdtReceiver :public RdtReceiver
{
private:
	int base;
	int NextSeqNum;	// 期待收到的下一个报文序号
	Packet lastAckPkt;				//上次发送的确认报文
	Packet ReceivedPacket[Seqlen];		//已发送并等待Ack的数据包
	bool packetWaitingFlags[Seqlen];     //用true和false标记窗口内每一个数据包是否处于等待状态（如果是等待，则说明处于缓存区，尚未交付）
public:
	StopWaitRdtReceiver();
	virtual ~StopWaitRdtReceiver();

public:
	
	void receive(const Packet &packet);	//接收报文，将被NetworkService调用
};

#endif

