#include "stdafx.h"
using namespace std;

CSimpleHandler::CSimpleHandler(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi) {}

CSimpleHandler::~CSimpleHandler() {}

// ���ͻ����뽻���й�ϵͳ������ͨ�����ӣ��ͻ�����Ҫ���е�¼
void CSimpleHandler::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	// get BrokerID
	strcpy(reqUserLogin.BrokerID, g_BrokerID);
	// get userid
	strcpy(reqUserLogin.UserID, g_UserID);
	// get password
	strcpy(reqUserLogin.Password, g_Password);
	// ������½����
	m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
	cout<<"��������������¼��..."<<endl;
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000);
#endif
}

// ���ͻ����뽻���й�ϵͳͨ�����ӶϿ�ʱ���÷���������
void CSimpleHandler::OnFrontDisconnected(int nReason)
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	cout<<"ͨ�����ӶϿ�����������..."<<endl;
}

// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
void CSimpleHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID != 0) {
		// �˵�ʧ�ܣ��ͻ�������д�����
		printf("Failed to login, errorcode=%d errormsg=%s requestid=%dchain = %d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
		exit(-1);
	}
	// �˵ǳɹ�
	cout<<"�����������½�ɹ�������"<<endl;
	////////---------------------------------------------------------////////
	//��¼�ɹ���ʼ��������
}

//��������Ӧ��
void CSimpleHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout<<pSpecificInstrument->InstrumentID<<"���ĳɹ�!"<<endl;
		//��ͬ�ĺ�Լ�����洢 
		//�ļ�����������
		char filePath[100] = { '\0' };
		sprintf(filePath, "data\\%s_%s.csv", pSpecificInstrument->InstrumentID,date_string.c_str());

		//����ļ�������,create
		fstream _file;
		_file.open(filePath, ios::in);
		if (!_file)
		{
			cout << filePath << "�����ڣ���������..." << endl;
			ofstream outFile;
			outFile.open(filePath, ios::out); // �¿��ļ�
			outFile << "��Լ����" << ","
				<< "��������" << ","
				<< "����ʱ��" << ","
				<< "���¼�" << ","
				<< "���һ" << ","
				<< "����һ" << ","
				<< "����һ" << ","
				<< "����һ" << ","
				<< "������" << ","
				<< "�ɽ���" << ","
				<< "�ֲ���" << ","
				<< "��ͣ���" << ","
				<< "��ͣ���"
				<< endl;
			outFile.close();
		}
	}
	else
		cerr << "���ش���! ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}
//�˶�����Ӧ��
void CSimpleHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << pSpecificInstrument->InstrumentID << "�˶��ɹ�!" << endl;
}

// ����Ӧ��
void CSimpleHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField*pDepthMarketData)
{
	//�յ��������
	printf("[%s]  [%s]  ���¼�=[%.2lf]  [% .2f%%]\n", pDepthMarketData->UpdateTime, pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, 100 * (pDepthMarketData->LastPrice / pDepthMarketData->PreSettlementPrice - 1));
	// ��tick�ش����ļ������ݿ�  

	//�ļ�����������
	char filePath[100] = { '\0' };
	sprintf(filePath, "data\\%s_%s.csv", pDepthMarketData->InstrumentID,date_string.c_str());

	ofstream outFile;
	outFile.open(filePath, ios::app); // �ļ�׷��д��   
	outFile << pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->TradingDay << ","
		<< pDepthMarketData->UpdateTime << "." << pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->LastPrice << ","
		<< pDepthMarketData->BidPrice1 << ","
		<< pDepthMarketData->BidVolume1 << ","
		<< pDepthMarketData->AskPrice1 << ","
		<< pDepthMarketData->AskVolume1 << ","
		<< pDepthMarketData->PreSettlementPrice << ","
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->OpenInterest << ","
		<< pDepthMarketData->UpperLimitPrice << ","
		<< pDepthMarketData->LowerLimitPrice
		<< endl;
	outFile.close();

};

// ����û�����ĳ���֪ͨ
void CSimpleHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspError:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,pRspInfo->ErrorMsg);
	// �ͻ�������д�����
}
