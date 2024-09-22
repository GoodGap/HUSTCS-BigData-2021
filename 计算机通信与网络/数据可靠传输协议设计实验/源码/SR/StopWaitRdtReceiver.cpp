#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

StopWaitRdtReceiver::StopWaitRdtReceiver():NextSeqNum(0),base(0)
{
	NextSeqNum = base + N;  //���շ����������Ͻ�
	lastAckPkt.acknum = -1; //��ʼ״̬�£��ϴη��͵�ȷ�ϰ���ȷ�����Ϊ-1��ʹ�õ���һ�����ܵ����ݰ�����ʱ��ȷ�ϱ��ĵ�ȷ�Ϻ�Ϊ-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//���Ը��ֶ�
	for(int i = 0; i < Configuration::PAYLOAD_SIZE;i++)
		lastAckPkt.payload[i] = '.';
	for (int i = 0; i < Seqlen; i++)
		packetWaitingFlags[i] = false;  //��ʼ������false��Ǵ�����ÿһ�����ݰ������ڵȴ�״̬�����ڻ���������δ����
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

StopWaitRdtReceiver::~StopWaitRdtReceiver()
{
}

void StopWaitRdtReceiver::receive(const Packet &packet) {
	int checkSum = pUtils->calculateCheckSum(packet);

	//���У�����ȷ��ͬʱ�յ����ĵ���ŵ��ڽ��շ��ڴ��յ��ı������
	if (checkSum == packet.checksum) {
		cout << "���շ�������������Ϊ " << '[' << ' ';
		for (int i = 0; i < 4; i++)
			cout << base + i << ' ';
		cout << ']' << endl;

		if (base == packet.seqnum) {  //Ϊ���շ�����������ǰ���һ��
			cout << "���շ��յ�����" << packet.seqnum << endl;

			pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
			packetWaitingFlags[packet.seqnum % Seqlen] = true;
			ReceivedPacket[packet.seqnum % Seqlen] = packet;
			ReceivedPacket[packet.seqnum % Seqlen].acknum = 0;
			while (packetWaitingFlags[base % Seqlen] == true)  //�����д��ڵȴ�״̬�������������ݰ���ȡ��Message�����ϵݽ���Ӧ�ò�
			{
				Message msg;
				memcpy(msg.data, ReceivedPacket[base % Seqlen].payload, sizeof(ReceivedPacket[base % Seqlen].payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				packetWaitingFlags[base++ % Seqlen] = false;  //�ͷŻ�����
				packetWaitingFlags[NextSeqNum++ % Seqlen] = false;  //���մ��ں��ƣ���ʼ�����ͷŻ�����
				ReceivedPacket[packet.seqnum % Seqlen].acknum = -1;
			}
		}
		else if (base < packet.seqnum && packet.seqnum < NextSeqNum) {  //��һ���յ����ǽ��շ�base�����ݰ���ֻ���治����
			cout << "���շ��յ�����" << packet.seqnum << endl;
			pUtils->printPacket("���շ��ѻ��汨��", packet);

			//�����ݰ����뻺����
			ReceivedPacket[packet.seqnum % Seqlen] = packet;
			packetWaitingFlags[packet.seqnum % Seqlen] = true;  //�����ݰ��ȴ�����

			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		}
		else if(packet.seqnum >= base-N && packet.seqnum < base)  //�ڽ��շ�base��������ݰ�����Ҫ�ط�ACK����Ϊ֮ǰ��ACK���ܶ�ʧ
		{
			pUtils->printPacket("���շ���ȷ�յ���ȷ�Ϲ��Ĺ�ʱ����", packet);
			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		}
		else {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı��ģ�������Ų���[base-N, base+N-1]��Χ��", packet);
			cout << "��ʱ���շ��ڴ��������" << this->base << "~" << this->NextSeqNum << "֮��" << endl;
		}
	}
	else pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı��ģ����ͷ����ݰ�checksum����", packet);
}