#if!defined(TRADERSPI_H)
#define TRADERSPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	// ���캯������Ҫһ����Ч��ָ��CThostFtdcMduserApiʵ����ָ��
	CTraderSpi(CThostFtdcTraderApi *pUserApi);
	~CTraderSpi();

	//---------------- ��¼ ----------------//
	// ���ͻ����뽻���й�ϵͳ������ͨ�����ӣ��ͻ�����Ҫ���е�¼
	void OnFrontConnected();
	// ���ͻ����뽻���й�ϵͳͨ�����ӶϿ�ʱ���÷���������
	void OnFrontDisconnected(int nReason);
	// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
	void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//�޸�����
	void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//---------------- ���� ----------------//
	// ����¼��Ӧ��
	void OnRspOrderInsert(CThostFtdcInputOrderField*pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// �����ر�
	void OnRtnOrder(CThostFtdcOrderField *pOrder);
	// ��������ر�
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	// �ɽ��ر�
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
	// �����ر�
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ���㵥ȷ��
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// ����û�����ĳ���֪ͨ
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//---------------- ��ѯ ----------------//
	// ��ѯͶ�����˻�
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pRspInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ������
	void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ�ֲ�
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ��Լ
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ����
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ�ɽ�
	void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// ��ѯ����
	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	//---------------- չʾ ----------------//
	void Show(CThostFtdcOrderField *pOrder); //����չʾ
	void Show(CThostFtdcTradeField *pTrade); //�ɽ�չʾ
	void Show(CThostFtdcInstrumentField *pRspInstrument);//��Լչʾ

private:
	// ָ��CThostFtdcMduserApiʵ����ָ��
	CThostFtdcTraderApi *m_pUserApi;
};

#endif