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
	if (this->waitingState) //���ͷ����ڵȴ�ȷ��״̬
		return false;

	if (init_flag == 1) {
		for (int i = 0; i < Seqlen; i++)
			this->packetWaitingAck[i].seqnum = -1; //δ����ʼ���������ݰ���seqnumȫ����Ϊ-1
		init_flag = 0;
	}

	if (expectSequenceNumberSend < base + N) { //��ǰ�����л���û���͵����ݰ���׼�������µġ�������ʵ�ʲ����ж���Ҫ�Ա��ȡģ
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].acknum = -1; //���Ը��ֶ�
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].seqnum = this->expectSequenceNumberSend; //д�����ݱ��ı��
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = 0;
		memcpy(this->packetWaitingAck[expectSequenceNumberSend % Seqlen].payload, message.data, sizeof(message.data));
		this->packetWaitingAck[expectSequenceNumberSend % Seqlen].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);

		pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);
		if (base == expectSequenceNumberSend) {
			cout << "���ͷ�������ʱ��" << endl;
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);			//�������ͻ����з���ʱ��
		}
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend % Seqlen]);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
		expectSequenceNumberSend++;
		cout << "�˱��ķ��ͺ���һ�������seqnum = " << expectSequenceNumberSend << endl;
		if (expectSequenceNumberSend == base + N)
			this->waitingState = true;//����ȴ�״̬
	}
	return true;
}

void StopWaitRdtSender::receive(const Packet& ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//���У�����ȷ�������յ���ȷ�����=���ͷ��ѷ��Ͳ��ȴ�ȷ�ϵ����ݰ����
	if (checkSum == ackPkt.checksum && ackPkt.acknum >= base) {
		int old_base = base;
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		base = ackPkt.acknum + 1; //acknum֮ǰ�������ݰ����ѱ����շ�����
		for (int i = base + N; i < base + 8; i++)
			packetWaitingAck[i % Seqlen].seqnum = -1;  //���û�����
		cout << "���ͷ���ǰ��������Ϊ�����ִ����ѷ��ͣ� " << '[' << ' ';
		for (int i = base; i < base + N; i++) {
			if (packetWaitingAck[i % Seqlen].seqnum == -1) //��δ����
				cout << '*' << ' ';
			else //�Ѿ�����
				cout << packetWaitingAck[i % Seqlen].seqnum << ' ';
		}
		cout << ']' << endl;

		if (base == expectSequenceNumberSend)   //��ǰ��base���ڽ�����Ҫ���͵ı��ĺ�
		{
			cout << "�ѷ��ͷ�����ȫ�������շ����գ��رռ�ʱ��" << endl;
			this->waitingState = false;
			pns->stopTimer(SENDER, old_base);	//�رն�ʱ��
		}
		else {
			pns->stopTimer(SENDER, old_base);//��û�����꣬�����ȴ�
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);//�ӵ�ǰ����base����������ʱ��
			this->waitingState = false;
		}
	}

	else {
		if (checkSum != ackPkt.checksum)
			cout << "���ͷ��յ���ACK��" << endl;
		else
			cout << "���ͷ�û���յ���ȷ�����ݰ�ACK��ţ������ȴ�" << endl;
	}
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	//Ψһһ����ʱ��,���迼��seqNum
	cout << "���ͳ�ʱ������N��" << endl;
	pns->stopTimer(SENDER, seqNum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
	int i = base;
	do {
		cout << "�ط�����" << i << endl;
		pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط�����", this->packetWaitingAck[i % Seqlen]);
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[i % Seqlen]);			//���·������ݰ�
		i++;
	} while (i != expectSequenceNumberSend);
}