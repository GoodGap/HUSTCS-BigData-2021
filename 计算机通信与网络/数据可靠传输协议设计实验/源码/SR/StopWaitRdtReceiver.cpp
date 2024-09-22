#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

StopWaitRdtReceiver::StopWaitRdtReceiver():NextSeqNum(0),base(0)
{
	NextSeqNum = base + N;  //接收方滑动窗口上界
	lastAckPkt.acknum = -1; //初始状态下，上次发送的确认包的确认序号为-1，使得当第一个接受的数据包出错时该确认报文的确认号为-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//忽略该字段
	for(int i = 0; i < Configuration::PAYLOAD_SIZE;i++)
		lastAckPkt.payload[i] = '.';
	for (int i = 0; i < Seqlen; i++)
		packetWaitingFlags[i] = false;  //初始化，用false标记窗口内每一个数据包均处于等待状态，处于缓存区，尚未交付
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

StopWaitRdtReceiver::~StopWaitRdtReceiver()
{
}

void StopWaitRdtReceiver::receive(const Packet &packet) {
	int checkSum = pUtils->calculateCheckSum(packet);

	//如果校验和正确，同时收到报文的序号等于接收方期待收到的报文序号
	if (checkSum == packet.checksum) {
		cout << "接收方滑动窗口内容为 " << '[' << ' ';
		for (int i = 0; i < 4; i++)
			cout << base + i << ' ';
		cout << ']' << endl;

		if (base == packet.seqnum) {  //为接收方滑动窗口最前面的一个
			cout << "接收方收到报文" << packet.seqnum << endl;

			pUtils->printPacket("接收方正确收到发送方的报文", packet);
			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
			packetWaitingFlags[packet.seqnum % Seqlen] = true;
			ReceivedPacket[packet.seqnum % Seqlen] = packet;
			ReceivedPacket[packet.seqnum % Seqlen].acknum = 0;
			while (packetWaitingFlags[base % Seqlen] == true)  //对所有处于等待状态并且连续的数据包，取出Message，向上递交给应用层
			{
				Message msg;
				memcpy(msg.data, ReceivedPacket[base % Seqlen].payload, sizeof(ReceivedPacket[base % Seqlen].payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				packetWaitingFlags[base++ % Seqlen] = false;  //释放缓存区
				packetWaitingFlags[NextSeqNum++ % Seqlen] = false;  //接收窗口后移，初始化后释放缓冲区
				ReceivedPacket[packet.seqnum % Seqlen].acknum = -1;
			}
		}
		else if (base < packet.seqnum && packet.seqnum < NextSeqNum) {  //第一次收到不是接收方base的数据包，只缓存不交付
			cout << "接收方收到报文" << packet.seqnum << endl;
			pUtils->printPacket("接收方已缓存报文", packet);

			//将数据包放入缓存区
			ReceivedPacket[packet.seqnum % Seqlen] = packet;
			packetWaitingFlags[packet.seqnum % Seqlen] = true;  //该数据包等待交付

			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		}
		else if(packet.seqnum >= base-N && packet.seqnum < base)  //在接收方base后面的数据包，需要重发ACK，因为之前的ACK可能丢失
		{
			pUtils->printPacket("接收方正确收到已确认过的过时报文", packet);
			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		}
		else {
			pUtils->printPacket("接收方没有正确收到发送方的报文，报文序号不在[base-N, base+N-1]范围内", packet);
			cout << "此时接收方期待的序号是" << this->base << "~" << this->NextSeqNum << "之间" << endl;
		}
	}
	else pUtils->printPacket("接收方没有正确收到发送方的报文，发送方数据包checksum错误", packet);
}