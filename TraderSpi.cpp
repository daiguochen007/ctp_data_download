#include "stdafx.h"
#include "TraderSpi.h"
using namespace std;

CTraderSpi::CTraderSpi(CThostFtdcTraderApi *pUserApi) :m_pUserApi(pUserApi){};

CTraderSpi::~CTraderSpi(){};

void CTraderSpi::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	// get BrokerID
	strcpy(reqUserLogin.BrokerID, g_BrokerID);
	// get userid
	strcpy(reqUserLogin.UserID, g_UserID);
	// get password
	strcpy(reqUserLogin.Password, g_Password);
	// 发出登陆请求
	m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
	cout<<"交易服务器请求登录中..."<<endl;
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000);
#endif
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	printf("OnFrontDisconnected.(交易前置连接断开)\n");
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//printf("OnRspUserLogin:\n");
	//printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	//printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	if (pRspInfo->ErrorID != 0) {
		// 端登失败，客户端需进行错误处理
		printf("-----------------------------\n");
		printf("登录失败...错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}

	cout<<"交易服务器登录成功！！！"<<endl;
	//printf("  前置编号:[%d]\n", pRspUserLogin->FrontID);//
	//printf("  会话编号:[%d]\n", pRspUserLogin->SessionID);//
	//printf("  最大报单编号:[%s]\n", pRspUserLogin->MaxOrderRef);
	g_nOrdLocalID = atoi(pRspUserLogin->MaxOrderRef); 
	
	//连接成功后自动
	trading_date = get_tradingdate_string();

	StartMenu();
}




void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("修改密码失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pUserPasswordUpdate == NULL)
	{
		printf("没有修改密码数据\n");
		return;
	}
	printf("-----------------------------\n");
	printf("修改密码成功\n");
	printf("-----------------------------\n");
	return;
}

//报单无响应 不触发
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField*pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("报单失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pInputOrder == NULL)
	{
		printf("没有报单数据\n");
		return;
	}
	printf("-----------------------------\n");
	printf("报单成功\n");
	printf("-----------------------------\n");
	return;
};

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("撤单失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pOrderAction == NULL)
	{
		printf("没有撤单数据\n");
		return;
	}
	printf("-----------------------------\n");
	printf("撤单成功\n");
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (pOrder->OrderStatus == 'a'){
		//printf("-----------------------------\n");
		//printf("报单错误！！！(原因：未开盘或者报价有问题)\n");
		//printf("-----------------------------\n");
	}
	else if (pOrder->OrderStatus == '5'){
		printf("-----------------------------\n");
		printf("该报单已经撤销\n");
		printf("-----------------------------\n");
	}
	else{
		printf("-----------------------------\n");
		printf("收到报单回报\n");
		Show(pOrder);
		printf("-----------------------------\n");
	}
	return;
}

void CTraderSpi::Show(CThostFtdcOrderField *pOrder)
{
	printf("-----------------------------\n");
	printf("交易所代码=[%s]\n", pOrder->ExchangeID);
	printf("交易日=[%s]\n", pOrder->TradingDay);
	//printf("会员编号=[%s]\n", pOrder->ParticipantID);
	printf("投资者编号=[%s]\n", pOrder->InvestorID);
	//printf("客户号=[%s]\n", pOrder->ClientID);
	printf("系统报单编号=[%s]\n", pOrder->OrderSysID);
	printf("本地报单编号=[%s]\n", pOrder->OrderLocalID);
	printf("前置编号=[%d]\n", pOrder->FrontID);
	printf("会话编号=[%d]\n", pOrder->SessionID);
	printf("用户本地报单号=[%s]\n", pOrder->OrderRef);
	printf("合约代码=[%s]\n", pOrder->InstrumentID);
	printf("报单价格条件=[%c]\n", pOrder->OrderPriceType);
	printf("买卖方向=[%c]\n", pOrder->Direction);
	printf("开平标志=[%c]\n", pOrder->CombOffsetFlag[0]);
	printf("投机套保标志=[%c]\n", pOrder->CombHedgeFlag[0]);
	printf("价格=[%lf]\n", pOrder->LimitPrice);
	printf("数量=[%d]\n", pOrder->VolumeTotalOriginal);
	//printf("报单来源=[%c]\n", pOrder->OrderSource);
	printf("报单状态=[%c]\n", pOrder->OrderStatus);
	printf("报单时间=[%s]\n", pOrder->InsertTime);
	printf("撤销时间=[%s]\n", pOrder->CancelTime);
	//printf("有效期类型=[%c]\n", pOrder->TimeCondition);
	//printf("GTD日期=[%s]\n", pOrder->GTDDate);
	//printf("最小成交量=[%d]\n", pOrder->MinVolume);
	//printf("止损价=[%lf]\n", pOrder->StopPrice);
	//printf("强平原因=[%c]\n", pOrder->ForceCloseReason);
	//printf("自动挂起标志=[%d]\n", pOrder->IsAutoSuspend);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("报单错误回报 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pInputOrder == NULL)
	{
		printf("没有数据\n");
		return;
	}
}

void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	printf("-----------------------------\n");
	printf("收到成交回报\n");
	Show(pTrade);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::Show(CThostFtdcTradeField *pTrade)
{
	printf("-----------------------------\n");
	printf("交易所代码=[%s]\n", pTrade->ExchangeID);
	printf("交易日=[%s]\n", pTrade->TradingDay);
	printf("会员编号=[%s]\n", pTrade->ParticipantID);
	printf("投资者编号=[%s]\n", pTrade->InvestorID);
	printf("客户号=[%s]\n", pTrade->ClientID);
	printf("成交编号=[%s]\n", pTrade->TradeID);
	printf("用户本地操作号=[%s]\n", pTrade->OrderRef);
	printf("本地报单编号=[%s]\n", pTrade->OrderLocalID);
	printf("合约代码=[%s]\n", pTrade->InstrumentID);
	printf("买卖方向=[%c]\n", pTrade->Direction);
	printf("开平标志=[%c]\n", pTrade->OffsetFlag);
	printf("投机套保标志=[%c]\n", pTrade->HedgeFlag);
	printf("成交价格=[%lf]\n", pTrade->Price);
	printf("成交数量=[%d]\n", pTrade->Volume);
	printf("结算会员编号=[%s]\n", pTrade->ClearingPartID);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pRspInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("查询投资者账户失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pRspInvestorAccount == NULL)
	{
		printf("没有查询到投资者账户\n");
		return;
	}
	printf("-----------------------------\n");
	printf("经纪公司编号=[%s]\n", pRspInvestorAccount->BrokerID);
	printf("资金帐号=[%s]\n", pRspInvestorAccount->AccountID);
	printf("上次结算准备金=[%lf]\n", pRspInvestorAccount->PreBalance);
	printf("入金金额=[%lf]\n", pRspInvestorAccount->Deposit);
	printf("出金金额=[%lf]\n", pRspInvestorAccount->Withdraw);
	printf("冻结的保证金=[%lf]\n", pRspInvestorAccount->FrozenMargin);
	printf("冻结手续费=[%lf]\n", pRspInvestorAccount->FrozenCommission);
	printf("手续费=[%lf]\n", pRspInvestorAccount->Commission);
	printf("平仓盈亏=[%lf]\n", pRspInvestorAccount->CloseProfit);
	printf("持仓盈亏=[%lf]\n", pRspInvestorAccount->PositionProfit);
	printf("可用资金=[%lf]\n", pRspInvestorAccount->Available);
	printf("可取资金=[%lf]\n", pRspInvestorAccount->WithdrawQuota);
	printf("交易所保证金=[%lf]\n", pRspInvestorAccount->ExchangeMargin);
	printf("保证金总额=[%lf]\n", pRspInvestorAccount->CurrMargin);
	printf("总权益=[%lf]\n", pRspInvestorAccount->CurrMargin + pRspInvestorAccount->FrozenMargin + pRspInvestorAccount->Available + pRspInvestorAccount->FrozenCommission
		+ pRspInvestorAccount->CloseProfit + pRspInvestorAccount->PositionProfit + pRspInvestorAccount->Deposit - pRspInvestorAccount->Withdraw);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("查询交易所失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pExchange == NULL)
	{
		printf("没有查询到交易所信息\n");
		return;
	}
	printf("-----------------------------\n");
	printf("[%s]\n", pExchange->ExchangeID);
	printf("[%s]\n", pExchange->ExchangeName);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("查询投资者持仓失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		return;
	}

	if (pInvestorPosition == NULL)
	{
		printf("没有查询到投资者持仓\n");
		return;
	}
	printf("-----------------------------\n");
	printf("经纪公司编号=[%s]\n", pInvestorPosition->BrokerID);
	printf("投资者编号=[%s]\n", pInvestorPosition->InvestorID);
	printf("交易日=[%s]\n", pInvestorPosition->TradingDay);
	printf("合约代码=[%s]\n", pInvestorPosition->InstrumentID);
	printf("合约多空方向=[%c]\n", pInvestorPosition->PosiDirection);
	printf("今持仓量=[%d]\n", pInvestorPosition->Position);
	printf("持仓日期=[%c]\n", pInvestorPosition->PositionDate);
	printf("持仓成本=[%lf]\n", pInvestorPosition->PositionCost);
	printf("持仓盈亏=[%lf]\n", pInvestorPosition->PositionProfit);
	printf("手续费=[%lf]\n", pInvestorPosition->Commission);
	return;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("查询交易编码失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		return;
	}
	if (pInstrument == NULL)
	{
		printf("没有查询到合约数据\n");
		return;
	}
	//Show(pInstrument);

	//文件名
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s\\contract_table_%s.csv", trading_date, trading_date);

	ofstream outFile;
	outFile.open(filePath, ios::app); // 文件追加写入   
	outFile << pInstrument->ExchangeID << ","
		<< pInstrument->InstrumentID << ","
		<< pInstrument->VolumeMultiple << ","
		<< pInstrument->PriceTick << ","
		<< pInstrument->ExpireDate
		<< endl;
	outFile.close();

	cout << pInstrument->InstrumentID << endl;
	instru_vec.push_back(pInstrument->InstrumentID);

	if (bIsLast){
		SubscribeMarketData_all();
	}
	return;
}

void CTraderSpi::Show(CThostFtdcInstrumentField *pRspInstrument)
{
	printf("-----------------------------\n");
	printf("交易所代码=[%s]\n", pRspInstrument->ExchangeID);
	printf("产品代码=[%s]\n", pRspInstrument->ProductID);
	printf("合约代码=[%s]\n", pRspInstrument->InstrumentID);
	printf("合约名称=[%s]\n", pRspInstrument->InstrumentName);
	printf("交割年份=[%d]\n", pRspInstrument->DeliveryYear);
	printf("交割月=[%d]\n", pRspInstrument->DeliveryMonth);
	printf("限价单最大下单量=[%d]\n", pRspInstrument->MaxLimitOrderVolume);
	printf("限价单最小下单量=[%d]\n", pRspInstrument->MinLimitOrderVolume);
	printf("市价单最大下单量=[%d]\n", pRspInstrument->MaxMarketOrderVolume);
	printf("市价单最小下单量=[%d]\n", pRspInstrument->MinMarketOrderVolume);

	printf("数量乘数=[%d]\n", pRspInstrument->VolumeMultiple);
	printf("最小变动价位=[%lf]\n", pRspInstrument->PriceTick);

	printf("创建日=[%s]\n", pRspInstrument->CreateDate);
	printf("上市日=[%s]\n", pRspInstrument->OpenDate);
	printf("到期日=[%s]\n", pRspInstrument->ExpireDate);
	printf("开始交割日=[%s]\n", pRspInstrument->StartDelivDate);
	printf("最后交割日=[%s]\n", pRspInstrument->EndDelivDate);
	printf("当前是否交易=[%d]\n", pRspInstrument->IsTrading);
	printf("-----------------------------\n");
}

void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("查询报单失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pOrder == NULL)
	{
		printf("没有查询到报单数据\n");
		return;
	}
	///全部成交 '0'
	///部分成交还在队列中 '1'
	///部分成交不在队列中 '2'
	///未成交还在队列中 '3'
	///未成交不在队列中 '4'
	///撤单 '5'
	///未知 'a'
	///尚未触发 'b'
	///已触发 'c'
	//只显示未成交还在队列中
	if (pOrder->OrderStatus == '3' || pOrder->OrderStatus == '1')
	{
		printf("-------未成交报单-------\n");
		Show(pOrder);
	}
	return;
}

void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("查询成交失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pTrade == NULL)
	{
		printf("没有查询到成交数据");
		return;
	}
	Show(pTrade);
	return;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("确认结算单失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	printf("-----------------------------\n");
	printf("--------结算单已确认！-------\n");
	printf("经纪编号=[%s]\n", pSettlementInfoConfirm->BrokerID);
	printf("投资者编号=[%s]\n", pSettlementInfoConfirm->InvestorID);
	printf("确认日期=[%s]\n", pSettlementInfoConfirm->ConfirmDate);
	printf("确认时间=[%s]\n", pSettlementInfoConfirm->ConfirmTime);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("查询行情失败 错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pDepthMarketData == NULL)
	{
		printf("没有查询到行情数据");
		return;
	}
	printf("-----------------------------\n");
	printf("交易日=[%s]\n", pDepthMarketData->TradingDay);
	printf("交易所代码=[%s]\n", pDepthMarketData->ExchangeID);
	printf("合约代码=[%s]\n\n", pDepthMarketData->InstrumentID);

	//printf("涨跌幅度=[%.2f%%]\n", 100 * (pDepthMarketData->LastPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("最新价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->LastPrice, 100 * (pDepthMarketData->LastPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("卖一价格=[%.2lf]   卖一量=[%.0lf]\n", pDepthMarketData->AskPrice1, pDepthMarketData->AskVolume1);
	printf("买一价格=[%.2lf]   买一量=[%.0lf]\n\n", pDepthMarketData->BidPrice1, pDepthMarketData->BidVolume1);

	printf("涨停板价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->UpperLimitPrice, 100 * (pDepthMarketData->UpperLimitPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("跌停板价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->LowerLimitPrice, 100 * (pDepthMarketData->LowerLimitPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("今开盘价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->OpenPrice, 100 * (pDepthMarketData->OpenPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("今最高价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->HighestPrice, 100 * (pDepthMarketData->HighestPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("今最低价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->LowestPrice, 100 * (pDepthMarketData->LowestPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("昨结算价=[%.2lf]\n", pDepthMarketData->PreSettlementPrice);
	printf("昨收盘价=[%.2lf]\n", pDepthMarketData->PreClosePrice);
	printf("成交量=[%d]\n", pDepthMarketData->Volume);
	printf("持仓量=[%d]\n", pDepthMarketData->OpenInterest);
	printf("-----------------------------\n");
	return;
}


void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printf("OnRspError:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	// 客户端需进行错误处理
}
