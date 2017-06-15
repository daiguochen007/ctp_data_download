#if!defined(MDSPI_H)
#define MDSPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"

class CSimpleHandler : public CThostFtdcMdSpi
{
public:
	// ���캯������Ҫһ����Ч��ָ��CThostFtdcMdApiʵ����ָ��
	CSimpleHandler(CThostFtdcMdApi *pUserApi);
	~CSimpleHandler();
	// ���ͻ����뽻���й�ϵͳ������ͨ�����ӣ��ͻ�����Ҫ���е�¼
	void OnFrontConnected();
	
	// ���ͻ����뽻���й�ϵͳͨ�����ӶϿ�ʱ���÷���������
	void OnFrontDisconnected(int nReason);
	
	// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// ��������Ӧ��
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ȡ����������Ӧ��
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// ����Ӧ��
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField*pDepthMarketData);

	// ����û�����ĳ���֪ͨ
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	// ָ��CThostFtdcMdApiʵ����ָ��
	CThostFtdcMdApi *m_pUserApi;
};



#endif