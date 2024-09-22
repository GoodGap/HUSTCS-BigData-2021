#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"

StopWaitRdtSender::StopWaitRdtSender():base(0),expectSequenceNumberSend(0),waitingState(false)
{
	for (int i = 0; i < Seqlen; i++) {
		ACKFlags[i] = false;  //初始化，缓冲区内ACK均默认未收到
		packetWaitingAck[i % Seqlen].seqnum = -1;
	}
}

StopWaitRdtSender::~StopWaitRdtSender()
{
}

bool StopWaitRdtSender::getWaitingState() {
	return waitingState;
}

bool StopWaitRdtSender::send(const Message &message) {
	if (this->waitingState) //发送方处于等待确认状态
		return false;

	if (expectSequenceNumberSend < base + N) {
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].acknum = -1; //忽略该字段
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].seqnum = this->expectSequenceNumberSend;
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = 0;
		memcpy(this->packetWaitingAck[expectSequenceNumberSend % Seqlen].payload, message.data, sizeof(message.data));
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);
		ACKFlags[expectSequenceNumberSend % Seqlen] = false;

		pUtils->printPacket("发送方发送报文", this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);
		
		cout << "发送方启动报文" << expectSequenceNumberSend << "的计时器" << endl;
		pns->startTimer(SENDER, Configuration::TIME_OUT, expectSequenceNumberSend);			//启动发送基序列方定时器（对每个报文都有一个单独的计时器）
		
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);		//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
		expectSequenceNumberSend++;
		
		cout << "此报文发送后，下一发送序号seqnum = " << expectSequenceNumberSend << endl;
		if (expectSequenceNumberSend == base + N)
			this->waitingState = true;//进入等待状态
	}
	return true;
}

void StopWaitRdtSender::receive(const Packet& ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确，并且确认序号=发送方已发送并等待确认的数据包序号
	if (checkSum == ackPkt.checksum) {
		pUtils->printPacket("发送方正确收到确认", ackPkt);
		for (int i = base + N; i < base + 8; i++)
			packetWaitingAck[i % Seqlen].seqnum = -1;  //重置缓冲区
		cout << "发送方当前滑动窗口为（数字代表已发送） " << '[' << ' ';
		for (int i = base; i < base + N; i++) {
			if (packetWaitingAck[i % Seqlen].seqnum == -1)
				cout << '*' << ' ';
			else
				cout << packetWaitingAck[i % Seqlen].seqnum << ' ';
		}
		cout << ']' << endl;

		if (ackPkt.acknum == base) {
			cout << "已确认序号为" << ackPkt.acknum << "的ACK，该序号为此时基序号base，发送方滑动窗口向前移动" << endl;
			pns->stopTimer(SENDER, ackPkt.acknum);
			ACKFlags[base % Seqlen] = true;
			while (ACKFlags[base % Seqlen])
				ACKFlags[base++ % Seqlen] = false;   //把窗口一直向前移动，直到碰到没有收到ACK的位置，同时把这些地方初始化
			waitingState = false;
		}
		else if (ackPkt.acknum > base && !ACKFlags[ackPkt.acknum % Seqlen]) {
			cout << "已确认序号为" << ackPkt.acknum << "的ACK" << endl;
			pns->stopTimer(SENDER, ackPkt.acknum);
			ACKFlags[ackPkt.acknum % Seqlen] = true;
		}
		else
			cout << "收到不需要报文的ACK，继续等待" << endl;
	}
	else cout << "发送方收到的ACK损坏" << endl;
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	cout << "发送超时" << endl;
	pns->stopTimer(SENDER, seqNum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
	cout << "重发报文" << seqNum << endl;
	pUtils->printPacket("发送方定时器时间到，重发报文", this->packetWaitingAck[seqNum % Seqlen]);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum % Seqlen]);			//重新发送数据包
}