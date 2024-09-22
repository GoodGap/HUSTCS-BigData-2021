#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"

StopWaitRdtSender::StopWaitRdtSender():base(0),expectSequenceNumberSend(0),waitingState(false)
{
	for (int i = 0; i < Seqlen; i++) {
		ACKFlags[i] = false;  //��ʼ������������ACK��Ĭ��δ�յ�
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
	if (this->waitingState) //���ͷ����ڵȴ�ȷ��״̬
		return false;

	if (expectSequenceNumberSend < base + N) {
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].acknum = -1; //���Ը��ֶ�
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].seqnum = this->expectSequenceNumberSend;
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = 0;
		memcpy(this->packetWaitingAck[expectSequenceNumberSend % Seqlen].payload, message.data, sizeof(message.data));
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);
		ACKFlags[expectSequenceNumberSend % Seqlen] = false;

		pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);
		
		cout << "���ͷ���������" << expectSequenceNumberSend << "�ļ�ʱ��" << endl;
		pns->startTimer(SENDER, Configuration::TIME_OUT, expectSequenceNumberSend);			//�������ͻ����з���ʱ������ÿ�����Ķ���һ�������ļ�ʱ����
		
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);		//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
		expectSequenceNumberSend++;
		
		cout << "�˱��ķ��ͺ���һ�������seqnum = " << expectSequenceNumberSend << endl;
		if (expectSequenceNumberSend == base + N)
			this->waitingState = true;//����ȴ�״̬
	}
	return true;
}

void StopWaitRdtSender::receive(const Packet& ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//���У�����ȷ������ȷ�����=���ͷ��ѷ��Ͳ��ȴ�ȷ�ϵ����ݰ����
	if (checkSum == ackPkt.checksum) {
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		for (int i = base + N; i < base + 8; i++)
			packetWaitingAck[i % Seqlen].seqnum = -1;  //���û�����
		cout << "���ͷ���ǰ��������Ϊ�����ִ����ѷ��ͣ� " << '[' << ' ';
		for (int i = base; i < base + N; i++) {
			if (packetWaitingAck[i % Seqlen].seqnum == -1)
				cout << '*' << ' ';
			else
				cout << packetWaitingAck[i % Seqlen].seqnum << ' ';
		}
		cout << ']' << endl;

		if (ackPkt.acknum == base) {
			cout << "��ȷ�����Ϊ" << ackPkt.acknum << "��ACK�������Ϊ��ʱ�����base�����ͷ�����������ǰ�ƶ�" << endl;
			pns->stopTimer(SENDER, ackPkt.acknum);
			ACKFlags[base % Seqlen] = true;
			while (ACKFlags[base % Seqlen])
				ACKFlags[base++ % Seqlen] = false;   //�Ѵ���һֱ��ǰ�ƶ���ֱ������û���յ�ACK��λ�ã�ͬʱ����Щ�ط���ʼ��
			waitingState = false;
		}
		else if (ackPkt.acknum > base && !ACKFlags[ackPkt.acknum % Seqlen]) {
			cout << "��ȷ�����Ϊ" << ackPkt.acknum << "��ACK" << endl;
			pns->stopTimer(SENDER, ackPkt.acknum);
			ACKFlags[ackPkt.acknum % Seqlen] = true;
		}
		else
			cout << "�յ�����Ҫ���ĵ�ACK�������ȴ�" << endl;
	}
	else cout << "���ͷ��յ���ACK��" << endl;
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	cout << "���ͳ�ʱ" << endl;
	pns->stopTimer(SENDER, seqNum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
	cout << "�ط�����" << seqNum << endl;
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط�����", this->packetWaitingAck[seqNum % Seqlen]);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum % Seqlen]);			//���·������ݰ�
}