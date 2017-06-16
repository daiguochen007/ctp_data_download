// ctptest-md-sim.cpp : �������̨Ӧ�ó������ڵ㡣
// Guochen Dai
// 2017

#include "stdafx.h"
#include "PublicFuncs.h"
#include "MdSpi.h"
#include "TraderSpi.h"
using namespace std;

void usage(char* pName)
{
	cout << "����·��: [" << pName << "]" << endl;
	cout << "����ǰ��: [tcp://180.168.146.187:10010]" << endl;
	cout << "����ǰ��: [tcp://180.168.146.187:10000]" << endl;
	time_t local_t = time(NULL);
	cout << "����ʱ��: "<<ctime(&local_t)<< endl;
	return;
}

void getid(TThostFtdcBrokerIDType &g_BrokerID, TThostFtdcUserIDType	&g_UserID, TThostFtdcPasswordType &g_Password){
	ifstream userfile("C://NYU poly//ctp_futures//ctp_trade_sim//ctptest-sim//user.txt");
	if (!userfile){
		cout << "User file not exit";
		exit(1);
	}
	char buffer[128];
	int i = 0;
	while (!userfile.eof())
	{
		userfile.getline(buffer, 128);
		if (buffer[0] != '#' && strlen(buffer) != 0){
			//cout << buffer << endl;
			i++;
		}
		if (i == 1){
			strcpy(g_BrokerID, buffer);
		}
		if (i == 2){
			strcpy(g_UserID, buffer);
		}
		if (i == 3){
			strcpy(g_Password, buffer);
		}
	}
	userfile.close();
	return;
}

////�õ���������
//void get_date_string(string &date_string){
//	stringstream date_oss;
//
//	time_t local_t = time(NULL);
//	struct tm *local_t_ptr;
//	local_t_ptr = localtime(&local_t);
//	//�ж�
//	date_oss << local_t_ptr->tm_year + 1900;
//	date_oss.width(2);
//	date_oss.fill('0');
//	date_oss << local_t_ptr->tm_mon + 1;
//	date_oss.width(2);
//	date_oss.fill('0');
//	date_oss << local_t_ptr->tm_mday;
//
//	date_string = date_oss.str();
//}

int main(int argc, char* argv[])
{
	usage(argv[0]);

	

	memset(g_frontaddr_data, 0, BUFLEN);
	memset(g_frontaddr_trade, 0, BUFLEN);
	strcpy(g_frontaddr_data, "tcp://180.168.146.187:10010");
	strcpy(g_frontaddr_trade, "tcp://180.168.146.187:10000");

	getid(g_BrokerID, g_UserID, g_Password);
	//get_date_string(date_string);

	// ----------------------------------------------����
	// ����һ��CThostFtdcTraderApiʵ��
	CThostFtdcTraderApi *p_Trader_UserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	g_puserapi =  p_Trader_UserApi;
	// ����һ���¼������ʵ��
	CTraderSpi myspi(p_Trader_UserApi);
	// ���ý����й�ϵͳ����ĵ�ַ������ע������ַ����
	p_Trader_UserApi->RegisterFront(g_frontaddr_trade);
	// ע��һ�¼������ʵ��
	p_Trader_UserApi->RegisterSpi(&myspi);
	// ����˽����
	p_Trader_UserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	// ���Ĺ�����
	p_Trader_UserApi->SubscribePublicTopic(THOST_TERT_RESUME);
	// TERT_RESTART:�ӱ������տ�ʼ�ش�
	// TERT_RESUME:���ϴ��յ�������
	// TERT_QUICK:ֻ���͵�¼��˽����������
	cout << " ����Api�汾: [" << p_Trader_UserApi->GetApiVersion() << "]" << endl;
	// ʹ�ͻ��˿�ʼ���̨����������
	p_Trader_UserApi->Init();

	// ----------------------------------------------����
	// ����һ��CThostFtdcMdApiʵ��
	CThostFtdcMdApi *p_Md_UserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	g_pmduserapi = p_Md_UserApi;
	// ����һ���¼������ʵ��
	CSimpleHandler mymdspi(p_Md_UserApi);
	// ע��һ�¼������ʵ��
	p_Md_UserApi->RegisterSpi(&mymdspi);
	// ���ý����й�ϵͳ����ĵ�ַ������ע������ַ����
	p_Md_UserApi->RegisterFront(g_frontaddr_data);
	cout << " ����Api�汾: [" << p_Md_UserApi->GetApiVersion() << "]" << endl;
	// ʹ�ͻ��˿�ʼ���̨����������
	p_Md_UserApi->Init();
    
	//-----------------------------------------------�ͷ�
	p_Trader_UserApi->Join();
	// �ͷ�APIʵ��
	p_Trader_UserApi->Release();

	p_Md_UserApi->Join();
	// �ͷ�APIʵ��
	p_Md_UserApi->Release();
	return 0;
}

