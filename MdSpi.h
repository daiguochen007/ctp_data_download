#if!defined(MDSPI_H)
#define MDSPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"

class CSimpleHandler : public CThostFtdcMdSpi
{
public:
	// 构造函数，需要一个有效的指向CThostFtdcMdApi实例的指针
	CSimpleHandler(CThostFtdcMdApi *pUserApi);
	~CSimpleHandler();
	// 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
	void OnFrontConnected();
	
	// 当客户端与交易托管系统通信连接断开时，该方法被调用
	void OnFrontDisconnected(int nReason);
	
	// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// 订阅行情应答
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	// 行情应答
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField*pDepthMarketData);

	// 针对用户请求的出错通知
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	// 指向CThostFtdcMdApi实例的指针
	CThostFtdcMdApi *m_pUserApi;
};



#endif