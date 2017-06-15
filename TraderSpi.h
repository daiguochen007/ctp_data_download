#if!defined(TRADERSPI_H)
#define TRADERSPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	// 构造函数，需要一个有效的指向CThostFtdcMduserApi实例的指针
	CTraderSpi(CThostFtdcTraderApi *pUserApi);
	~CTraderSpi();

	//---------------- 登录 ----------------//
	// 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
	void OnFrontConnected();
	// 当客户端与交易托管系统通信连接断开时，该方法被调用
	void OnFrontDisconnected(int nReason);
	// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
	void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//修改密码
	void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//---------------- 交易 ----------------//
	// 报单录入应答
	void OnRspOrderInsert(CThostFtdcInputOrderField*pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 报单回报
	void OnRtnOrder(CThostFtdcOrderField *pOrder);
	// 报单错误回报
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	// 成交回报
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
	// 撤单回报
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 结算单确认
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// 针对用户请求的出错通知
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//---------------- 查询 ----------------//
	// 查询投资者账户
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pRspInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询交易所
	void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询持仓
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询合约
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询报单
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询成交
	void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	// 查询行情
	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	//---------------- 展示 ----------------//
	void Show(CThostFtdcOrderField *pOrder); //报单展示
	void Show(CThostFtdcTradeField *pTrade); //成交展示
	void Show(CThostFtdcInstrumentField *pRspInstrument);//合约展示

private:
	// 指向CThostFtdcMduserApi实例的指针
	CThostFtdcTraderApi *m_pUserApi;
};

#endif