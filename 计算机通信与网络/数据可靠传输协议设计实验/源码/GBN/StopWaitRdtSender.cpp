#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"

StopWaitRdtSender::StopWaitRdtSender():base(0),expectSequenceNumberSend(0),waitingState(false)
{
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

	if (init_flag == 1) {
		for (int i = 0; i < Seqlen; i++)
			this->packetWaitingAck[i].seqnum = -1; //未作初始化，将数据包的seqnum全部赋为-1
		init_flag = 0;
	}

	if (expectSequenceNumberSend < base + N) { //当前窗口中还有没发送的数据包，准备发送新的。下面在实际操作中都需要对编号取模
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].acknum = -1; //忽略该字段
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].seqnum = this->expectSequenceNumberSend; //写入数据报文编号
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = 0;
		memcpy(this->packetWaitingAck[expectSequenceNumberSend % Seqlen].payload, message.data, sizeof(message.data));
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);

		pUtils->printPacket("发送方发送报文", this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);
		if (base == expectSequenceNumberSend) {
			cout << "发送方启动计时器" << endl;
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);			//启动发送基序列方定时器
		}
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
		expectSequenceNumberSend++;
		cout << "此报文发送后，下一发送序号seqnum = " << expectSequenceNumberSend << endl;
		if (expectSequenceNumberSend == base + N)
			this->waitingState = true;//进入等待状态
	}
	return true;
}

void StopWaitRdtSender::receive(const Packet& ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确，并且收到的确认序号=发送方已发送并等待确认的数据包序号
	if (checkSum == ackPkt.checksum && ackPkt.acknum >= base) {
		int old_base = base;
		pUtils->printPacket("发送方正确收到确认", ackPkt);
		base = ackPkt.acknum + 1; //acknum之前所有数据包均已被接收方接收
		for (int i = base + N; i < base + 8; i++)
			packetWaitingAck[i % Seqlen].seqnum = -1;  //重置缓冲区
		cout << "发送方当前滑动窗口为（数字代表已发送） " << '[' << ' ';
		for (int i = base; i < base + N; i++) {
			if (packetWaitingAck[i % Seqlen].seqnum == -1) //还未发出
				cout << '*' << ' ';
			else //已经发出
				cout << packetWaitingAck[i % Seqlen].seqnum << ' ';
		}
		cout << ']' << endl;

		if (base == expectSequenceNumberSend)   //当前的base等于接下来要发送的报文号
		{
			cout << "已发送分组已全部被接收方接收，关闭计时器" << endl;
			this->waitingState = false;
			pns->stopTimer(SENDER, old_base);	//关闭定时器
		}
		else {
			pns->stopTimer(SENDER, old_base);//还没接收完，继续等待
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);//从当前的新base重新启动计时器
			this->waitingState = false;
		}
	}

	else {
		if (checkSum != ackPkt.checksum)
			cout << "发送方收到的ACK损坏" << endl;
		else
			cout << "发送方没有收到正确的数据包ACK序号，继续等待" << endl;
	}
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	//唯一一个定时器,无需考虑seqNum
	cout << "发送超时，回退N步" << endl;
	pns->stopTimer(SENDER, seqNum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
	int i = base;
	do {
		cout << "重发报文" << i << endl;
		pUtils->printPacket("发送方定时器时间到，重发报文", this->packetWaitingAck[i % Seqlen]);
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[i % Seqlen]);			//重新发送数据包
		i++;
	} while (i != expectSequenceNumberSend);
}