#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

StopWaitRdtReceiver::StopWaitRdtReceiver():expectSequenceNumberRcvd(0)
{
	lastAckPkt.acknum = -1; //初始状态下，上次发送的确认包的确认序号为-1，使得当第一个接受的数据包出错时该确认报文的确认号为-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//忽略该字段
	for(int i = 0; i < Configuration::PAYLOAD_SIZE;i++)
		lastAckPkt.payload[i] = '.';
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

StopWaitRdtReceiver::~StopWaitRdtReceiver()
{
}

void StopWaitRdtReceiver::receive(const Packet &packet) {
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(packet);

	//如果校验和正确，同时收到报文的序号等于接收方期待收到的报文序号一致
	if (checkSum == packet.checksum && this->expectSequenceNumberRcvd == packet.seqnum) {
		pUtils->printPacket("接收方正确收到发送方的报文", packet);

		//取出Message，向上递交给应用层
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);

		lastAckPkt.acknum = packet.seqnum; //准备发出去的确认报文中，确认序号等于收到的报文序号
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt); //计算确认报文的检验和
		pUtils->printPacket("接收方发送确认报文", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		this->expectSequenceNumberRcvd++;
	}
	else {
		if (checkSum != packet.checksum)
			pUtils->printPacket("接收方没有正确收到发送方的报文，发送方数据包checksum错误", packet);
		else {
			pUtils->printPacket("接收方没有正确收到发送方的报文（丢弃），报文序号不满足预期", packet);
			cout << "接收方期待的序号为" << this->expectSequenceNumberRcvd << endl;
		}
		pUtils->printPacket("接收方重新发送上次的ACK报文", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送上次的确认报文
	}
}